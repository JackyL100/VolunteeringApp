#ifndef SERVER_HPP
#define SERVER_HPP

#include <sys/socket.h>
#include <iostream>
#include <arpa/inet.h>
#include <sys/types.h>
#include <string>
#include <string_view>
#include <vector>
#include <deque>
#include <utility>
#include <thread>
#include <unordered_map>
#include <queue>

class Server {
    private:
        int sockfd;

        std::vector<std::thread> readingThreads;

        std::thread sendingThread;

        std::thread acceptionThread;

        socklen_t clilen;

        struct sockaddr_in serv_addr, cli_addr;

        bool alive;

        void reading(int client_socket);

        void accepting_new_clients();

        void error(const char* msg) {
            perror(msg);
            exit(1);
        }

    public:
        std::vector<int> connected_clients;

        std::queue<std::string> incoming_requests;

        Server(int port, int max_clients);

        void kill();
        
        std::string get_new_message();
};

#endif