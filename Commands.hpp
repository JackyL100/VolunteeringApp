#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <string>
#include <vector>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <iostream>

class Commands {
    public:
        static int sockfd;
        static void logIn(std::string username, std::string password);
        static void signUp(std::string username, std::string password);
        static void joinEvent(std::string eventName);
        static void createEvent(std::vector<std::string> eventInfo);
        static void viewEvents();
        static std::vector<std::string> getEvents();

};

int Commands::sockfd = 0;
#endif