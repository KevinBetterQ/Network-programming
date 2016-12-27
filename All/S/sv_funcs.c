/* sv_funcs.c - TCPechod, TCPchargend, TCPdaytimed, TCPtimed */

#include <sys/types.h>

#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <errno.h>


#include <sys/socket.h>
#include <netinet/in.h>



#define	BUFFERSIZE	4096		/* max read buffer size	*/

void	TCPechod(int), TCPchargend(int), TCPdaytimed(int), TCPtimed(int), UDPechod(int), UDPtimed(int);
int	errexit(const char *format, ...);

/*------------------------------------------------------------------------
 * TCPecho - do TCP ECHO on the given socket
 *------------------------------------------------------------------------
 */
void
TCPechod(int fd)
{
	char	buf[BUFFERSIZE];
	int	cc;

	while (cc = read(fd, buf, sizeof buf)) {
		if (cc < 0)
			errexit("echo read: %s\n", strerror(errno));
		if (write(fd, buf, cc) < 0)
			errexit("echo write: %s\n", strerror(errno));
	}
}

void
UDPechod(int sock)
{
	struct sockaddr_in fsin;	/* the from address of a client	*/
	char	buf[128];			/* "input" buffer; any size > 0	*/
	time_t	now;			/* current time			*/
	unsigned int	alen;		/* from-address length		*/
	int n;												/*接收数据长度*/
	    alen = sizeof(fsin);
		n = recvfrom(sock, buf, sizeof(buf), 0,
				(struct sockaddr *)&fsin, &alen);	
		(void) sendto(sock, buf, n, 0,
			(struct sockaddr *)&fsin, sizeof(fsin));
			
}


#define	LINELEN		72

/*------------------------------------------------------------------------
 * TCPchargend - do TCP CHARGEN on the given socket
 *------------------------------------------------------------------------
 */
void
TCPchargend(int fd)
{
	char	c, buf[LINELEN+2];	/* print LINELEN chars + \r\n */

	c = ' ';
	buf[LINELEN] = '\r';
	buf[LINELEN+1] = '\n';
	while (1) {
		int	i;

		for (i=0; i<LINELEN; ++i) {
			buf[i] = c++;
			if (c > '~')
				c = ' ';
		}
		if (write(fd, buf, LINELEN+2) < 0)
			break;
	}
}

/*------------------------------------------------------------------------
 * TCPdaytimed - do TCP DAYTIME protocol
 *------------------------------------------------------------------------
 */
void
TCPdaytimed(int fd)
{
	char	buf[LINELEN], *ctime();
	time_t	now;

	(void) time(&now);
	sprintf(buf, "%s", ctime(&now));
	(void) write(fd, buf, strlen(buf));
}

#define	UNIXEPOCH	2208988800UL	/* UNIX epoch, in UCT secs	*/

/*------------------------------------------------------------------------
 * TCPtimed - do TCP TIME protocol
 *------------------------------------------------------------------------
 */
void
TCPtimed(int fd)
{
	time_t	now;

	(void) time(&now);
	now = htonl((unsigned long)(now + UNIXEPOCH));
	(void) write(fd, (char *)&now, sizeof(now));
}

void
UDPtimed(int sock)
{
	struct sockaddr_in fsin;	/* the from address of a client	*/
	char	buf[1];			/* "input" buffer; any size > 0	*/
	time_t	now;			/* current time			*/
	unsigned int	alen;		/* from-address length		*/
	
	    alen = sizeof(fsin);
		if (recvfrom(sock, buf, sizeof(buf), 0,
				(struct sockaddr *)&fsin, &alen) < 0)
			errexit("recvfrom: %s\n", strerror(errno));
		(void) time(&now);
		now = htonl((unsigned long)(now + UNIXEPOCH));
		(void) sendto(sock, (char *)&now, sizeof(now), 0,
			(struct sockaddr *)&fsin, sizeof(fsin));
}






