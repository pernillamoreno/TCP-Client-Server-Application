#include "networking/client/tcp_client.h"
#include <iostream>
#include <sstream>

namespace MOYF // Ensure the namespace matches the one in the header file
{

    // Constructor to initialize the client with the server address and port
    TCPClient::TCPClient(const std::string &address, int port) : _socket(_ioContext)
    {
        io::ip::tcp::resolver resolver{_ioContext};
        _endpoints = resolver.resolve(address, std::to_string(port)); // List of endpoints
    }

    // Method to run the client
    void TCPClient::Run()
    {
        io::async_connect(_socket, _endpoints, [this](boost::system::error_code ec, io::ip::tcp::endpoint /*ep*/){
            if (!ec)
                asyncRead(); // Start reading if connection is successful
        });

        _ioContext.run(); // Run the I/O context to perform asynchronous operations
    }

    // Method to stop the client
    void TCPClient::Stop()
    {
        boost::system::error_code ec;
        _socket.close(ec); // Close the socket

        if (ec)
        {
            // Process error if any
        }
    }

    // Method to post a message to the server
    void TCPClient::Post(const std::string &message)
    {
        bool queueIdle = _outgoingMessages.empty();
        _outgoingMessages.push(message); // Add the message to the outgoing messages queue

        if (queueIdle)
        {
            asyncWrite(); // Start writing if the queue was idle
        }
    }

    // Method to initiate an asynchronous read operation
    void TCPClient::asyncRead()
    {
        io::async_read_until(_socket, _streamBuf, "\n", [this](boost::system::error_code ec, size_t bytesTransferred){
            onRead(ec, bytesTransferred); // Handle the read completion
        });
    }

    // Handler for read completion
    void TCPClient::onRead(boost::system::error_code ec, size_t /*bytesTransferred*/)
    {
        if (ec)
        {
            Stop(); // Stop the client if there was an error
            return;
        }
        std::stringstream message;
        message << std::istream{&_streamBuf}.rdbuf(); // Extract the message from the stream buffer
        OnMessage(message.str()); // Call the message handler with the received message
        asyncRead();              // Start the read loop again
    }

    // Method to initiate an asynchronous write operation
    void TCPClient::asyncWrite()
    {
        io::async_write(_socket, io::buffer(_outgoingMessages.front()), [this](boost::system::error_code ec, size_t bytesTransferred){
            onWrite(ec, bytesTransferred); // Handle the write completion
        });
    }

    // Handler for write completion
    void TCPClient::onWrite(boost::system::error_code ec, size_t /*bytesTransferred*/)
    {
        if (ec)
        {
            Stop(); // Stop the client if there was an error
            return;
        }

        _outgoingMessages.pop(); // Remove the message from the queue

        if (!_outgoingMessages.empty())
        {
            asyncWrite(); // Continue writing if there are more messages in the queue
        }
    }
}

