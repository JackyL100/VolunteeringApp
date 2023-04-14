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
#include <memory>

// compile command: g++ event.cpp user_profile.cpp calendar.cpp server.cpp app_server.cpp -std=c++17 -o server.out

class Server {
    private:
        int sockfd;

        std::vector<std::thread> readingThreads;
        
        std::unordered_map<std::string, std::shared_ptr<Event>> allEvents;

        std::unordered_map<std::string, std::unique_ptr<UserProfile>> users;

        std::unordered_map<std::string, int> connections;

        std::thread sendingThread;

        std::thread acceptingThread;

        socklen_t clilen;

        struct sockaddr_in serv_addr, cli_addr;

        bool alive;

        bool reading(int client_socket);

        void addUserToEvent(std::string eventName, std::string userName);

        void sendEventList(std::string userName);

        bool checkLogIn(std::string userName, std::string password);

        void sendUserEvents(std::string userName);

        void createNewAccount(std::string newName, std::string newPassword);

        void createNewEvent(std::vector<std::string> info);

        void error(const char* msg) {
            perror(msg);
            exit(1);
        }

    public:

        std::queue<std::string> incoming_requests;

        Server(int port, int max_clients, bool use_dummy_events);

        void kill();

        bool isAlive();
        
        std::string get_new_message(bool remove);

        void process_request(std::string request);

        void accepting_new_clients();
};

#endif