/* superd.c - main */

#define	_USE_BSD
#include <sys/types.h>
#include <sys/param.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/errno.h>
#include <sys/signal.h>
#include <sys/wait.h>
#include <netinet/in.h>

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#define	UDP_SERV	0
#define	TCP_SERV	1

#define	NOSOCK		-1	/* an invalid socket descriptor	*/

struct service {
	char	*sv_name;
	char	sv_useTCP;
	int	sv_sock;
	void	(*sv_func)(int);
};

void	TCPechod(int), TCPchargend(int), TCPdaytimed(int), TCPtimed(int);

int	passiveTCP(const char *service, int qlen);
int	passiveUDP(const char *service);
int	errexit(const char *format, ...);
void	doTCP(struct service *psv);
void	reaper(int sig);

struct service svent[] =
	{	{ "echo", TCP_SERV, NOSOCK, TCPechod },
		{ "chargen", TCP_SERV, NOSOCK, TCPchargend },
		{ "daytime", TCP_SERV, NOSOCK, TCPdaytimed },
		{ "time", TCP_SERV, NOSOCK, TCPtimed },
		{ 0, 0, 0, 0 },
	};

#ifndef	MAX
#define	MAX(x, y)	((x) > (y) ? (x) : (y))
#endif	/* MAX */

#define	QLEN		 32

#define	LINELEN		128

extern	unsigned short	portbase;	/* from passivesock()	*/

/*------------------------------------------------------------------------
 * main - Super-server main program
 *------------------------------------------------------------------------
 */
int
main(int argc, char *argv[])
{
	struct service	*psv,		/* service table pointer	*/
		*fd2sv[NOFILE];		/* map fd to service pointer	*/
	int	fd, nfds;
	fd_set	afds, rfds;		/* readable file descriptors	*/

	switch (argc) {
	case 1:
		break;
	case 2:
		portbase = (unsigned short) atoi(argv[1]);
		break;
	default:
		errexit("usage: superd [portbase]\n");
	}

	nfds = 0;
	FD_ZERO(&afds);
	for (psv = &svent[0]; psv->sv_name; ++psv) {
		if (psv->sv_useTCP)
			psv->sv_sock = passiveTCP(psv->sv_name, QLEN);
		else
			psv->sv_sock = passiveUDP(psv->sv_name);
		fd2sv[psv->sv_sock] = psv;
		nfds = MAX(psv->sv_sock+1, nfds);
		FD_SET(psv->sv_sock, &afds);
	}
	(void) signal(SIGCHLD, reaper);

	while (1) {
		memcpy(&rfds, &afds, sizeof(rfds));
		if (select(nfds, &rfds, (fd_set *)0, (fd_set *)0,
				(struct timeval *)0) < 0) {
			if (errno == EINTR)
				continue;
			errexit("select error: %s\n", strerror(errno));
		}
		for (fd=0; fd<nfds; ++fd)
			if (FD_ISSET(fd, &rfds)) {
				psv = fd2sv[fd];
				if (psv->sv_useTCP)
					doTCP(psv);
				else
					psv->sv_func(psv->sv_sock);
			}
	}
}

/*------------------------------------------------------------------------
 * doTCP - handle a TCP service connection request
 *------------------------------------------------------------------------
 */
void
doTCP(struct service *psv)
{
	struct sockaddr_in fsin;	/* the request from address	*/
	unsigned int	alen;		/* from-address length		*/
	int	fd, ssock;

	alen = sizeof(fsin);
	ssock = accept(psv->sv_sock, (struct sockaddr *)&fsin, &alen);
	if (ssock < 0)
		errexit("accept: %s\n", strerror(errno));
	switch (fork()) {
	case 0:	
		break;
	case -1:
		errexit("fork: %s\n", strerror(errno));
	default:
		(void) close(ssock);
		return;		/* parent */
	}
	/* child */

	for (fd = NOFILE; fd >= 0; --fd)
		if (fd != ssock)
			(void) close(fd);
	psv->sv_func(ssock);
	exit(0);
}

/*------------------------------------------------------------------------
 * reaper - clean up zombie children
 *------------------------------------------------------------------------
 */
void
reaper(int sig)
{
	int	status;

	while (wait3(&status, WNOHANG, (struct rusage *)0) >= 0)
		/* empty */;
}
