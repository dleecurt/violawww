/*                               /Net/dxcern/userd/timbl/hypertext/WWW/Library/src/HTTCP.html
                               GENERIC TCP/IP COMMUNICATION

   This module has the common code for handling TCP/IP connections etc.

 */
#ifndef HTTCP_H
#define HTTCP_H

#include "HTUtils.h"

#include "tcp.h"

#ifdef SHORT_NAMES
#define HTInetStatus HTInStat
#define HTInetString HTInStri
#define HTParseInet HTPaInet
#endif

/*      Produce a string for an internet address
**      ---------------------------------------
**
** On exit:
**           returns a pointer to a static string which must be copied if
**                it is to be kept.
*/
#ifdef __STDC__
extern const char* HTInetString(struct sockaddr_in* sin);
#else
extern char* HTInetString();
#endif

/*      Encode INET status (as in sys/errno.h)                    inet_status()
**      ------------------
**
** On entry:
**              where gives a description of what caused the error
**      global errno gives the error number in the unix way.
**
** On return:
**      returns a negative status in the unix way.
*/
#ifdef __STDC__
extern int HTInetStatus(char* where);
#else
extern int HTInetStatus();
#endif

/*      Publicly accessible variables
 */
/* extern struct sockaddr_in HTHostAddress; */
/* The internet address of the host */
/* Valid after call to HTHostName() */

/*      Parse a cardinal value                                 parse_cardinal()
**      ----------------------
**
** On entry:
**      *pp points to first character to be interpreted, terminated by
**      non 0..9 character.
**      *pstatus points to status already valid,
**      maxvalue gives the largest allowable value.
**
** On exit:
**      *pp points to first unread character,
**      *pstatus points to status updated iff bad
*/

extern unsigned int HTCardinal PARAMS((int* pstatus, char** pp, unsigned int max_value));

/*      Parse an internet node address and port
**      ---------------------------------------
**
** On entry:
**               str points to a string with a node name or number,
**               with optional trailing colon and port number.
**               sin points to the binary internet or decnet address field.
**
** On exit:
**               *sin is filled in. If no port is specified in str, that
**               field is left unchanged in *sin.
*/
#ifdef __STDC__
extern int HTParseInet(struct sockaddr_in* sin, const char* str);
/*!! had to change this to get it to compile. CTB */
#else
extern int HTParseInet();
#endif

/*      Get Name of This Machine
**      ------------------------
**
*/

extern const char* HTHostName NOPARAMS;

/*	Keeping the user interface alive during network waits
**	----------------------------------------------------
**	The application may set HTIdle_fd to a descriptor it wants watched
**	(e.g. the X server connection) and HTIdle_hook to a function that
**	handles activity on it.  While a read waits for data, the hook is
**	called whenever HTIdle_fd is readable, and every 100 ms.  With no
**	hook set, reads block exactly as before.
*/
extern void (*HTIdle_hook)(void);
extern int HTIdle_fd;

/*	Wait until sock can be read (or written if for_write), up to the
**	socket's SO_RCVTIMEO/SO_SNDTIMEO (60 s if unset), calling the idle
**	hook meanwhile.  Returns 1 when ready (or on error, so the caller's
**	read reports it), 0 on timeout.
*/
extern int HTWaitSocket(int sock, int for_write);

/*	read() that waits with HTWaitSocket first.  Returns -1 with errno
**	EAGAIN on timeout, like a socket with SO_RCVTIMEO.
*/
extern int HTNetRead(int fd, char* buf, int len);

#endif /* HTTCP_H */
/*

   End.  */
