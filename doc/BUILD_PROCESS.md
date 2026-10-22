# Linux Build Process Reference

This document describes how ViolaWWW is built on Linux (tested on Debian 13
"trixie", x86_64, GCC 14): what the Makefile detects, the order things are
built in, the available targets, and how to fix common build errors.

For installing the packages and a first build, start with
[DEBIAN.md](DEBIAN.md).

---

## Overview

```
make          →  src/vw/vw          Motif browser (the one you normally run)
                 src/viola/viola    plain X11 browser (no Motif)
                 src/viola/sgmlsA2B HMML converter (only if OpenSP is installed)
                 vplot_dir/vplot    3D surface plotter
```

Everything is built in place; there is no separate build directory.

---

## Configuration Detection

The top-level `Makefile` checks `uname -s` and, on Linux, detects libraries
as follows:

| What | How it is found | If missing |
|------|-----------------|------------|
| X11 libraries | `pkg-config x11 xext xmu xt sm ice xrender xp`, falling back to `-lXext -lXmu -lXt -lSM -lICE -lX11 -lXrender` | Build fails |
| Motif | Headers and library under `/usr` (`libmotif-dev`) | Only `viola` can be built |
| OpenSSL | `pkg-config openssl` | Built without HTTPS (warning printed) |
| ICU | `pkg-config icu-i18n` | Built without UTF-8 transliteration (warning printed) |
| OpenSP | `onsgmls` in `PATH` | `sgmlsA2B` not built; HMML unsupported (warning printed) |

`xp` (the X print extension) no longer exists on Debian, so the `pkg-config`
check fails and the fallback library list is used. This is expected.

Run `make info` to see the compiler, flags and detected paths.

### Compiler flags

Libraries and the browser are compiled with `-O2 -std=gnu17
-funsigned-char -fno-common -DVIOLA_LINUX`, plus `-DUSE_SSL` / `-DUSE_ICU`
when those are available. Warnings use `-Wall -Wextra
-Wno-unused-parameter -Wno-missing-field-initializers`; a few hundred
harmless warnings (mostly sign comparisons and missing braces in old tables)
remain and are expected.

`-funsigned-char` matters: much of the original 1990s code assumes `char`
is unsigned.

---

## Build Stages

`make` runs these stages; `make -j"$(nproc)"` runs them in parallel.

### 1. Libraries

| Library | Output | Contents |
|---------|--------|----------|
| libWWW | `src/libWWW/libwww.a` | CERN WWW library: HTTP, HTTPS (OpenSSL), FTP, Gopher, News, Web Archive fallback, keep-alive pool, charset conversion |
| libXPM | `src/libXPM/libXpm.a` | Bundled X Pixmap library |
| libXPA | `src/libXPA/src/libXpa.a` | X Pixmap Applets |
| libIMG | `src/libIMG/libIMG.a` | GIF, XBM, XPM and other image decoders |
| libStyle | `src/libStyle/libstg.o` | STG stylesheet parser |

### 2. Script grammar

`src/viola/gram.y` is turned into `src/viola/gram.c` by **byacc** (Berkeley
Yacc). GNU Bison is not used, for compatibility with the original SunOS
yacc grammar.

### 3. Embedded Viola scripts

Much of the browser's behavior is written in Viola script. Each script in
`src/viola/embeds/*.v` is converted to a C string literal in a matching
`*.v.h` file using `hexdump` (package `bsdextrautils`), and
`src/viola/objs.c` includes them all.

If `hexdump` is missing, the `.v.h` files come out empty and the browser
will not work properly, so make sure `bsdextrautils` is installed.

### 4. Viola core and front ends

- `src/viola/*.c` — the Viola object system, script interpreter, HTML
  builder and X11 drawing code
- `src/vw/*.c` — the Motif front end (menus, dialogs, title bar)
- `vw` links the Motif front end with the Viola core (without its `main`);
  `viola` links the core with its own plain X11 `main`

### 5. Helpers

- `src/viola/sgmlsA2B` — converts HMML documents using `onsgmls`; the path
  found at build time is compiled in
- `vplot_dir/vplot` — the 3D plotter, built from `src/vplot/vplot.c`

---

## Make Targets

| Target | What it does |
|--------|--------------|
| `make` / `make all` | Build everything |
| `make vw`, `make viola`, `make libs` | Build only that part |
| `make test` | Build and run the unit tests (21 tests) |
| `make san` | Clean, then build with AddressSanitizer + UndefinedBehaviorSanitizer |
| `make asan`, `make ubsan` | One sanitizer only |
| `make clean` | Remove object files, dependency files and generated `.v.h` files |
| `make distclean` | `clean` plus libraries and binaries |
| `make rebuild` | `distclean` then `all` |
| `make install` / `make uninstall` | Copy to / remove from `$(PREFIX)` (default `/usr/local`) |
| `make info` | Show compiler, flags and detected paths |
| `make check-help` | List code-analysis targets (`check-includes`, `check-libs`, `check-symbols`, `check-unused`) |
| `make help` | List targets |

Notes:

- Run `make` before `make test`. The tests reuse some of the browser's
  object files and will compile missing ones with their own flags.
- `make san` cleans first. To return to a normal build afterwards, run
  `make clean && make`.
- `make install` is not tested on Linux in this fork; running the browser
  from the build directory is the supported way.

---

## When a Clean Rebuild Is Needed

Dependencies are tracked automatically (`-MMD -MP`), so a plain `make` is
usually enough. Use `make clean && make` when:

- a change touches `src/viola/ident.h` (the table of script identifiers;
  everything depends on its numbering)
- you switch between a sanitizer build and a normal build
- page fonts or other embedded scripts don't seem to change after an update

---

## Common Build Errors

| Error | Fix |
|-------|-----|
| `Xm/Xm.h: No such file or directory` | `sudo apt install libmotif-dev` |
| `X11/Intrinsic.h: No such file or directory` | `sudo apt install libxt-dev` |
| `X11/Xlib.h: No such file or directory` | `sudo apt install libx11-dev` |
| `byacc not found` | `sudo apt install byacc` |
| `hexdump: not found` | `sudo apt install bsdextrautils` |
| `openssl/ssl.h: No such file or directory` | `sudo apt install libssl-dev` (or build without HTTPS) |
| `unicode/utrans.h: No such file or directory` | `sudo apt install libicu-dev` |
| `cannot find -lXmu` (or another `-lX…`) | Install the matching `libx…-dev` package, e.g. `libxmu-dev` |
| `make san` uses the wrong compiler | Override with `make san SAN_CC=clang` |

---

## Files Reference

| File | Purpose |
|------|---------|
| `Makefile` | Main build |
| `test/Makefile` | Unit test build (platform-aware) |
| `src/viola/gram.y` | Viola script grammar (byacc input) |
| `src/viola/embeds/*.v` | Viola scripts compiled into the browser |
| `src/viola/objs.c` | Built-in object definitions; includes the embedded scripts |

---

## See Also

- [DEBIAN.md](DEBIAN.md) — Installing, building and running on Debian
- [EXTERNAL_DEPENDENCIES.md](EXTERNAL_DEPENDENCIES.md) — External programs and environment variables
- [Main README](../README.md)
