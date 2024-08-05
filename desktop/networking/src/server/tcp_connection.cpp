#include "networking/server/tcp_connection.h"
#include <iostream>
#include <sstream> // Include sstream if not already included

namespace MOYF
{

    // Constructor to initialize the socket with socket. Move socket from server to connection object
    TCPConnection::TCPConnection(io::ip::tcp::socket &&socket) : _socket(std::move(socket))
    {
        boost::system::error_code ec; // keep our username

        std::stringstream name;
        name << _socket.remote_endpoint();

        _username = name.str();
    }

    // Start method to begin the asynchronous operations for the connection
    void TCPConnection::Start(MessageHandler&& messageHandler, ErrorHandler&& errorHandler){
        _messageHandler = std::move(messageHandler);
        _errorHandler = std::move(errorHandler);

        asyncRead(); // asynRead loop
    }

    void TCPConnection::Post(const std::string &message)
    {
        bool queueIdle = _outgoingMessages.empty();
        _outgoingMessages.push(message);

        if (queueIdle) {
              asyncWrite();
        }
           
    }

    void TCPConnection::asyncRead()
    {
        io::async_read_until(_socket, _streamBuf, "\n", [self = shared_from_this()](boost::system::error_code ec, size_t bytesTranferred)
                             { self->onRead(ec, bytesTranferred); });
    }

    void TCPConnection::onRead(boost::system::error_code ec, size_t bytesTranferred)
    {
        if (ec)
        {
            _socket.close(ec);
            _errorHandler();
            return;
        }
        std::stringstream message;
        message << _username << ":" << std::istream(&_streamBuf).rdbuf(); /*if i call rdbuf() it consume bytes on the streambuf. if i want to do more whit the bytes i have to have use another function*/
        _streamBuf.consume(bytesTranferred);

        _messageHandler(message.str());
        asyncRead(); 
    }

    void TCPConnection::asyncWrite()
    {
        io::async_write(_socket, io::buffer(_outgoingMessages.front()), [self = shared_from_this()](boost::system::error_code ec, size_t bytesTransferred)
        {
            self->onWrite(ec, bytesTransferred);
        });
    }

    void TCPConnection::onWrite(boost::system::error_code ec, size_t /*bytesTranferred*/)
    {
        if (ec)
        {
            _socket.close(ec);
            _errorHandler();
            return;
        }

        _outgoingMessages.pop();

        if (!_outgoingMessages.empty())
        {
            asyncWrite();
        }
    }
}
