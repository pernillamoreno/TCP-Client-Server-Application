#ifndef TCP_SERVER_H
#define TCP_SERVER_H

#include <boost/asio.hpp>
#include "networking/tcp_connection.h"
namespace MOYF
{
    enum class IPV
    {
        V4,
        V6
    };
    class TCPServer
    {

    public:
        TCPServer(IPV ipv, int port); // constuctor

        int Run();

    private:
        void startAccept();

    private:
        IPV _ipVersion;
        int _port;

        boost::asio::io_context _ioContext;
        boost::asio::ip::tcp::acceptor _acceptor; // create acceptor

        std::vector<TCPConnection::pointer> _connections{};
    };

}

#endif