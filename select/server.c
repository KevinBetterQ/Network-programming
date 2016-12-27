/* TCPmechod.c - main, echo */

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>


#define PORT 6000
#define LISTENQ 20
#define BUFFSIZE 4096
#define FILE_NAME_MAX_SIZE 512




int		echo(int sc){
	//process_conn_server(sc);		/*处理连接*/
			ssize_t size = 0;
			char buffer[1024];							/*数据的缓冲区*/	
				size = read(sc, buffer, 1024);			/*从套接字中读取数据放到													缓冲区buffer中*/
				write(sc, buffer, strlen(buffer)+1);/*发给客户端*/
			return size;
	
			
}

int passiveTCP (const char*service){
	//Create socket
	int sockfd,connfd;
	struct sockaddr_in svraddr,clientaddr;
	bzero(&svraddr,sizeof(svraddr));
	
	svraddr.sin_family=AF_INET;
	svraddr.sin_addr.s_addr=htonl(INADDR_ANY);
	svraddr.sin_port=htons(PORT);

	sockfd=socket(AF_INET,SOCK_STREAM,0);
	if(sockfd<0)
	{
		perror("socket");
		exit(1);
	}

	//bind	
	if(bind(sockfd,(struct sockaddr*)&svraddr,sizeof(svraddr))<0)
	{
		perror("bind");
		exit(1);
	}

	//listen
	if(listen(sockfd,LISTENQ)<0)
	{
		perror("listen");
		exit(1);
	}
	return sockfd;
}



int main(int argc, char *argv[])
{         
	char	*service = "echo";/* service name or port number*/
	struct sockaddr_in fsin;	/* the from address of a client	*/
	int	msock;	    /* master server socket		*/
	fd_set	rfds;			/* read file descriptor set	*/
	fd_set	afds;			/* active file descriptor set	*/
	unsigned int	alen;	   /* from-address length	*/
	int	fd, nfds;
    
	msock = passiveTCP(service);
	nfds = getdtablesize();
	FD_ZERO(&afds);
	FD_SET(msock, &afds);
    while (1) {
      memcpy(&rfds, &afds, sizeof(rfds));
      if (select(nfds, &rfds, (fd_set *)0, (fd_set *)0,(struct timeval *)0) < 0)
		//errexit("select: %s\n", strerror(errno));
		printf("select error");
	if (FD_ISSET(msock, &rfds)) {
		int	ssock;
		alen = sizeof(fsin);
		ssock = accept(msock, (struct sockaddr *)&fsin,&alen);
		if (ssock < 0)
		//errexit("accept: %s\n",strerror(errno));
		printf("accept error");
		
		FD_SET(ssock, &afds);
		}
	for (fd=0; fd<nfds; ++fd)
		if (fd != msock && FD_ISSET(fd, &rfds))
			if (echo(fd) == 0) {
				close(fd);
				FD_CLR(fd, &afds);
			}
}
}
