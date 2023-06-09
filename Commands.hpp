#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <string>
#include <vector>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <iostream>
#include <algorithm>
#include <sstream>

class Commands {
    public:
        static int sockfd;
        static std::string user;
        static std::string logIn(std::string username, std::string password);
        static std::string signUp(std::string username, std::string password);
        static void joinEvent(std::string eventName);
        static void createEvent(std::vector<std::string> eventInfo);
        static void viewEvents(std::string option);
        static std::vector<std::string> getEvents(std::string option);

};

#endif