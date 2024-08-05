#ifndef TCP_CLIENT_H
#define TCP_CLIENT_H

#include <boost/asio.hpp> 
#include <queue> 

namespace MOYF {
    namespace io = boost::asio; //io to simplify usage
    using MessageHandler = std::function<void(std::string)>; // Define a message handler function

    class TCPClient {
    public:
        TCPClient(const std::string& address, int port); // Constructor to initialize the client with server address and port

        void Run(); // start the client
        void Stop(); 
        void Post(const std::string& message); // send a message to the server

    private:
        void asyncRead(); 
        void onRead(boost::system::error_code ec, size_t bytesTransferred); 
        void asyncWrite(); 
        void onWrite(boost::system::error_code ec, size_t bytesTransferred); 

    public:
        MessageHandler OnMessage; // Callback for handling received messages

    private:
        io::io_context _ioContext{}; // Boost.Asio I/O context for managing asynchronous operations
        io::ip::tcp::socket _socket; // TCP socket for communication with the server

        io::ip::tcp::resolver::results_type _endpoints; // Resolved endpoints for the server address

        io::streambuf _streamBuf{65536}; // Stream buffer for reading data from the server
        std::queue<std::string> _outgoingMessages{}; // Queue for storing outgoing messages
    };

}

#endif
