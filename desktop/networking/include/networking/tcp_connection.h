#ifndef TCP_CONNECTION_H
#define TCP_CONNECTION_H

#include <boost/asio.hpp>
#include <memory>
#include <iostream>

namespace MOYF
{
    using boost::asio::ip::tcp;

    class TCPConnection : public std::enable_shared_from_this<TCPConnection>
    {
    public:
        using pointer = std::shared_ptr<TCPConnection>;

       
        static pointer Create(boost::asio::io_context &ioContext)  // Factory method to create a shared pointer to a new TCPConnection
        {
            return pointer(new TCPConnection(ioContext));
        }

        
        tcp::socket &Socket() // Get the socket associated with the connection
        {
            return _socket;
        }

        // Start the connection
        void Start();

    private:
        
        explicit TCPConnection(boost::asio::io_context &ioContext); // Constructor is private to enforce the use of the factory method

        tcp::socket _socket;   // The socket for the connection
        std::string _message{"Hello, client\n"};   // Message to send to the client
    };
}

#endif
