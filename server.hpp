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

        std::unordered_map<std::string, int> connections;

        std::thread sendingThread;

        std::thread acceptingThread;

        socklen_t clilen;

        struct sockaddr_in serv_addr, cli_addr;

        bool alive;

        bool reading(int client_socket);

        void addUserToEvent(std::string eventName, std::string userName);

        void sendEventList();

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

        Server(int port, int max_clients);

        void kill();

        bool isAlive();
        
        std::string get_new_message(bool remove);

        void process_request(std::string request);

        void accepting_new_clients();
};

#endif