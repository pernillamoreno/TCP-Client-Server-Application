#ifndef TCP_SERVER_H
#define TCP_SERVER_H

#include <boost/asio.hpp>
#include "networking/tcp_connection.h"

namespace MOYF
{
    // Enum for specifying IP version
    enum class IPV
    {
        V4,
        V6
    };

    class TCPServer
    {
    public:
        // Constructor, initializes the server with the given IP version and port
        TCPServer(IPV ipv, int port);

        // Method to run the server
        int Run();

    private:
        // Method to start accepting connections
        void startAccept();

        // Private members
        IPV _ipVersion; // IP version (IPv4 or IPv6)
        int _port;      // Port number

        boost::asio::io_context _ioContext;       // IO context to manage asynchronous operations
        boost::asio::ip::tcp::acceptor _acceptor; // Acceptor to listen for incoming connections

        std::vector<TCPConnection::pointer> _connections{}; // Container to hold active connections
    };
}

#endif // TCP_SERVER_H

