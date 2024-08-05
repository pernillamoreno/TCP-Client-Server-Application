#ifndef TCP_SERVER_H
#define TCP_SERVER_H

#include <boost/asio.hpp>
#include <networking/server/tcp_connection.h>
#include <functional>
#include <optional>
#include <unordered_set>

namespace MOYF {
    namespace io = boost::asio;

    // Enum for specifying IP version
    enum class IPV {
        V4,
        V6
    };

    class TCPServer {
        using OnJoinHandler = std::function<void(TCPConnection::pointer)>;
        using OnLeaveHandler = std::function<void(TCPConnection::pointer)>;
        using OnClientMessageHandler = std::function<void(std::string)>;

    public:
        TCPServer(IPV ipv, int port); // Constructor, initializes the server with the given IP version and port

        int Run();                                  // Method to run the server
        void Broadcast(const std::string &message); // Method to broadcast a message to all clients

    private:
        void startAccept(); // Method to start accepting connections

    public:
        OnJoinHandler OnJoin; // Handler for when a client joins
        OnLeaveHandler OnLeave; // Handler for when a client leaves
        OnClientMessageHandler OnClientMessage; // Handler for when a message is received from a client

    private:
        IPV _ipVersion; // IP version (IPv4 or IPv6)
        int _port;      // Port number

        io::io_context _ioContext;       // IO context to manage asynchronous operations
        io::ip::tcp::acceptor _acceptor; // Acceptor to listen for incoming connections

        std::optional<io::ip::tcp::socket> _socket; // Optional socket for the next incoming connection
        std::unordered_set<TCPConnection::pointer> _connections{}; // Container to hold active connections
    };
}

#endif // TCP_SERVER_H

