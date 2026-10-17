
	switch (arg[0]) {
	case "doTelnet":
		/* arg[1]	normalized URL
		 * openExternal() starts a terminal running telnet without
		 * going through a shell (see cl_generic.c).
		 */
		return openExternal(arg[1]);
	break;
	case "doMailTo":
		/* arg[1]	normalized URL
		 * openExternal() hands the mailto: URL to the desktop's mail
		 * program (xdg-open) without going through a shell.
		 */
		return openExternal(arg[1]);
	break;
	}
	usual();
