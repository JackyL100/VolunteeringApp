#ifndef SERVER_HPP
#define SERVER_HPP

#include "event.hpp"
#include "user_profile.hpp"
#include "connection.hpp"
#include <sys/socket.h>
#include <iostream>
#include <arpa/inet.h>
#include <sys/types.h>
#include <string>
#include <string_view>
#include <vector>
#include <deque>
#include <utility>
#include <mutex>
#include <thread>
#include <unordered_map>
#include <queue>
#include <sstream>
#include <memory>
#include <fcntl.h>
#include <cassert>
#include <unistd.h>
#include <poll.h>
// compile command: g++ event.cpp user_profile.cpp calendar.cpp server.cpp app_server.cpp -std=c++17 -o server.out

class Server {
    private:
        int sockfd;

        std::vector<std::thread> readingThreads;
        
        std::unordered_map<std::string, std::shared_ptr<Event>> allEvents;

        std::unordered_map<std::string, std::shared_ptr<UserProfile>> users;

        std::unordered_map<int, std::shared_ptr<UserProfile>> loggedInProfiles;

        //std::unordered_map<std::string, int> connections;

        std::thread sendingThread;

        //std::thread thread_pool[5];

        //std::thread acceptingThread;

        socklen_t clilen;

        struct sockaddr_in serv_addr, cli_addr;

        bool alive;

        void set_to_non_blocking(int& sockfd);

        void reading(std::shared_ptr<Connection> connection);

        void respond(std::shared_ptr<Connection> connection);

        void addUserToEvent(std::shared_ptr<Connection> connection, std::string eventName);

        void addUserToEvent(std::string eventName, std::string userName);

        void sendEventList(std::shared_ptr<Connection> connection);

        bool checkLogIn(std::string userName, std::string password);

        void processLogIn(std::shared_ptr<Connection> connection, std::string userName, std::string password);

        void processSignUp(std::shared_ptr<Connection> connection, std::string userName, std::string password);

        void sendUserEvents(std::shared_ptr<Connection> connection, std::string userName);

        void createNewAccount(std::string newName, std::string newPassword);

        void createNewEvent(std::vector<std::string> info);

        void error(const char* msg) {
            perror(msg);
            exit(1);
        }

    public:

        std::queue<std::string> incoming_requests;

        std::vector<pollfd> poll_args;

        std::unordered_map<int, std::shared_ptr<Connection>> connections;

        Server(int port, int max_clients, bool use_dummy_events);

        void kill();

        bool isAlive();

        int get_sockfd() {
            return sockfd;
        }
        
        std::string get_new_message(bool remove);

        void process_request(std::shared_ptr<Connection> connection, std::string request);

        void decide_io(std::shared_ptr<Connection> connection);

        void accepting_new_clients();
};

#endif