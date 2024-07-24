#include <boost/asio.hpp>
#include <array>
#include <iostream>

using boost::asio::ip::tcp;

int main(int argc, char *argv[])
{

    (void)argc; // Suppress unused parameter warning
    (void)argv; // Suppress unused parameter warning

    try
    {
        boost::asio::io_context ioContext; // create the io

        tcp::resolver resolver{ioContext};                      // Resolver resolves hostnames for establishing connections but doesn't connect itself, the socket does that. Resolve possible endpoints
        auto endpoints = resolver.resolve("127.0.0.1", "1337"); // Endpoints created, a series of connections

        tcp::socket socket{ioContext};           // Create a socket
        boost::asio::connect(socket, endpoints); // Create connection

        while (true)
        { // if connection succeeds it will listen for messages and print to the console

            // listen for messages
            std::array<char, 128> buf;
            boost::system::error_code error;

            size_t len = socket.read_some(boost::asio::buffer(buf), error);

            if (error == boost::asio::error::eof)
            {
                // clean shutdown, the server has quit the connection
                break;
            }
            else if (error)
            {
                throw boost::system::system_error(error);
            }
            std::cout.write(buf.data(), len);
        }
    }
    catch (std::exception &e)
    {
        // Handle exception
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
