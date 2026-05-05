# Vendored PSX Modding Toolchain Files

CTR-ModSDK vendors a small subset of `psx-modding-toolchain` so the SDK can be cloned and used on its own.

Source project:

```text
https://github.com/mateusfavarin/psx-modding-toolchain
```

Source commit used for this vendor slice:

```text
9332a8d43943c513502e92a7b8b03d9950fff5bb
```

Vendored files:

```text
tools/psx/common.mk                         <- games/common.mk
tools/psx/mod-builder/*.py                  <- tools/mod-builder/*.py
tools/psx/nugget/common.mk                  <- tools/nugget/common.mk
tools/psx/nugget/common/macros/gte.h        <- tools/nugget/common/macros/gte.h
tools/psx/trimbin/trimbin.py                <- tools/trimbin/trimbin.py
```

The parent toolchain license is preserved at:

```text
tools/psx/LICENSE.psx-modding-toolchain
```

Nugget's license is preserved at:

```text
tools/psx/nugget/LICENSE
```

These files are maintained as a small compatibility slice, not as a full copy of the original toolchain repository.
