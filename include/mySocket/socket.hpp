#ifndef SOCKET_H
#define SOCKET_H

/* 通用头文件 */
#include <iostream>
#include <cstdbool>
#include <string.h>

#if _WIN32

/* Windows 平台所需头文件 */
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib,"ws2_32.lib")

/* 用 SOCKET_FD 代替 SOCKET，统一类型名 */
typedef SOCKET SOCKET_FD;

#elif __linux__

/* Linux 平台所需头文件 */
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

/* 用 SOCKET_FD 代替 int，统一类型名 */
typedef int SOCKET_FD;

#endif

namespace azh
{
    class SocketAddr
    {
        private:
            struct sockaddr_in m_Addr;
        public:
            SocketAddr(int af, const char* ip, unsigned int port)
            {
                memset(&m_Addr,0,sizeof(m_Addr));
                m_Addr.sin_family=af;
                m_Addr.sin_port=htons(port);
                #if _WIN32
                    m_Addr.sin_addr.S_un.S_addr=inet_addr(ip);
                #elif __linux__
                    inet_pton(af,ip,&m_Addr.sin_addr.s_addr);
                #endif
            }
            SocketAddr(const SocketAddr& addr)
            {
                memcpy(&m_Addr,&addr.m_Addr,sizeof(addr.m_Addr));
            }
            inline struct sockaddr_in* data() { return &m_Addr; }
            inline size_t size() { return sizeof(m_Addr); }
            virtual ~SocketAddr() {  }
    };
    
    class Socket
    {
        private:
            bool m_IsClosed;

        protected:
            SOCKET_FD m_Fd;

        public:
            /* 构造函数 */
            Socket(int af=AF_INET,int type=SOCK_STREAM,int protocol=0)
                : m_IsClosed(false)
            {
                /* 初始化套接字，win 平台需要*/
                #if _WIN32
                WSADATA wsadata;
                if(WSAStartup(MAKEWORD(2,2),&wsadata)!=0)
                    throw std::string("WSAStart error!");
                #endif

                /* 创建套接字 */
                m_Fd=socket(af,type,protocol);
                #if _WIN32
                    if(m_Fd==INVALID_SOCKET)
                    {
                        WSACleanup();
                        throw std::string("socket error!");
                    }     
                #elif __linux__
                    if(m_Fd<=0)
                        throw std::string("socket error!");
                #endif
                m_IsClosed=true;
            }

            /* 禁止拷贝构造函数、赋值运算符 */
            Socket(const Socket& socket)=delete;
            Socket& operator=(const Socket& socket)=delete;

            /* 析构函数 */
            virtual ~Socket()
            {
                if(!isClosed())
                    Close();
            }

            inline bool isClosed() { return m_IsClosed; }

            inline void Close()
            {
                #if _WIN32
                    closesocket(m_Fd);
                    WSACleanup();
                #elif __linux_
                    close(m_Fd);
                #endif
            }

            void setTimeout(time_t time)
            {
                struct timeval timeout;
                timeout.tv_sec=time;
                timeout.tv_usec=0;
                setsockopt(m_Fd,SOL_SOCKET,SO_SNDTIMEO,(char*)&timeout,sizeof(struct timeval));
                /* 不知道为什么 Windows 下设置这个会有问题，暂时不用 */
                #if __linux__
                    setsockopt(m_Fd,SOL_SOCKET,SO_RCVTIMEO,(char*)&timeout,sizeof(struct timeval));
                #endif
            }

            void Send(const std::string& str)
            {
                send(m_Fd,str.c_str(),str.length(),0);
            }

            std::string Recv()
            {
                char buf[4096];
                recv(m_Fd,buf,sizeof(buf),0);
                return buf;
            }
    };

    class ClientSocket : public Socket
    {
        private:
            SocketAddr* m_ServerAddr;

        public:
            /* 构造函数 */
            ClientSocket(int af=AF_INET,int type=SOCK_STREAM,int protocol=0)
                : Socket(af,type,protocol), m_ServerAddr(nullptr) {}
            
            /* 禁止拷贝构造函数、赋值运算符 */
            ClientSocket(const ClientSocket& client)=delete;
            ClientSocket& operator=(const ClientSocket& client)=delete;

            /* 析构函数 */
            virtual ~ClientSocket() { if(m_ServerAddr!=nullptr) delete m_ServerAddr; }
            
            void setServerAddr(const SocketAddr& addr)
            {
                m_ServerAddr=new SocketAddr(addr);
            }

            bool Connect()
            {
                int status;
                #if __linux__
                    int flags;
                    flags=fcntl(m_Fd,F_GETFL);
                    fcntl(m_Fd,F_SETFL,flags|O_NONBLOCK);
                #endif
                    status=connect(m_Fd,(struct sockaddr*)(m_ServerAddr->data()),m_ServerAddr->size());
                #if _WIN32
                    if(status==SOCKET_ERROR)
                    {
                        #if _WIN32
                            closesocket(m_Fd);
                            WSACleanup();
                        #elif __linux_
                            close(m_Fd);
                        #endif
                        throw std::string("connect error!");
                    }
                #elif __linux__
                    if(status<0)
                    {
                        if(errno!=EINPROGRESS)
                        {
                            close(m_Fd);
                            throw std::string("connect error!");
                        }
                        printf("connect nonblock.\n");
                    }
                    fcntl(m_Fd,F_SETFL,flags);
                #endif
                return true;
            }
    };
};

#endif