#include <iostream>
#include "networking/tcp_server.h"

using boost::asio::ip::tcp;

int main(int argc, char *argv[])
{
    // Suppress unused parameter warnings
    (void)argc;
    (void)argv;

    // Create and run the server on IPv4 and port 1337
    MOYF::TCPServer server{MOYF::IPV::V4, 1337};

    // Run the server
    server.Run();

    return 0;
}
