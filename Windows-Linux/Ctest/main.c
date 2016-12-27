#include <stdlib.h>
#include <winsock2.h>
#include <stdio.h>
//#pragma comment(lib,"ws2_32.lib")	//把ws2_32.lib加到Link页的连接库
//#define IP "172.18.68.243"			//在两台计算机上测试，IP为Server端的IP地址
#define IP "192.168.248.131"				//在一台计算机上测试，IP为本地回送地址
#define PORT 6000					//注意：客户端设置通信的端口 = 服务端的端口
#define BUFFER_SIZE 1024			//数据发送缓冲区大小

#define LISTENQ 20
#define BUFFSIZE 4096
#define FILE_NAME_MAX_SIZE 512


void recvTCP(int clientfd){
	//recv file imformation
	char buff[BUFFSIZE];
	char filename[FILE_NAME_MAX_SIZE];
	int count;
	//bzero(buff,BUFFSIZE);
	memset(buff,0,BUFFSIZE);

	count=recv(clientfd,buff,BUFFSIZE,0);
	if(count<0)
	{
		perror("recv");
		exit(1);
	}
	strncpy(filename,buff,strlen(buff)>FILE_NAME_MAX_SIZE?FILE_NAME_MAX_SIZE:strlen(buff));

	printf("Preparing recv file : %s  \n",filename);


	//recv file
	FILE *fd=fopen(filename,"wb+");
	if(NULL==fd)
	{
		perror("open");
		exit(1);
	}
	//bzero(buff,BUFFSIZE);
	memset(buff,0,BUFFSIZE);

	int length=0;
	while(length=recv(clientfd,buff,BUFFSIZE,0))
	{
		if(length<0)
		{
			perror("recv");
			exit(1);
		}
		int writelen=fwrite(buff,sizeof(char),length,fd);
		if(writelen<length)
		{
			perror("write");
			exit(1);
		}
		//bzero(buff,BUFFSIZE);
		memset(buff,0,BUFFSIZE);
	}
	printf("Receieved file:%s  finished!\n",filename);
	fclose(fd);
}


int connectTCP(const char *host, const char *port){
       WSADATA WSAData;
		if(WSAStartup(MAKEWORD(2,0),&WSAData)==SOCKET_ERROR)  //WSAStartup()函数对Winsock DLL进行初始化
		{
			printf("Socket initialize fail!\n");
			//continue;
		}
		SOCKET sock;											//客户端进程创建套接字
		if((sock=socket(AF_INET,SOCK_STREAM,0))==SOCKET_ERROR)  //创建流套接字（与服务端保持一致）
		{
			printf("Socket create fail!\n");
			WSACleanup();
			//continue;
		}

		struct sockaddr_in ClientAddr;				//sockaddr_in结构用来标识TCP/IP协议下的地址，可强制转换为sockaddr结构
		ClientAddr.sin_family=AF_INET;				//指Internet域
		ClientAddr.sin_port=htons(PORT);			//指定服务端所预留的端口
		ClientAddr.sin_addr.s_addr=inet_addr(IP);	//指定服务端所绑定的IP地址
		if(connect(sock,(LPSOCKADDR)&ClientAddr,sizeof(ClientAddr))==SOCKET_ERROR)  //调用connect()函数，向服务器进程发出连接请求
		{
			printf("Connect fail!\n");
			closesocket(sock);
			WSACleanup();
			//continue;
		}
		return sock;
}


int main()
{
    SOCKET sock;											//客户端进程创建套接字
	char buf[BUFFER_SIZE];								//buf数组存放客户端发送的消息
	int inputLen;										//用于输入字符自增变量
	while(1)
	{
		printf("Socket\\Client>");
		inputLen=0;
		memset(buf,0,sizeof(buf));
		while((buf[inputLen++]=getchar())!='\n')		//输入以回车键为结束标识
		{
			;
		}
		if(buf[0]=='e' && buf[1]=='x' && buf[2]=='i' && buf[3]=='t')
		{
			printf("The End.\n");
			break;
		}

        sock=connectTCP(IP,PORT);

		//send(sock,buf,BUFFER_SIZE,0);				 //向服务器发送数据

		recvTCP(sock);



		closesocket(sock);							 //关闭套接字
		WSACleanup();								//终止对Winsock DLL的使用，并释放资源，以备下一次使用
	}
	return 0;
}

