#include "server.hpp"
#include <chrono>

void quit(Server& server) {
    std::cin.get();
    server.kill();
}

int main() {
    Server server(80, 10);
    std::thread killThread(quit, server);
    while (server.isAlive()) {
        server.process_request(server.get_new_message(true));
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}