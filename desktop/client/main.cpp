#include <boost/asio.hpp>
#include <array>
#include <iostream>

using boost::asio::ip::tcp;

int main(int argc, char *argv[])
{
    // Suppress unused parameter warnings
    (void)argc;
    (void)argv;

    try
    {
        // Create an io_context object to manage asynchronous I/O operations
        boost::asio::io_context ioContext;

        // Create a resolver object to resolve hostnames to TCP endpoints
        tcp::resolver resolver{ioContext};

        // Resolve the server address and port
        auto endpoints = resolver.resolve("127.0.0.1", "1337");

        // Create a TCP socket
        tcp::socket socket{ioContext};

        // Connect the socket to one of the resolved endpoints
        boost::asio::connect(socket, endpoints);

        // Main loop to read messages from the server
        while (true)
        {
            // Buffer to store incoming data
            std::array<char, 128> buf;
            boost::system::error_code error;

            // Read data from the socket
            size_t len = socket.read_some(boost::asio::buffer(buf), error);

            if (error == boost::asio::error::eof)
            {
                // Clean shutdown: the server closed the connection
                break;
            }
            else if (error)
            {
                // Other errors are thrown as exceptions
                throw boost::system::system_error(error);
            }

            // Write the received data to the standard output
            std::cout.write(buf.data(), len);
        }
    }
    catch (std::exception &e)
    {
        // Print any exceptions that are thrown
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
