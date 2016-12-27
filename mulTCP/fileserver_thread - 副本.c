#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>

#define PORT 6000
#define LISTENQ 20
#define BUFFSIZE 4096
#define FILE_NAME_MAX_SIZE 512

int connfd;

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


// 线程函数
//线程函数可以完成更复杂的工作，这仅仅是一个示例
void *my_thread()
{
	        printf("222222222 \n");
			char filename[FILE_NAME_MAX_SIZE];
			printf("333333 \n");
			printf("\n %d request\n",connfd);
			//Input the file name
			bzero(filename,FILE_NAME_MAX_SIZE);
			printf("Please input the file name you wana to send:");
			scanf("%s",&filename);
			getchar();
			
	
			//send filename imformation
			char buff[BUFFSIZE];
			int count;
			bzero(buff,BUFFSIZE);
			strncpy(buff,filename,strlen(filename)>FILE_NAME_MAX_SIZE?FILE_NAME_MAX_SIZE:strlen(filename));
			count=send(connfd,buff,BUFFSIZE,0);
			if(count<0)
			{
				perror("Send file information");
				exit(1);
			}
		
			//read file 
			FILE *fd=fopen(filename,"rb");
			if(fd==NULL)
			{
				printf("File :%s not found!\n",filename);
			}
			else 
			{
				bzero(buff,BUFFSIZE);
				int file_block_length=0;
				while((file_block_length=fread(buff,sizeof(char),BUFFSIZE,fd))>0)
				{
					printf("file_block_length:%d\n",file_block_length);
					if(send(connfd,buff,file_block_length,0)<0)
					{
					perror("Send");
					exit(1);
				}
				bzero(buff,BUFFSIZE);	
			}
				fclose(fd);
				printf("Transfer file finished !\n");
			}	
			close(connfd);//关闭客户端的连接
	        pthread_exit(NULL);

}

void thread_create()
{
	pthread_t thread;
	printf("111111111\n");
	if(pthread_create(&thread, NULL, my_thread, NULL) != 0) {    //创建线程
		printf("Creating thread has failed!\n");
	}
		
}



int main(int argc, char **argv[])
{
	char filename[FILE_NAME_MAX_SIZE];
	int sockfd;
	struct sockaddr_in clientaddr;
	pid_t pid;
	sockfd = passiveTCP(PORT);
	

	while(1)
	{
		socklen_t length=sizeof(clientaddr);

		//accept
		connfd=accept(sockfd,(struct sockaddr*)&clientaddr,&length);
		if(connfd<0)
		{
			perror("connect");
			exit(1);
		}
		
		/*建立一个新的线程处理到来的连接*/
		thread_create();
	}
	close(sockfd);
	return 0;
}






