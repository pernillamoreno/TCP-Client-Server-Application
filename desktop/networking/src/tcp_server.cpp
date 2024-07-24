#include "networking/tcp_server.h"

#include <iostream>

namespace MOYF
{
    using boost::asio::ip::tcp;

    TCPServer::TCPServer(IPV ipv, int port) : _ipVersion(ipv),
                                              _port(port),
                                              _acceptor(_ioContext, tcp::endpoint(_ipVersion == IPV::V4 ? tcp::v4() : tcp::v6(), _port))
    {
        // create acceptor (tcp server) there are no default
    }

    int TCPServer::Run()
    {
        // run our ioContext
        try
        {
            startAccept();
            _ioContext.run(); // as long as ioContext run
        }
        catch (std::exception &e)
        {
            std::cerr << e.what() << std::endl;
            return -1; // if there's an error
        }
        return 0; // if there's not an error
    }

    void TCPServer::startAccept()
    {

        // create a connection
        auto connection = TCPConnection::Create(_ioContext);
        _connections.push_back(connection); // keep connection open

        // asynchronously accept the connection
        _acceptor.async_accept(connection->Socket(), [connection, this](const boost::system::error_code &error)
                               {
            if (!error) {
                connection->Start();
            }
            startAccept(); });
    }
}
