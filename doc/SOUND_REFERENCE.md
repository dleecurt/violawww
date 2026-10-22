# Sound Reference

> **ViolaWWW 4.0 Additions:**  
> - `bell()` rings the X11 bell (`XBell()`) on Linux
> - Audio file playback (see the Linux note under Supported Audio Formats)

This document describes sound capabilities in ViolaWWW.

## Contents

- [Viola Script Functions](#viola-script-functions)
- [Playing Audio Files in HTML](#playing-audio-files-in-html)
- [Supported Audio Formats](#supported-audio-formats)

---

## Viola Script Functions

### bell()

Plays a system alert sound.

**Returns:** (int) 1

**Example:**
```c
bell();
```

**Linux:** Rings the X11 bell with `XBell()`. Whether you hear anything
depends on your desktop's bell setting (`xset b on` enables it).

---

### bellVolume([percent])

Gets or sets the bell volume.

**Parameters:**
- `percent` (int, optional) - volume from 0 to 100

**Returns:** (int) current volume

**Example:**
```c
bellVolume(50);   /* Set volume to 50% */
bell();           /* Play at 50% volume */

vol = bellVolume();  /* Get current volume */
```

---

## Playing Audio Files in HTML

### Method 1: Hyperlink

Create a hyperlink to an audio file. When clicked, the browser downloads and plays the file:

```html
<A HREF="sound.au">Play sound</A>
```

### Method 2: Auto-play with INSERT

Use the `<INSERT>` tag to automatically play a sound when the page loads:

```html
<INSERT HREF="sound.au"></INSERT>
```

### Method 3: Looping with AFTER

Add the `AFTER` attribute to repeat playback at a specified interval (in milliseconds):

```html
<INSERT HREF="sound.au" AFTER="1000"></INSERT>
```

This plays `sound.au` immediately, then repeats every 1000ms (1 second).

**Note:** The `AFTER` interval is measured from the start of each playback, not from when the previous playback ends.

---

## Supported Audio Formats

**Linux status:** audio file playback does not work on Linux yet. The
player command is hard-coded to macOS's `/usr/bin/afplay`
(`src/viola/embeds/wwwHandlerDispatch_script.v`), which Debian does not
have. A Linux player such as `aplay` (package `alsa-utils`, WAV/AU) or
`paplay` (package `pulseaudio-utils`) could take its place.

---

## Technical Notes

1. **Bell:** `bell()` uses the X11 bell (`XBell()`).

2. **File Playback:** Audio files are first downloaded via HTTP to a temporary file, then handed to an external player in the background (currently `afplay`, which is not available on Linux; see above).

3. **Limitations:**
   - `sleep()` and `delay()` only accept whole seconds, making precise rhythm timing difficult in Viola scripts
   - The `AFTER` attribute uses milliseconds for more precise timing
   - Audio playback is asynchronous - the script continues while sound plays

