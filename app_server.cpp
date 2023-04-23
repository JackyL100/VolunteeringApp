#include "server.hpp"
#include <chrono>
#include <future>
#include <functional>

/*void quit(Server& server) {
    std::cin.get();
    server.kill();
}*/


int main() {
    Server server(80, 10, true);
    //std::thread killThread(quit, std::ref(server));
    //std::vector<std::future<void>> poll_futures;
    while (server.isAlive()) {
        server.poll_args.clear();
        struct pollfd listening = {server.get_sockfd(), POLLIN, 0};
        server.poll_args.push_back(listening);
        for (auto& [key, value] : server.connections) {
            struct pollfd pfd = {};
            pfd.fd = value->sockfd;
            pfd.events = (value->state == STATE_REQ) ? POLLIN : POLLOUT;
            pfd.events = pfd.events | POLLERR;
            server.poll_args.push_back(pfd);
        }
        int rv = poll(server.poll_args.data(), server.poll_args.size(), 1000);
        if (rv < 0) {
            server.kill();
        }
        for (int i = 1; i < server.poll_args.size(); i++) {
            if (server.poll_args[i].revents) {
                server.decide_io(server.connections[server.poll_args[i].fd]);
                //poll_futures.push_back(std::async(std::launch::async, [&server, i](){server.decide_io(server.connections[server.poll_args[i].fd]);}));
                if (server.connections[server.poll_args[i].fd]->state == STATE_END) {
                    server.connections[server.poll_args[i].fd].reset();
                    close(server.poll_args[i].fd);
                }
            }
        }
        if (server.poll_args[0].revents) {
            server.accepting_new_clients();
        }
    }
}