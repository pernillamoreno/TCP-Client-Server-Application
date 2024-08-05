#ifndef TCP_CONNECTION_H
#define TCP_CONNECTION_H

#include <boost/asio.hpp>
#include <memory>
#include <iostream>
#include <queue>

namespace MOYF
{
    using boost::asio::ip::tcp;
    namespace io = boost::asio;

    using MessageHandler = std::function<void(std::string)>;
    using ErrorHandler = std::function<void()>;

    class TCPConnection : public std::enable_shared_from_this<TCPConnection>
    {
    public:
        using pointer = std::shared_ptr<TCPConnection>;

        // Factory method to create a new TCPConnection object
        static pointer Create(io::ip::tcp::socket&& socket) // Create function taking io::ip::tcp::socket with move semantics
        {
            return pointer(new TCPConnection(std::move(socket))); // Move the socket to the new TCPConnection object
        }

        // Getter for the username associated with the connection
        inline const std::string& GetUsername() const { return _username; }
        
        // Get the socket associated with the connection
        tcp::socket &Socket() 
        {
            return _socket;
        }

        // Start the connection with message and error handlers
        void Start(MessageHandler&& messageHandler, ErrorHandler&& errorHandler);
        
        // Post a message to the client. The message is added to the outgoing messages queue
        void Post(const std::string& message);

    private:
        // Constructor to initialize the connection with a socket using move semantics
        explicit TCPConnection(io::ip::tcp::socket&& socket);
        
        // Wait for a new message from the client
        void asyncRead();
        void onRead(boost::system::error_code ec, size_t bytesTransferred);

        void asyncWrite();
        void onWrite(boost::system::error_code ec, size_t bytesTransferred);

    private:
        tcp::socket _socket;   // The socket for the connection
        std::string _username; // Username associated with the connection

        std::queue<std::string> _outgoingMessages;  // Queue for storing outgoing messages
        io::streambuf _streamBuf {65536}; // Maximum buffer size to prevent buffer overflow

        MessageHandler _messageHandler; // Handler for processing received messages
        ErrorHandler _errorHandler; // Handler for processing errors
    };
}

#endif
