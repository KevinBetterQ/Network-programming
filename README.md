# Network-programming
 Socket网络程序设计，主要参考
《Internetworking With TCP/IP Vol Three : Client-Server Programming And Applications (Linux/POSIX Sockets Version)》实现。

学习过程中，C语言实现了一些网络通信的服务器/客户端模型，希望对其它学习TCP/UDP，Socket，网络文件传输或网络程序设计的人有所帮助，有需要可以拿来跑跑试试。

在 CSDN 上开了个专栏，可以参考看看：[【Socket网络程序设计】](https://blog.csdn.net/KevinBetterQ/column/info/16801)

## 实现内容
1. [自己动手实现一个基于TCP协议的客户-服务器通信实例](https://blog.csdn.net/KevinBetterQ/article/details/74158091)
2. [自己动手实现一个基于UDP协议的客户-服务器通信实例](https://blog.csdn.net/KevinBetterQ/article/details/74170821)
3. [利用TCP（循环面向连接的模型）完成文件的传输](https://blog.csdn.net/KevinBetterQ/article/details/77033260)
4. [利用tcp完成linux和windows平台的文件传输](https://blog.csdn.net/KevinBetterQ/article/details/77036547)
5. [多进程和多线程实现服务器端的并发处理](https://blog.csdn.net/KevinBetterQ/article/details/77045187)
6. [利用I/O复用完成单进程并发服务器的处理](https://blog.csdn.net/KevinBetterQ/article/details/77071016)
7. [支持多协议多服务的服务器设计和实现](https://blog.csdn.net/KevinBetterQ/article/details/77072829)

## 代码结构
- `UDP-Practice/` 实现 UDP 协议的客户端和服务器
- `TCP-Practice/` 实现 UDP 协议的客户端和服务器
- `TCP_file/` 利用 TCP 实现文件传输
- `Windows-Linux/` 实现Windows和Linux的文件传输
- `mulTCP/` 在 TCP 文件传输代码的基础上，利用多进程和多线程实现服务器端的并发处理
- `select/` 在ECHO代码的基础上，利用I/O复用完成单进程并发服务器的处理
- `All/` 融合之前实现的各种协议与服务的代码，实现一个支持多协议多服务的服务器

## Authors
qiwenkaikai@qq.com
