# External Dependencies Reference

This document lists the external programs and environment variables
ViolaWWW uses on Linux, with the Debian package that provides each program.

All of these are optional: the browser itself only needs X11 and, for `vw`,
Motif. For building, see [BUILD_PROCESS.md](BUILD_PROCESS.md) and
[DEBIAN.md](DEBIAN.md).

---

## External Programs

### Links and Network Protocols

| Program | Debian package | Purpose | How it is started |
|---------|----------------|---------|-------------------|
| `xdg-open` | `xdg-utils` | Opens `mailto:` links in your desktop's mail program | `openExternal()` in `cl_generic.c`, without a shell |
| `telnet` | `inetutils-telnet` | `telnet://host[:port]` and `telnet://user@host` links | `openExternal()`, in a new terminal window |
| `x-terminal-emulator` or `xterm` | your desktop's terminal, or `xterm` | Terminal window for `telnet://` links | `openExternal()` |
| `c3270` | `c3270` (Debian contrib section) | `tn3270://host` links (IBM mainframes) | `HTTelnet.c`, with `system()`, in the terminal ViolaWWW was started from |
| `rlogin` | none (no longer packaged) | `rlogin://` links | `HTTelnet.c`, with `system()` |

`mailto:` and `telnet:` links are handed to these programs as separate
arguments, never through a shell, and telnet host, port and user names must
be plain characters. If a program is missing, the status line says which
package to install.

### Documents and Images

| Program | Debian package | Purpose | Search order |
|---------|----------------|---------|--------------|
| `onsgmls` | `opensp` | Parse HMML documents (via `sgmlsA2B`) | The program's own directory, then `/usr/local/bin`, `/usr/bin` and a few other standard locations |
| `gs` (Ghostscript) | `ghostscript` | Display PostScript | Same as `onsgmls`, then `PATH` |
| `magick` (ImageMagick) | `imagemagick` | Convert PostScript to GIF for inline display | `PATH` |

### Mail and Printing

| Program | Debian package | Purpose | Path |
|---------|----------------|---------|------|
| `mail` | `mailutils` (or `bsd-mailx`) | "Send comments" window (File menu) | `/usr/bin/mail` (fixed) |
| `lpr` | `cups-bsd` | Printing documents | `/usr/bin/lpr` (fixed; override with `WWW_LPR`) |

Sending mail with `mail` also needs a working local mail setup (for example
`exim4` or `msmtp-mta`); without one, messages are not delivered.

### Audio

Audio file playback does **not** work on Linux yet: the player is hard-coded
to macOS's `/usr/bin/afplay` in
`src/viola/embeds/wwwHandlerDispatch_script.v`. The script `bell()` function
works; it uses the X11 bell. See [SOUND_REFERENCE.md](SOUND_REFERENCE.md).

### Image Editors and Viewers (Optional)

| Program | Debian package | Purpose |
|---------|----------------|---------|
| `bitmap` | `x11-apps` | Edit XBM images |
| `pixmap` | not packaged | Edit XPM images |
| `xv` | not packaged | External image viewer |

### System Utilities

Viola scripts also call standard tools that every Debian system has (`rm`,
`mv`), plus `finger` (package `finger`) for one legacy feature.

---

## Environment Variables

### Core Configuration

| Variable | Purpose | Default |
|----------|---------|---------|
| `VIOLA_PATH` | Directory with Viola's application files | Detected automatically when run from the build directory |
| `VIOLA_SGMLSA2B` | Path to the `sgmlsA2B` HMML converter | `sgmlsA2B` in `PATH` |
| `WWW_HOME` | Home page loaded at startup | `http://info.cern.ch/hypertext/WWW/TheProject.html` |
| `WWW_LPR` | Print command | `/usr/bin/lpr` |
| `HOME` | Location of `~/.viola_history` and other per-user files | System default |
| `TMPDIR` | Directory for temporary files | `/tmp` |

### Display

| Variable | Purpose | Default |
|----------|---------|---------|
| `DISPLAY` | X server to connect to | Set by your desktop session (also works under Xwayland) |

### Network Protocols

| Variable | Purpose |
|----------|---------|
| `NNTPSERVER` | News server for `news:` and `nntp:` URLs |
| `USER` | User name sent as the anonymous FTP password |

### Ghostscript

| Variable | Purpose |
|----------|---------|
| `GS_LIB` | Ghostscript library and resource paths |
| `GS_FONTPATH` | Additional Ghostscript font paths |

### VPLOT

| Variable | Purpose | Default |
|----------|---------|---------|
| `VPLOT_SCALE` | Scale factor for 3D rendering, e.g. `2.0` on HiDPI screens | From `Xft.dpi`, else `1.0` |

### System

| Variable | Purpose |
|----------|---------|
| `PATH` | Search path for external programs |
| `PRINTER` | Default printer for `lpr` |

### Debugging (Development)

| Variable | Purpose |
|----------|---------|
| `ASAN_OPTIONS` | Options for `make san` builds, e.g. `detect_leaks=0` or `halt_on_error=1` |
| `UBSAN_OPTIONS` | Options for UndefinedBehaviorSanitizer, e.g. `print_stacktrace=1` |
| `YYDEBUG` | Script parser debugging output |

---

## Installing Everything

```bash
# Links
sudo apt install xdg-utils inetutils-telnet xterm

# Documents and images
sudo apt install opensp ghostscript imagemagick

# Mail and printing (optional)
sudo apt install mailutils cups-bsd
```

---

## See Also

- [DEBIAN.md](DEBIAN.md) — Installing, building and running on Debian
- [PROTOCOLS_REFERENCE.md](PROTOCOLS_REFERENCE.md) — Supported URL schemes
- [HMML_REFERENCE.md](HMML_REFERENCE.md) — HMML processing pipeline
- [SOUND_REFERENCE.md](SOUND_REFERENCE.md) — Audio
- [VPLOT_REFERENCE.md](VPLOT_REFERENCE.md) — 3D visualization
- [TTY_REFERENCE.md](TTY_REFERENCE.md) — Running external programs from Viola scripts
