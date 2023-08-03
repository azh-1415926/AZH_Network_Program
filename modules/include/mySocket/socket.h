#ifndef _MY_SOCKET_H_
#define _MY_SOCKET_H_

#ifdef __cplusplus
extern "C" {
#endif

// standard headers
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

// headers for windows
#if _WIN32

#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib,"ws2_32.lib")

typedef SOCKET SOCKET_FD;

bool initalizeSocket();

// headers for linux
#elif __linux__

#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

typedef int SOCKET_FD;

#endif

// general function

// convert the server_ip,serverport to inital struct sockaddr_in
struct sockaddr_in* setSockaddr(struct sockaddr_in* server_addr,int af,const char* server_ip,int server_port);
// create socket by af,type,protocol
SOCKET_FD Socket(int af,int type,int protocol);
// connect to the server
bool Connect(SOCKET_FD s,struct sockaddr* server_addr,socklen_t server_len);
// set the send and receive time
void setTimeout(SOCKET_FD s,int time);
// close socket
void closeSocket(SOCKET_FD s);

#ifdef __cplusplus
}
#endif

#endif