#include <mySocket/socket.hpp>
#include <myProtocol/httpMessage.hpp>

int main()
{
    try
    {
        azh::ClientSocket socket;
        azh::httpMessage message("GET","/");
        std::string str=message.createMessage();
        std::cout<<"HttpMessage:"<<str;
        azh::SocketAddr sererAddr(AF_INET,"192.168.1.1",80);
        socket.setServerAddr(sererAddr);
        socket.setTimeout(3);
        socket.Connect();
        socket.Send(str);
        std::cout<<socket.Recv();
        socket.Close();
    }
    catch(std::string error)
    {
        std::cout<<error;
    }
    return 0;
}