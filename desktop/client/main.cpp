#include <networking/client/tcp_client.h>
#include <iostream>
#include <thread>

using namespace MOYF;

int main(int argc, char *argv[])
{
    // Suppress unused parameter warnings
    (void)argc;
    (void)argv;
    TCPClient client{"localhost", 1337};

    client.OnMessage = [](const std::string& message)
    {
        std::cout << message;
    };

    std::thread t{[&client]()
                  { client.Run(); }};

    while (true)
    {
        std::string message;
        getline(std::cin, message);

        if (message == "\\q")
            break;
        message += "\n";

        client.Post(message);
    }

    client.Stop();
    t.join();  //wait for the thread to end and close application
    return 0;
}
