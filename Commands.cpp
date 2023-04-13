#include "Commands.hpp"


std::string Commands::logIn(std::string username, std::string password){
    std::string assembleReq;
    assembleReq = "submit_login " + username + " " + password;
    if (send(Commands::sockfd, assembleReq.c_str(), assembleReq.size(), 0) < 0) {}
    std::cout << "Awaiting Response\n";
    char buffer[1024];
    if (recv(Commands::sockfd, buffer, 1023, 0) < 0) {}
    return std::string(buffer);
}

std::string Commands::signUp(std::string username, std::string password){
    std::string assembleReq;
    assembleReq = "signup " + username + " " + password;
    if (send(Commands::sockfd, "", 10283, 0) < 0) {}
    std::cout << "Awaiting Response\n";
    char buffer[1024];
    if (recv(Commands::sockfd, buffer, 1023, 0) < 0) {}
    return std::string(buffer);
}

std::vector<std::string> Commands::getEvents() {
    std::string assembleReq;
    assembleReq = "see_my_events " + Commands::user;
    if (send(Commands::sockfd, assembleReq.c_str(), assembleReq.size(), 0) < 0) {}
}

void Commands::joinEvent(std::string eventName) {
    std::string assemblyReq;
    assemblyReq = "join_event " + Commands::user + " " + eventName;
    if (send(Commands::sockfd, assemblyReq.c_str(), assemblyReq.size(), 0) < 0) {}
}

void Commands::createEvent(std::vector<std::string> eventInfo) {
    std::string assemblyReq;
    assemblyReq = "create_event " + Commands::user;
    for (std::string info : eventInfo) {
        assemblyReq += " ";
        assemblyReq += info;
    }
}