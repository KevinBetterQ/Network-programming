#include <sys/types.h>
#include <sys/socket.h>						/*包含socket()/bind()*/
#include <netinet/in.h>						/*包含struct sockaddr_in*/
#include <string.h>							/*包含memset()*/
#define PORT_SERV 8888/*服务器端口*/
#define BUFF_LEN 256									/*缓冲区大小*/
void static udpserv_echo(int s, struct sockaddr*client)
{
	int n;												/*接收数据长度*/
	char buff[BUFF_LEN];								/*接收发送缓冲区															*/
	socklen_t len;											/*地址长度*/
	while(1)											/*循环等待*/
	{
		len = sizeof(*client);
		n = recvfrom(s, buff, BUFF_LEN, 0, client, &len);
								/*接收数据放到buff中，并获得客户端地址*/
		sendto(s, buff, n, 0, client, len);/*将接收到的n个字节发送回客户												端*/
	}	
}

int main(int argc, char*argv[])
{
	int s;									/*套接字文件描述符*/
	struct sockaddr_in addr_serv,addr_clie;		/*地址结构*/
	
	s = socket(AF_INET, SOCK_DGRAM, 0);			/*建立数据报套接字*/
	
	memset(&addr_serv, 0, sizeof(addr_serv));		/*清空地址结构*/
	addr_serv.sin_family = AF_INET;				/*地址类型为AF_INET*/
	addr_serv.sin_addr.s_addr = htonl(INADDR_ANY);	/*任意本地地址*/
	addr_serv.sin_port = htons(PORT_SERV);			/*服务器端口*/
	
	bind(s, (struct sockaddr*)&addr_serv, sizeof(addr_serv));
													/*绑定地址*/
	udpserv_echo(s, (struct sockaddr*)&addr_clie);	/*回显处理程序*/
	
	return 0;	
}
