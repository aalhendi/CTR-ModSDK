from __future__ import annotations

import argparse
import json
import os
import shutil
import subprocess
import sys
from pathlib import Path


DEFAULT_VERSION = "ntsc-u"


def sdk_root() -> Path:
    return Path(__file__).resolve().parents[2]


def mod_builder_path(root: Path) -> Path:
    return root.parents[1] / "tools" / "mod-builder" / "main.py"


def version_index(root: Path, version: str) -> int:
    data = json.loads((root / "config.json").read_text(encoding="utf-8"))
    names: list[str] = []
    for item in data["versions"]:
        names.extend(item.keys())
    if version not in names:
        valid = ", ".join(names)
        raise SystemExit(f"Unknown version {version!r}. Valid versions: {valid}")
    return names.index(version) + 1


def run(command: list[str], cwd: Path) -> int:
    print(f"+ {' '.join(command)}", flush=True)
    print(f"  cwd: {cwd}", flush=True)
    return subprocess.run(command, cwd=cwd).returncode


def run_mod_builder(root: Path, cwd: Path, action: int, version: str | None = None) -> int:
    command = [sys.executable, str(mod_builder_path(root)), str(action)]
    if version is not None:
        command.append(str(version_index(root, version)))
    return run(command, cwd)


def lsp_refresh(root: Path, version: str) -> int:
    script = root / "tools" / "dev" / "generate_compile_commands.py"
    return run([sys.executable, str(script), "--version", version], root)


def ps1_compile(root: Path, target: str) -> int:
    if target == "decompile":
        return run_mod_builder(root, root / "decompile", 1)
    if target == "WorkInProgress":
        return run_mod_builder(root, root / "decompile" / "WorkInProgress", 1)
    raise SystemExit("target must be 'decompile' or 'WorkInProgress'")


def ps1_extract(root: Path, version: str) -> int:
    return run_mod_builder(root, root / "decompile", 3, version)


def ps1_build(root: Path, version: str) -> int:
    return run_mod_builder(root, root / "decompile", 4, version)


def ps1_clean(root: Path) -> int:
    return run_mod_builder(root, root / "decompile", 6)


def cmake_generator_args() -> list[str]:
    if shutil.which("ninja"):
        return ["-G", "Ninja"]
    if os.name == "nt" and shutil.which("mingw32-make"):
        return ["-G", "MinGW Makefiles"]
    if shutil.which("make"):
        return ["-G", "Unix Makefiles"]
    return []


def pc_build_dir(root: Path) -> Path:
    return root / "rebuild_PC" / "build-zed" / "debug"


def pc_configure_debug(root: Path) -> int:
    build_dir = pc_build_dir(root)
    command = [
        "cmake",
        "-S",
        str(root / "rebuild_PC"),
        "-B",
        str(build_dir),
        "-DCMAKE_BUILD_TYPE=Debug",
        "-DCMAKE_EXPORT_COMPILE_COMMANDS=ON",
    ] + cmake_generator_args()
    return run(command, root)


def pc_build_debug(root: Path) -> int:
    command = [
        "cmake",
        "--build",
        str(pc_build_dir(root)),
        "--config",
        "Debug",
        "--parallel",
    ]
    return run(command, root)


def pc_run_debug(root: Path) -> int:
    build_dir = pc_build_dir(root)
    candidates = [
        build_dir / "CrashTeamRacingPC.exe",
        build_dir / "CrashTeamRacingPC",
        build_dir / "Debug" / "CrashTeamRacingPC.exe",
        build_dir / "Debug" / "CrashTeamRacingPC",
    ]
    for candidate in candidates:
        if candidate.exists():
            return run([str(candidate)], root / "rebuild_PC")
    raise SystemExit(f"Could not find CrashTeamRacingPC executable under {build_dir}")


def main(argv: list[str] | None = None) -> int:
    parser = argparse.ArgumentParser(description="Small CTR-ModSDK task runner for editor tasks")
    subparsers = parser.add_subparsers(dest="command", required=True)

    lsp = subparsers.add_parser("lsp-refresh", help="Regenerate compile_commands.json")
    lsp.add_argument("--version", default=DEFAULT_VERSION)

    compile_parser = subparsers.add_parser("ps1-compile", help="Compile a PS1 buildList target")
    compile_parser.add_argument("target", choices=["decompile", "WorkInProgress"])

    extract = subparsers.add_parser("ps1-extract", help="Extract ISO through mod-builder")
    extract.add_argument("--version", default=DEFAULT_VERSION)

    build = subparsers.add_parser("ps1-build", help="Build ISO through mod-builder")
    build.add_argument("--version", default=DEFAULT_VERSION)

    subparsers.add_parser("ps1-clean", help="Clean PS1 ISO build outputs")
    subparsers.add_parser("pc-configure-debug", help="Configure rebuild_PC Debug build")
    subparsers.add_parser("pc-build-debug", help="Build rebuild_PC Debug build")
    subparsers.add_parser("pc-run-debug", help="Run rebuild_PC Debug executable")

    args = parser.parse_args(argv)
    root = sdk_root()

    if args.command == "lsp-refresh":
        return lsp_refresh(root, args.version)
    if args.command == "ps1-compile":
        return ps1_compile(root, args.target)
    if args.command == "ps1-extract":
        return ps1_extract(root, args.version)
    if args.command == "ps1-build":
        return ps1_build(root, args.version)
    if args.command == "ps1-clean":
        return ps1_clean(root)
    if args.command == "pc-configure-debug":
        return pc_configure_debug(root)
    if args.command == "pc-build-debug":
        return pc_build_debug(root)
    if args.command == "pc-run-debug":
        return pc_run_debug(root)

    parser.error(f"Unhandled command: {args.command}")
    return 2


if __name__ == "__main__":
    raise SystemExit(main())
