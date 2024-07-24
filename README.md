#include <boost/asio.hpp>
#include <iostream>

using boost::asio::ip::tcp;

int main(int argc, char *argv[])
{
    (void)argc; // Suppress unused parameter warning
    (void)argv; // Suppress unused parameter warning
    try
    {
        boost::asio::io_context io_context;

        tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), 1337)); // creat a acceptor for tcp, 
        /*passed in v4 but we can abstract it out and mabey we can switch out our network backend in the future if we see fit*/ 

        while (true)
        {
            std::cout << "Accept connect on port 1337!\n";

            tcp::socket socket(io_context); // connection blocks until it get connection
            acceptor.accept(socket);        // when they get connection here they put info in socket

            std::cout << "Client connect! Sending message!\n";

            std::string hello_message = "Hello, client! \n";
            boost::system::error_code error;

            boost::asio::write(socket, boost::asio::buffer(hello_message), error);
        }
    }
    catch (std::exception &e)
    {

        std::cerr << "Exception: " << e.what() << std::endl;
    }
    return 0;
}*/


tcp_server.cpp
#include "networking/tcp_server.h"
#include "networking/tcp_connection.h"
#include <iostream>

namespace MOYF
{
    using boost::asio::ip::tcp;

    TCPServer::TCPServer(IPV ipv, int port) : 
        _ipVersion(ipv), 
        _port(port),
        _acceptor(_ioContext, tcp::endpoint(_ipVersion == IPV::V4 ? tcp::v4() : tcp::v6(), _port)) 
    { 
        // create acceptor (tcp server) there are no default
    }

    int TCPServer::Run() { 
        // run our ioContext
        try {
            startAccept();
            _ioContext.run(); // as long as ioContext run
        } catch (std::exception& e) {
            std::cerr << e.what() << std::endl;
            return -1; // if there's an error
        }
        return 0; // if there's not an error
    }

    void TCPServer::startAccept() {
        // create a connection
        auto connection = TCPConnection::Create(_ioContext);

        // asynchronously accept the connection
        _acceptor.async_accept(connection->Socket(), [connection, this](const boost::system::error_code& error) { //HERE [connection, this] MAKES A COPY 
            if (!error) {
                connection->Start();
            }
            startAccept();  //AND HERE IT EXIT THE FUNCTION.   WE DONT WONT THAT WE WANT TO STAY CONNECTED
        });
    }
}  