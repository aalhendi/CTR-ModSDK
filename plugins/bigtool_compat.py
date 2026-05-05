from __future__ import annotations

import argparse
import hashlib
import re
import struct
import sys
import xml.etree.ElementTree as ET
from dataclasses import dataclass
from pathlib import Path


SECTOR_SIZE = 0x800
HEADER_SECTORS = 3
TABLE_OFFSET = 8
MAX_FILES = 2048

DATA_DIR = Path(__file__).with_name("bigtool_compat_data")

VERSION_ALIASES = {
    "rewrite": "ntsc-u",
    "ntsc-j-trial": "ntsc-j",
}

VERSION_LISTS = {
    "ntsc-u": "big_usa_release.txt",
    "pal": "big_pal_release.txt",
    "ntsc-j": "big_usa_release.txt",
    "proto-sep3": "big_usa_beta_sep.txt",
}

FILE_COUNT_LISTS = {
    608: "big_usa_release.txt",
    723: "big_pal_release.txt",
    453: "big_usa_beta_aug.txt",
    511: "big_usa_beta_14th.txt",
    609: "big_usa_beta_sep.txt",
    189: "big_usa_demo.txt",
    196: "big_usa_demo.txt",
}


@dataclass(frozen=True)
class BigEntry:
    name: str
    sector_offset: int
    size: int
    data: bytes


def padded_size(size: int) -> int:
    return (size + SECTOR_SIZE - 1) // SECTOR_SIZE * SECTOR_SIZE


def normalize_version(game_version: str | None) -> str | None:
    if not game_version:
        return None
    return VERSION_ALIASES.get(game_version, game_version)


def split_entry_name(name: str) -> list[str]:
    parts = [part for part in re.split(r"[\\/]+", name) if part]
    if any(part in {".", ".."} for part in parts):
        raise ValueError(f"Unsafe BIG entry path: {name}")
    return parts


def entry_path(root: Path, name: str) -> Path:
    return root.joinpath(*split_entry_name(name))


def default_build_output(manifest_path: Path) -> Path:
    lower_output = manifest_path.with_name(f"{manifest_path.stem.lower()}.big")
    for sibling in manifest_path.parent.iterdir():
        if sibling.is_file() and sibling.name.lower() == lower_output.name.lower():
            return sibling
    return lower_output


def load_versions() -> ET.Element:
    return ET.parse(DATA_DIR / "versions.xml").getroot()


def load_numbered_list(filename: str) -> dict[int, str]:
    names: dict[int, str] = {}
    for raw_line in (DATA_DIR / filename).read_text(encoding="utf-8-sig").splitlines():
        line = raw_line.split("#", 1)[0].strip().replace(" ", "")
        if not line:
            continue
        number, name = line.split("=", 1)
        names[int(number)] = name.strip()
    return names


def list_from_md5(md5: str) -> str | None:
    root = load_versions()
    for entry in root.findall("./big/entry"):
        md5_node = entry.find("md5")
        list_node = entry.find("list")
        if md5_node is not None and list_node is not None and md5_node.text:
            if md5_node.text.lower() == md5.lower():
                return list_node.text
    return None


def choose_list_filename(md5: str, file_count: int, game_version: str | None) -> str | None:
    version = normalize_version(game_version)
    if version in VERSION_LISTS:
        return VERSION_LISTS[version]
    return list_from_md5(md5) or FILE_COUNT_LISTS.get(file_count)


def entry_names(md5: str, file_count: int, game_version: str | None) -> list[str]:
    list_filename = choose_list_filename(md5, file_count, game_version)
    numbered = load_numbered_list(list_filename) if list_filename else {}
    return [numbered.get(index, f"file_{index:04d}.bin") for index in range(file_count)]


def read_big(path: Path, game_version: str | None = None) -> list[BigEntry]:
    data = path.read_bytes()
    if len(data) < TABLE_OFFSET:
        raise ValueError(f"{path} is too small to be a CTR BIG file")

    first, file_count = struct.unpack_from("<ii", data, 0)
    if first != 0:
        raise ValueError(f"{path} is unlikely a CTR BIG file")
    if file_count > MAX_FILES:
        raise ValueError(f"{path} is unlikely a CTR BIG file, more than {MAX_FILES} files")

    table_size = file_count * 8
    if len(data) < TABLE_OFFSET + table_size:
        raise ValueError(f"{path} has a truncated BIG table")

    table: list[tuple[int, int]] = []
    for index in range(file_count):
        sector_offset, size = struct.unpack_from("<ii", data, TABLE_OFFSET + index * 8)
        if sector_offset < 0 or size < 0:
            raise ValueError(f"{path} has a negative BIG table entry at {index}")
        if sector_offset > len(data) or sector_offset + size > len(data):
            raise ValueError(f"{path} has an invalid BIG table entry at {index}")
        byte_offset = sector_offset * SECTOR_SIZE
        if byte_offset + size > len(data):
            raise ValueError(f"{path} has an out-of-bounds BIG entry at {index}")
        table.append((sector_offset, size))

    md5 = hashlib.md5(data).hexdigest()
    names = entry_names(md5, file_count, game_version)

    entries: list[BigEntry] = []
    for index, (sector_offset, size) in enumerate(table):
        byte_offset = sector_offset * SECTOR_SIZE
        entries.append(BigEntry(names[index], sector_offset, size, data[byte_offset:byte_offset + size]))
    return entries


def extract_big(input_path: Path, output_dir: Path | None = None, game_version: str | None = None) -> Path:
    input_path = input_path.resolve()
    if output_dir is None:
        output_dir = input_path.with_name(input_path.stem.lower())
    entries = read_big(input_path, game_version)

    manifest_lines: list[str] = []
    for entry in entries:
        manifest_lines.append(entry.name)
        if entry.size == 0:
            continue
        out_path = entry_path(output_dir, entry.name)
        out_path.parent.mkdir(parents=True, exist_ok=True)
        out_path.write_bytes(entry.data)

    manifest_path = output_dir.with_suffix(".txt")
    manifest_path.write_bytes(("\r\n".join(manifest_lines) + "\r\n").encode("ascii"))
    return output_dir


def build_big(manifest_path: Path, output_path: Path | None = None) -> Path:
    manifest_path = manifest_path.resolve()
    if output_path is None:
        output_path = default_build_output(manifest_path)

    source_root = manifest_path.with_suffix("")
    names = manifest_path.read_text(encoding="ascii").splitlines()

    payloads: list[bytes] = []
    total_size = HEADER_SECTORS * SECTOR_SIZE
    for name in names:
        path = entry_path(source_root, name)
        data = path.read_bytes() if path.is_file() else b""
        payloads.append(data)
        total_size += padded_size(len(data))

    out = bytearray(total_size)
    struct.pack_into("<ii", out, 0, 0, len(names))

    cursor = HEADER_SECTORS * SECTOR_SIZE
    table_entries: list[tuple[int, int]] = []
    for data in payloads:
        sector_offset = cursor // SECTOR_SIZE
        out[cursor:cursor + len(data)] = data
        table_entries.append((sector_offset, len(data)))
        cursor += padded_size(len(data))

    for index, (sector_offset, size) in enumerate(table_entries):
        struct.pack_into("<ii", out, TABLE_OFFSET + index * 8, sector_offset, size)

    output_path.write_bytes(out)
    return output_path


def main(argv: list[str] | None = None) -> int:
    parser = argparse.ArgumentParser(description="Extracts and rebuilds Crash Team Racing BIG files")
    parser.add_argument("input", type=Path, help="Path to BIGFILE.BIG or bigfile.txt")
    parser.add_argument("--game-version", help="CTR-ModSDK game version, used for extraction names")
    parser.add_argument("--output", type=Path, help="Optional output path")
    args = parser.parse_args(argv)

    suffix = args.input.suffix.lower()
    try:
        if suffix == ".big":
            out_dir = extract_big(args.input, args.output, args.game_version)
            print(f"Extracted BIG to: {out_dir}")
        elif suffix == ".txt":
            out_file = build_big(args.input, args.output)
            print(f"Built BIG: {out_file}")
        else:
            print(f"Unsupported file type: {args.input.suffix}", file=sys.stderr)
            return 2
    except Exception as exc:
        print(f"Error: {exc}", file=sys.stderr)
        return 1
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
