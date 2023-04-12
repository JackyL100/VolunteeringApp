#include "server.hpp"

void Server::kill() {
    alive = false;
}

Server::Server(int port, int max_clients) {
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) error("SERVER ERROR OPENING SOCKET");
    memset(&serv_addr,0,sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(port);
    if (bind(sockfd, (sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {error("ERROR ON BINDING");}
    listen(sockfd, max_clients);
}

std::string Server::get_new_message() {
    std::string new_massage = incoming_requests.front();
    incoming_requests.pop();
    return new_massage;
}

void Server::reading(int client_socket) {
    char buffer[1024];
    if (recv(client_socket, buffer, 1023, 0) < 0) error("Could not read");
    incoming_requests.emplace(buffer);
}

void Server::accepting_new_clients() {
    struct sockaddr_in client_address;
    socklen_t clilen;
    clilen = sizeof(client_address);
    int client_socket = accept(sockfd, (sockaddr*)&cli_addr, &clilen);
    if (client_socket < 0) {
        error("Could not accept new client");
    } else {
        std::cout << "Accepted client\n";
    }
    readingThreads.push_back(std::thread([&](){while(alive){reading(client_socket);}}));
}