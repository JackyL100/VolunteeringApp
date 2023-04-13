#include "server.hpp"
#include <chrono>

int main() {
    Server server(80, 10);
    while (server.isAlive()) {
        server.process_request(server.get_new_message(true));
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}