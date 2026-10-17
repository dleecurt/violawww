/*		Wayback Machine (Internet Archive) Integration		HTWayback.h
**		==============================================
**
**	This module provides integration with the Internet Archive's
**	Wayback Machine for accessing archived versions of websites.
*/

#ifndef HTWAYBACK_H
#define HTWAYBACK_H

#include "HTUtils.h"

/*	Wayback Machine API configuration
*/
#define WAYBACK_API_HOST "web.archive.org"

/*	Wayback Machine URL for the newest capture of a URL
**	----------------------------------------------------
**
**  On entry:
**	url		The original URL (e.g., "http://example.com/page")
**
**  On exit:
**	returns		https://web.archive.org/web/<now>id_/<url>, which the
**			Archive redirects to its newest capture (or answers 404
**			if it has none); must be freed by caller.
**			NULL if url is empty, already an Archive URL, or on error.
**	No network access happens here.
*/
PUBLIC char* HTWaybackCheck PARAMS((const char* url));

#endif /* HTWAYBACK_H */

