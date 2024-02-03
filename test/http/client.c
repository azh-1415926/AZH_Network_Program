#include <mySocket/socket.h>
#include <myProtocol/http.h>

#define SERVER_IP "192.168.168.15"
#define SERVER_PORT 8080
#define HOST "192.168.168.15:8080"
#define URL "/Self/login/"

static char ServerIp[16];
static int ServerPort;
static char Host[21];
static char Url[32];

static void getHostInfo()
{
    char buf[6];
    char ch=0;
    memset(buf,0,sizeof(buf));
    memset(ServerIp,0,sizeof(ServerIp));
    memset(Host,0,sizeof(Host));
    #ifdef SERVER_IP
        strcat(ServerIp,SERVER_IP);
    #else
    printf("Please input server ip:");
    for(int i=0;(ch=getchar())&&ch!='\n';)
    {
        if(ch=='\0')
            continue;
        ServerIp[i]=ch;
        i++;
    }
    #endif
    #if defined(HOST) && defined(SERVER_PORT)
        strcat(Host,HOST);
        ServerPort=SERVER_PORT;
    #else
    printf("Please input server port:");
    while(!scanf("%d",&ServerPort))
        scanf("%*s");
    printf("port:%d\n",ServerPort);
    strcat(Host,ServerIp);
    Host[strlen(ServerIp)]=':';
    sprintf(buf,"%d",ServerPort);
    strcat(Host,buf);
    #endif
    #ifdef URL
        strcat(Url,URL);
    #else
    printf("Please input server url:");
    getchar();
    for(int i=0;(ch=getchar())&&ch!='\n';)
    {
        if(ch=='\0')
            continue;
        Url[i]=ch;
        i++;
    }
    #endif
}

int main()
{
    SOCKET_FD s;
    char buf[1024];
    struct sockaddr_in server_addr;
    struct httpMessage http;
    #if _WIN32
        system("chcp 65001");
        printf("[-] Winsock Initalize.\n");
        initalizeSocket();
    #endif
    getHostInfo();
    printf("[*] SERVER_IP:%s\n[*] SERVER_PORT:%d\n",ServerIp,ServerPort);
    setSockaddr(&server_addr,sizeof(server_addr),AF_INET,ServerIp,ServerPort);
    httpMessageInitalize(&http,sizeof(http),"GET",Url,Host);
    memset(buf,0,sizeof(buf));
    createHttpMessage(buf,&http);
    httpMessageFree(&http);
    printf("[*] HttpRequestMessage:\n--------------\n%s\n--------------\n",buf);
    s=Socket(AF_INET,SOCK_STREAM,0);
    printf("[-] Connecting to SERVER...\n");
    Connect(s,(struct sockaddr*)&server_addr,sizeof(server_addr));  
    printf("[-] Set timeout:2.\n");
    setTimeout(s,2);
    printf("[-] Sending to server.\n");
    send(s,buf,sizeof(buf),0);
    memset(buf,0,sizeof(buf));
    printf("[-] Receving from server.\n");
    recv(s,buf,sizeof(buf),0);
    printf("[*] Recevie success!\n");
    printf("[*] HttpResponseMessage:\n--------------\n%s\n--------------\n",buf);
    closeSocket(s);
    printf("[-] Clsoe socket.\n");
    return 0;
}