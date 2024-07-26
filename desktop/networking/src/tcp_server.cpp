#include "networking/tcp_server.h"
#include <iostream>

namespace MOYF
{
    using boost::asio::ip::tcp;

    // Constructor, initializes the server with the given IP version and port
    TCPServer::TCPServer(IPV ipv, int port) : _ipVersion(ipv),
                                              _port(port),
                                              _acceptor(_ioContext, tcp::endpoint(_ipVersion == IPV::V4 ? tcp::v4() : tcp::v6(), _port))
    {
        // The acceptor is initialized with the appropriate IP version and port
    }

    // Run the server
    int TCPServer::Run()
    {
        try
        {
            startAccept();   // Start accepting connections
            _ioContext.run(); // Run the IO context to perform asynchronous operations
        }
        catch (std::exception &e)
        {
            std::cerr << e.what() << std::endl;
            return -1; // Return error code on exception
        }
        return 0; // Return success code
    }

    // Start accepting connections
    void TCPServer::startAccept()
    {
        auto connection = TCPConnection::Create(_ioContext); // Create a new connection
        _connections.push_back(connection);                  // Add connection to the list

        // Asynchronously accept the connection
        _acceptor.async_accept(connection->Socket(), [connection, this](const boost::system::error_code &error)
                               {
            if (!error) {
                connection->Start(); // Start the connection if no error
            }
            startAccept(); // Accept next connection
        });
    }
}

