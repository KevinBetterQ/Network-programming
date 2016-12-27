#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#define PORT 8888						/*侦听端口地址*/
#define BACKLOG 2						/*侦听队列长度*/

int main(int argc, char *argv[])
{
	int ss,sc;		/*ss为服务器的socket描述符，sc为客户端的socket描述符*/
	struct sockaddr_in server_addr;	/*服务器地址结构*/
	struct sockaddr_in client_addr;	/*客户端地址结构*/
	int err;							/*返回值*/
	pid_t pid;	
    char words[1024];	/*分叉的进行ID*/

	/*建立一个流式套接字*/
	ss = socket(AF_INET, SOCK_STREAM, 0);
	if(ss < 0){							/*出错*/
		printf("socket error\n");
		return -1;	
	}
	
	/*设置服务器地址*/
	bzero(&server_addr, sizeof(server_addr));			/*清零*/
	server_addr.sin_family = AF_INET;					/*协议族*/
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);	/*本地地址*/
	server_addr.sin_port = htons(PORT);				/*服务器端口*/
	
	/*绑定地址结构到套接字描述符*/
	err = bind(ss, (struct sockaddr*)&server_addr, sizeof(server_addr));
	if(err < 0){/*出错*/
		printf("bind error\n");
		return -1;	
	}
	
	/*设置侦听*/
	err = listen(ss, BACKLOG);
	if(err < 0){										/*出错*/
		printf("listen error\n");
		return -1;	
	}
	
		/*主循环过程*/
	for(;;)	{
		socklen_t addrlen = sizeof(struct sockaddr);
		
		sc = accept(ss, (struct sockaddr*)&client_addr, &addrlen); 
		/*接收客户端连接*/
		if(sc < 0){							/*出错*/
			continue;						/*结束本次循环*/
		}else{
			sprintf(words, "%d login\n", sc);
			write(1, words, strlen(words)+1);
		}
		
		/*建立一个新的进程处理到来的连接*/
		pid = fork();						/*分叉进程*/
		if( pid == 0 ){						/*子进程中*/
			//process_conn_server(sc);		/*处理连接*/
			ssize_t size = 0;
			char buffer[1024];							/*数据的缓冲区*/
			for(;;){									/*循环处理过程*/		
				size = read(sc, buffer, 1024);			/*从套接字中读取数据放到													缓冲区buffer中*/
				if(size == 0){							/*没有数据*/
					return;	
				}
				write(1, buffer, size);					/*写到标准输出*/
				sprintf(buffer, "From %d\n", sc);
				write(1, buffer, strlen(buffer)+1);					/*写到标准输出*/
		
		
				/*构建响应字符，为接收到客户端字节的数量*/
				sprintf(buffer, "I receive %d bytes altogether\n", size);
				write(sc, buffer, strlen(buffer)+1);/*发给客户端*/
			}	
			
			close(ss);						/*在子进程中关闭服务器的侦听*/
		}else{
			//close(sc);						/*在父进程中关闭客户端的连接*/
		}
	}
}



