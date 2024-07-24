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

        static pointer Create(boost::asio::io_context &ioContext)
        {
            return pointer(new TCPConnection(ioContext));
        }
        tcp::socket &Socket()
        {
            return _socket;
        }

        void Start();

    private:
        explicit TCPConnection(boost::asio::io_context &ioContext); // constuctor

    private:
        tcp::socket _socket; // create our socket
        std::string _message{"Hello, client\n"};
    };
}
#endif