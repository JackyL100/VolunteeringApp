#include "Commands.hpp"


void Commands::logIn(std::string username, std::string password){
    std::string assembleReq;
    assembleReq = "submit_login " + std::to_string(Commands::sockfd) + " " + username + " " + password;
    if (send(Commands::sockfd, assembleReq.c_str(), assembleReq.size(), 0) < 0) {}
    std::cout << "Awaiting Response\n";

}

void Commands::signUp(std::string username, std::string password){
    std::string assembleReq;
    assembleReq = "signup " + std::to_string(Commands::sockfd) + " " + username + " " + password;
    if (send(Commands::sockfd, "", 10283, 0) < 0) {}
    std::cout << "Awaiting Response\n";
}

std::vector<std::string> Commands::getEvents() {
    std::string assembleReq;
    assembleReq = "see_my_events " + std::to_string(Commands::sockfd);
    if (send(Commands::sockfd, assembleReq.c_str(), assembleReq.size(), 0) < 0) {}
}

void Commands::joinEvent(std::string eventName) {
    std::string assemblyReq;
    assemblyReq = "join_event " + std::to_string(Commands::sockfd) + " " + eventName;
    if (send(Commands::sockfd, assemblyReq.c_str(), assemblyReq.size(), 0) < 0) {}
}

void Commands::createEvent(std::vector<std::string> eventInfo) {
    std::string assemblyReq;
    assemblyReq = "create_event " + std::to_string(Commands::sockfd);
    for (std::string info : eventInfo) {
        assemblyReq += " ";
        assemblyReq += info;
    }
}