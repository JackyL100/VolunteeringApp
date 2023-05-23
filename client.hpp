#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "network_client.hpp"
#include <vector>
#include <unordered_map>
#include <sstream>
#include <utility>
#include <memory>

class app_client {
    private:
        std::unique_ptr<network_client> net_client;
        std::string userName;
        std::vector<std::string> createEventScreen();
        bool processLogInResponse(const std::string& response);
        bool processSignUpResponse(const std::string& response);
    public:
        app_client(char* hostname, int port);
        bool logIn(const std::string& username, const std::string& password);
        bool signUp(const std::string& username, const std::string& password);
        std::string getUsername();
        void help();
        void joinEvent(const std::string& eventName);
        void createEvent();
        std::vector<std::string> getEvents();
        void viewEvents();
};


#endif