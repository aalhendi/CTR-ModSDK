# Zed Workflow

This folder contains the small helper scripts that make CTR-ModSDK feel closer to a simple `build.bat` workflow while still giving Zed and clangd enough information for autocomplete.

## First Run

Open this directory in Zed:

```text
CTR-ModSDK
```

Then open the command palette and run:

```text
task: spawn
```

Pick:

```text
lsp: refresh compile_commands
```

That generates `compile_commands.json` in the SDK root. The file is intentionally ignored by git. Regenerate it when `buildList.txt`, `config.json`, CMake settings, or source layout changes.

## What The Files Do

- `.zed/settings.json` tells Zed to use clangd for C/C++ and to keep formatting conservative.
- `.zed/tasks.json` exposes the common build commands in Zed's task picker.
- `.clangd` keeps clangd in C mode and disables automatic include insertion.
- `tools/dev/generate_compile_commands.py` generates the PS1/decompile LSP database.
- `tools/dev/ctr_task.py` is the small task runner used by Zed and by terminal commands.

## Common Zed Tasks

Use `task: spawn`, then pick one of these:

```text
lsp: refresh compile_commands
ps1: compile decompile
ps1: compile WorkInProgress
ps1: extract ISO ntsc-u
ps1: build ISO ntsc-u
ps1: clean
pc: configure debug
pc: build debug
pc: run debug
```

The same tasks can be run manually:

```powershell
uv run --active python tools/dev/ctr_task.py lsp-refresh
uv run --active python tools/dev/ctr_task.py ps1-compile decompile
uv run --active python tools/dev/ctr_task.py ps1-compile WorkInProgress
uv run --active python tools/dev/ctr_task.py ps1-extract --version ntsc-u
uv run --active python tools/dev/ctr_task.py ps1-build --version ntsc-u
uv run --active python tools/dev/ctr_task.py ps1-clean
```

## LSP And Autocomplete

Zed uses clangd for C. Clangd needs a compilation database, so `lsp-refresh` creates `compile_commands.json` with:

- SDK include paths from `include/`
- Nugget macro include paths from `tools/psx/nugget/common/macros/`
- active decompile build directories
- entries from `decompile/buildList.txt`
- entries from `decompile/WorkInProgress/buildList.txt`
- fallback entries for inactive decompile `.c` files, so browsing old or commented-out functions still works

If autocomplete looks stale, run:

```text
lsp: refresh compile_commands
```

Then restart clangd or reload the Zed workspace.

## PS1 Build Loop

For normal decompile work:

1. Edit C files under `decompile/General` or `decompile/WorkInProgress`.
2. Run `ps1: compile decompile` or `ps1: compile WorkInProgress`.
3. Run `ps1: build ISO ntsc-u` when you want a rebuilt disc.

The PS1 tasks use the vendored Python mod-builder under `tools/psx/mod-builder`.
They just avoid fragile interactive menu clicking by passing the right options for you.
Compile tasks default to `ntsc-u`; add `--version pal`, `--version ntsc-j`, or
another configured version when you need a different build.

Every `build.bat` in the SDK is also a root-discovery wrapper. You can still run
it from an individual mod folder, but it will find the SDK root and call the
same local mod-builder through `uv`.

If you already activated a venv before launching Zed or running a terminal
command, `uv run --active` will prefer that venv. Without an active venv, `uv`
uses the SDK-local project environment.

## PC Rebuild Loop

The PC rebuild uses CMake from `rebuild_PC`.

Run:

```text
pc: configure debug
pc: build debug
pc: run debug
```

The configure step writes to:

```text
rebuild_PC/build-zed/debug
```

When CMake successfully generates `rebuild_PC/build-zed/debug/compile_commands.json`, the next `lsp-refresh` will merge the PC compile database into the root `compile_commands.json`.

## What May Be Missing

For PS1/decompile work, this machine currently has the important pieces:

```text
uv
mipsel-none-elf-gcc
cmake
```

For `rebuild_PC`, you still need a native C/C++ compiler on `PATH`, such as:

```text
gcc and g++
clang and clang++
cl from Visual Studio Build Tools
```

The pragmatic no-Visual-Studio route on Windows is MSYS2 UCRT64 with GCC and Ninja. After installing it, make sure Zed can see the MSYS2 UCRT64 bin directory, usually:

```text
C:\msys64\ucrt64\bin
```

If `pc: configure debug` says no `CMAKE_C_COMPILER` or `CMAKE_CXX_COMPILER` was found, this is the missing piece.
