
#include <iostream>
#include <networking/tcp_server.h>

using boost::asio::ip::tcp;

int main(int argc, char *argv[])
{
    (void)argc; // Suppress unused parameter warning
    (void)argv; // Suppress unused parameter warning

    MOYF::TCPServer server{MOYF::IPV::V4, 1337};

    server.Run();

    return 0;
}