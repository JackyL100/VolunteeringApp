#ifndef SERVER_HPP
#define SERVER_HPP

#include "event.hpp"
#include "user_profile.hpp"
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
#include <sstream>

class Server {
    private:
        int sockfd;

        std::vector<std::thread> readingThreads;
        
        std::unordered_map<std::string, Event> allEvents;

        std::unordered_map<std::string, UserProfile> users;

        std::thread sendingThread;

        std::thread acceptingThread;

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

        void process_request(std::string request);
};

#endif