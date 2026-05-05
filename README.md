# CTR-ModSDK

*ModSDK*. *(noun)* A toolkit that allows you to create mods for Crash Team Racing (1999) in C.

[![Contributors][contributors-badge]][contributors-link] [![Discord Server][discord-badge]][discord]

[contributors-link]: https://github.com/CTR-Tools/CTR-ModSDK/graphs/contributors
[contributors-badge]: https://img.shields.io/github/contributors/CTR-Tools/CTR-ModSDK

[discord]: https://discord.gg/WHkuh2n
[discord-badge]: https://img.shields.io/discord/527135227546435584?color=%237289DA&logo=discord&logoColor=ffffff

This repository also houses an effort to fully decompile and reverse-engineer [CTR in C.](https://github.com/CTR-tools/CTR-ModSDK#CTR-in-C)

# 🏁 Modding CTR

## Requirements

CTR-ModSDK can be cloned and opened as a standalone project. The small pieces of
[psx-modding-toolchain](https://github.com/mateusfavarin/psx-modding-toolchain)
that the SDK needs are vendored under `tools/psx/`.

You still need these external tools installed:

- `uv` and Python
- `mipsel-none-elf-gcc`
- `make`
- an original CTR ISO/BIN supplied by you in `build/`
- optional: a native C/C++ compiler for `rebuild_PC`
- optional: PCSX-Redux or NoPS for hot reload workflows

Install the local Python dependencies from the SDK root with:

```sh
uv sync
```

## Downloading the SDK

Clone this repository wherever you want to work on it:

```
$ git clone https://github.com/CTR-Tools/CTR-ModSDK.git
$ cd CTR-ModSDK
$ uv sync
```

- You can find example mods in the `mods/` folder.
- Each mod folder includes a `readme.txt` file on what the mod does and how to use it. 

## Local Zed workflow

This checkout includes repo-local Zed tasks and clangd setup for C autocomplete.
Open this folder directly in Zed, then run `task: spawn` -> `lsp: refresh compile_commands`.

See [`tools/dev/README.md`](tools/dev/README.md) for the day-to-day workflow, task list, and PC compiler notes.

# CTR-in-C

As mentioned earlier, this repository is also used for an attempt at decompiling the original CTR assembly into human-readable C code. Our decompilation targets **non-matching** code that still functions identically compared to the original code.

![decompile progress](decomp_progress.png)

[![Decompile Overview](https://img.youtube.com/vi/V9QlFzSVDAU/hqdefault.jpg)](https://www.youtube.com/watch?v=V9QlFzSVDAU)

## 🤝 Contributing

Are you interested in contributing? Have any experience in C programming language? You're welcome to join!

### How to rewrite functions:

- Make sure you have cloned this repo and installed the requirements.
- Choose a .c file from the `ghidra/` folder. Each file represents a code section or category. All non-numbered .c files are parts of the main EXE while the numbered files are overlays.
- Choose a function in that section to rewrite. Read the documentation comments on what the function does and what's the address.
- Set up a compile folder for your rewritten function using the existing `decompile/` or `mods/` layout.

\* Build codenames:  
common: All versions;  
926: USA Retail;  
1006: Japan Trial;  
1020: Europe Retail;  
1111: Japan Retail.  

\**  Code region:  
exe: main EXE;  
221-233: Overlays (use the original .c filename number)

- Run `build.bat` and choose "Compile", "Build ISO", then test the game.
- After you've confirmed that it's functional, add your new function .c file to the `decompile/` directory.
- Feel free to make a pull request after everything is working!

If you have any questions, reach us out in our [Discord server](https://discord.gg/WHkuh2n).
