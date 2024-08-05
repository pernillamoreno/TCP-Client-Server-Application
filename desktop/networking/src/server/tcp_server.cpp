#include "networking/server/tcp_server.h"
#include <iostream>

namespace MOYF
{
    using boost::asio::ip::tcp;

    TCPServer::TCPServer(IPV ipv, int port) : _ipVersion(ipv), // Constructor, initializes the server with the given IP version and port
                                              _port(port),
                                              _acceptor(_ioContext, tcp::endpoint(_ipVersion == IPV::V4 ? tcp::v4() : tcp::v6(), _port))
    {
        // The acceptor is initialized with the appropriate IP version and port
    }

    int TCPServer::Run()
    { // Run the server

        try
        {
            startAccept();    // Start accepting connections.  startAccept defines our asynchronous functions
            _ioContext.run(); // Run the IO context to perform asynchronous operations
        }
        catch (std::exception &e)
        {
            std::cerr << e.what() << std::endl;
            return -1; // Return error code on exception
        }
        return 0; // Return success code
    }

    void TCPServer::Broadcast(const std::string &message){
        for (auto& connection : _connections){ // for connection in connections
            connection->Post(message); //for each connection, post message that broadcast
        }
      
    }

    void TCPServer::startAccept()
    { // Start accepting connections

        _socket.emplace(_ioContext); // create a socket we are waiting on

        _acceptor.async_accept(*_socket, [this](const boost::system::error_code &error) { // Asynchronously accept the connection
            auto connection = TCPConnection::Create(std::move(*_socket));                 // Create a new connection, move memory in to socket. Declare a move create a right hand side object

            if(OnJoin){
                OnJoin(connection);
            }
            _connections.insert(connection); // insert a set

            if (!error)
            {
                connection->Start(
                    [this](const std::string& message) { 
                        if (OnClientMessage) 
                            OnClientMessage(message); 
                    },
                    [&, weak = std::weak_ptr(connection)]{
                        if (auto shared = weak.lock(); shared && _connections.erase(shared)) {
                            if (OnLeave) OnLeave(shared);
                        }
                    }
                ); // Start the connection with handlers
            }
            startAccept(); // Accept next connection
        });
    }
}

