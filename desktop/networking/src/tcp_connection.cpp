#include "networking/tcp_connection.h"
#include <iostream>

namespace MOYF
{
    // Constructor to initialize the socket with the given io_context
    TCPConnection::TCPConnection(boost::asio::io_context &ioContext) : _socket(ioContext)
    {
    }

    // Start method to begin the asynchronous operations for the connection
    void TCPConnection::Start()
    {
        // Asynchronously write the message to the socket
        boost::asio::async_write(_socket, boost::asio::buffer(_message),
                                 [this](const boost::system::error_code &error, size_t bytesTransferred) {
                                     // The lambda function captures 'this' to access class members
                                     if (error)
                                     {
                                         std::cout << "Failed to send message! \n";
                                     }
                                     else
                                     {
                                         std::cout << "Sent " << bytesTransferred << " bytes of data!\n";
                                     }
                                 });

        // Create a buffer to store received data
        boost::asio::streambuf buffer;

        // Asynchronously receive data from the socket
        _socket.async_receive(buffer.prepare(512), [this](const boost::system::error_code &error, size_t bytesTransferred)
                              {
                                  if (error == boost::asio::error::eof)
                                  {
                                      // Clean connection cut off
                                      std::cout << "Client disconnected properly. \n";
                                  }
                                  else if (error)
                                  {
                                      std::cout << "Client disconnected in a bad way. \n";
                                  }
                                  // Use bytesTransferred to avoid unused parameter warning
                                  (void)bytesTransferred;
                              });
    }
}

