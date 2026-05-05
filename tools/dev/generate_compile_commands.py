from __future__ import annotations

import argparse
import json
import os
import re
import sys
from pathlib import Path
from typing import Iterable


DEFAULT_VERSION = "ntsc-u"
SOURCE_SUFFIXES = {".c", ".cc", ".cpp"}
COMMENT_SYMBOL = "//"


def sdk_root() -> Path:
    return Path(__file__).resolve().parents[2]


def as_posix(path: Path) -> str:
    return path.resolve().as_posix()


def load_config(root: Path) -> dict:
    return json.loads((root / "config.json").read_text(encoding="utf-8"))


def version_entries(config: dict) -> dict[str, dict]:
    entries: dict[str, dict] = {}
    for item in config["versions"]:
        entries.update(item)
    return entries


def build_id_for_version(config: dict, version: str) -> int:
    versions = version_entries(config)
    if version not in versions:
        valid = ", ".join(versions)
        raise SystemExit(f"Unknown version {version!r}. Valid versions: {valid}")
    return int(versions[version]["build_id"])


def tokenize_build_list_line(line: str) -> list[str]:
    line = line.replace(COMMENT_SYMBOL, f",{COMMENT_SYMBOL},")
    tokens = [token.strip() for token in line.split(",") if token.strip()]
    output: list[str] = []
    for token in tokens:
        if token == COMMENT_SYMBOL:
            break
        output.append(token)
    return output


def is_number(value: str) -> bool:
    try:
        int(value, 0)
        return True
    except ValueError:
        return False


def version_matches(token: str, version: str, build_id: int) -> bool:
    first = token.replace("  ", " ").split(" ")[0]
    if is_number(first):
        return int(first, 0) == build_id
    return first.lower() == "common" or first == version


def compile_list_sources(build_list: Path, version: str, build_id: int) -> list[Path]:
    sources: list[Path] = []
    seen: set[str] = set()
    base_dir = build_list.parent

    for line in build_list.read_text(encoding="utf-8").splitlines():
        tokens = tokenize_build_list_line(line)
        if len(tokens) < 5 or not version_matches(tokens[0], version, build_id):
            continue

        for source_token in tokens[4].split():
            for source in resolve_source_token(base_dir, source_token):
                if source.suffix.lower() not in SOURCE_SUFFIXES:
                    continue
                key = os.path.normcase(str(source.resolve()))
                if key not in seen:
                    sources.append(source.resolve())
                    seen.add(key)
    return sources


def resolve_source_token(base_dir: Path, source_token: str) -> Iterable[Path]:
    path = (base_dir / source_token).resolve()
    if "*" not in path.name:
        if path.exists():
            yield path
        else:
            print(f"warning: source not found: {path}", file=sys.stderr)
        return

    directory = path.parent
    if not directory.is_dir():
        print(f"warning: source directory not found: {directory}", file=sys.stderr)
        return

    pattern = re.compile(re.escape(path.name).replace(r"\*", ".*"))
    for child in sorted(directory.iterdir(), key=lambda item: item.name.lower()):
        if child.is_file() and pattern.search(child.name):
            yield child


def ps1_include_paths(root: Path, build_dir: Path, config: dict) -> list[Path]:
    tools_root = root / "tools" / "psx"
    compiler = config["compiler"]

    includes = [
        tools_root / "nugget" / "common" / "macros",
        root / "include",
        build_dir,
        build_dir / "src",
    ]
    if compiler.get("mininoob", 0) != 0:
        includes.append(tools_root / "minin00b" / "include")
    if compiler.get("psyq", 0) != 0:
        includes.append(tools_root / "gcc-psyq-converted" / "include")
    return includes


def ps1_arguments(root: Path, build_dir: Path, build_id: int, config: dict) -> list[str]:
    args = [
        "clang",
        "-xc",
        "-std=gnu99",
        "--target=mipsel-none-elf",
        "-fsyntax-only",
        "-ffreestanding",
        "-fno-builtin",
        "-fno-strict-aliasing",
        "-fno-common",
        "-DBUILD=" + str(build_id),
        "-Wno-unknown-attributes",
        "-Wno-implicit-function-declaration",
        "-Wno-implicit-int",
        "-Wno-int-conversion",
        "-Wno-incompatible-pointer-types",
        "-Wno-return-type",
    ]
    for include in ps1_include_paths(root, build_dir, config):
        args.extend(["-I", as_posix(include)])
    return args


def ps1_entries(root: Path, version: str) -> list[dict]:
    config = load_config(root)
    build_id = build_id_for_version(config, version)
    build_lists = [
        root / "decompile" / "buildList.txt",
        root / "decompile" / "WorkInProgress" / "buildList.txt",
    ]

    entries: list[dict] = []
    seen: set[str] = set()
    for build_list in build_lists:
        build_dir = build_list.parent
        args = ps1_arguments(root, build_dir, build_id, config)
        for source in compile_list_sources(build_list, version, build_id):
            key = os.path.normcase(str(source))
            if key in seen:
                continue
            entries.append({
                "directory": as_posix(build_dir),
                "file": as_posix(source),
                "arguments": args + [as_posix(source)],
            })
            seen.add(key)
    fallback_roots = [
        (root / "decompile", root / "decompile" / "General"),
        (root / "decompile" / "WorkInProgress", root / "decompile" / "WorkInProgress"),
    ]
    for build_dir, source_root in fallback_roots:
        args = ps1_arguments(root, build_dir, build_id, config)
        for source in sorted(source_root.rglob("*"), key=lambda item: as_posix(item).lower()):
            if not source.is_file() or source.suffix.lower() not in SOURCE_SUFFIXES:
                continue
            key = os.path.normcase(str(source.resolve()))
            if key in seen:
                continue
            entries.append({
                "directory": as_posix(build_dir),
                "file": as_posix(source),
                "arguments": args + [as_posix(source)],
            })
            seen.add(key)
    return entries


def cmake_entries(root: Path) -> list[dict]:
    path = root / "rebuild_PC" / "build-zed" / "debug" / "compile_commands.json"
    if not path.exists():
        return []
    return json.loads(path.read_text(encoding="utf-8"))


def write_compile_commands(root: Path, entries: list[dict]) -> Path:
    output = root / "compile_commands.json"
    output.write_text(json.dumps(entries, indent=2) + "\n", encoding="utf-8")
    return output


def main(argv: list[str] | None = None) -> int:
    parser = argparse.ArgumentParser(description="Generate clangd compile_commands.json for CTR-ModSDK")
    parser.add_argument("--version", default=DEFAULT_VERSION, help="CTR version to model for PS1 decompile files")
    parser.add_argument("--no-pc", action="store_true", help="Do not merge rebuild_PC CMake compile commands")
    args = parser.parse_args(argv)

    root = sdk_root()
    entries = ps1_entries(root, args.version)
    if not args.no_pc:
        entries.extend(cmake_entries(root))
    output = write_compile_commands(root, entries)

    print(f"Wrote {len(entries)} entries to {output}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
