/*		Wayback Machine (Internet Archive) Integration		HTWayback.c
**		==============================================
**
**	This module provides integration with the Internet Archive's
**	Wayback Machine for accessing archived versions of websites.
**
**	It used to ask the CDX search API for the newest capture first, but
**	that search routinely took 20+ seconds.  The Wayback Machine already
**	does the same job itself: a request for
**	    https://web.archive.org/web/<timestamp>id_/<url>
**	is redirected to the capture closest to <timestamp>.  Using the current
**	time picks the newest capture, and the normal HTTPS loader follows the
**	redirect.  If there is no capture the Archive answers 404 and the load
**	fails as before.  ("id_" asks for the page as originally served,
**	without the Archive's toolbar and rewritten links.)
*/

#include "HTWayback.h"
#include "HTUtils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* External UI function - defined in viola code */
extern void showHelpMessageInMainWindow(char* message) __attribute__((weak));

/* Helper to safely call UI update function (same way as html2.c does) */
PRIVATE void safe_show_message(const char* message) {
#ifdef VIOLA
    extern void* mesgObj;  /* VObj* but we don't want to include viola.h */
    if (!mesgObj) {
        /* Try to find message object */
        extern void* findObject(int);
        extern int getIdent(const char*);
        mesgObj = findObject(getIdent("www.mesg.tf"));
    }
    if (mesgObj) {
        extern int sendMessage1N1str(void*, const char*, char*);
        sendMessage1N1str(mesgObj, "show", (char*)message);
    }
#else
    /* Fallback for non-Viola builds */
    if (showHelpMessageInMainWindow) {
        showHelpMessageInMainWindow((char*)message);
    }
#endif
}

/*	Build the Wayback URL for the newest capture of a URL
**	------------------------------------------------------
*/
PUBLIC char* HTWaybackCheck PARAMS((const char* url)) {
    static const char prefix[] = "https://" WAYBACK_API_HOST "/web/";
    char stamp[16];
    char* wayback_url;
    char* dst;
    const char* src;
    time_t now = time(NULL);
    struct tm tm_utc;

    if (!url || !*url)
        return NULL;

    /* Skip if URL is already a Wayback Machine URL */
    if (strstr(url, WAYBACK_API_HOST "/") != NULL) {
        if (TRACE) fprintf(stderr, "Wayback: URL is already from Wayback Machine, skipping\n");
        return NULL;
    }

    if (!gmtime_r(&now, &tm_utc) ||
        strftime(stamp, sizeof(stamp), "%Y%m%d%H%M%S", &tm_utc) != 14)
        return NULL;

    /* prefix + stamp + "id_/" + url with spaces as %20 (3 bytes each) */
    wayback_url = (char*)malloc(sizeof(prefix) + 14 + 4 + strlen(url) * 3 + 1);
    if (!wayback_url)
        return NULL;
    dst = wayback_url + sprintf(wayback_url, "%s%sid_/", prefix, stamp);
    for (src = url; *src && *src != '#'; src++) { /* the #fragment is local */
        if (*src == ' ') {
            memcpy(dst, "%20", 3);
            dst += 3;
        } else {
            *dst++ = *src;
        }
    }
    *dst = '\0';

    if (TRACE) fprintf(stderr, "Wayback: Trying newest capture via %s\n", wayback_url);
    safe_show_message("Loading from Web Archive...");
    return wayback_url;
}
