#include <iostream>
#include "networking/server/tcp_server.h"

using boost::asio::ip::tcp;

int main(int argc, char *argv[])
{
    (void)argc;
    (void)argv;

    // Create and run the server on IPv4 and port 1337
    MOYF::TCPServer server{MOYF::IPV::V4, 1337};

    server.OnJoin = [](MOYF::TCPConnection::pointer server){
        std::cout << "Username has joined the server: " << server->GetUsername() << std::endl;
    };

    server.OnLeave = [](MOYF::TCPConnection::pointer server){
         std::cout << "Username has left the server: " << server->GetUsername() << std::endl;
    };

    server.OnClientMessage = [&server](const std::string& message){
        //Parse the message
        //Do game server things
        
        //Send message to client
        server.Broadcast(message);
    };

    // Run the server
    server.Run();

    return 0;
}

