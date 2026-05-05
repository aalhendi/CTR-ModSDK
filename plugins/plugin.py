import os
import subprocess
import sys
from pathlib import Path

def warning() -> None:
    print("[Plugin] WARNING: ctr-tools not found. Please download the tools from https://github.com/CTR-tools/CTR-tools/releases")
    print("rename the folder to ctr-tools and put it in CrashTeamRacing/plugins/")

def run_bigtool(plugin_path: str, game_path: str, game_version, input_name: str) -> None:
    plugin_dir = Path(plugin_path)
    game_dir = Path(game_path)
    input_path = game_dir / input_name

    compat_tool = plugin_dir / "bigtool_compat.py"
    if compat_tool.exists():
        command = [sys.executable, str(compat_tool), str(input_path)]
        if game_version:
            command.extend(["--game-version", str(game_version)])
        subprocess.check_call(command)
        return

    windows_tool = plugin_dir / "ctr-tools" / "bigtool.exe"
    if os.name == "nt" and windows_tool.exists():
        subprocess.check_call([str(windows_tool), str(input_path)])
        return

    warning()

def extract(plugin_path: str, game_path: str, game_version: None) -> None:
    run_bigtool(plugin_path, game_path, game_version, "BIGFILE.BIG")

def build(plugin_path: str, game_path: str, game_version: None) -> None:
    run_bigtool(plugin_path, game_path, game_version, "bigfile.txt")
