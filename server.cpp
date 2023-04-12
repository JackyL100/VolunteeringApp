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
    connected_clients.push_back(client_socket);
    readingThreads.push_back(std::thread([&](){while(alive){reading(client_socket);}}));
}
// request structure: request_type, socket, who is making request, parameters
void Server::process_request(std::string request) {
    std::vector<std::string> parsed;
    std::stringstream ss(request);
    std::string item;
    while(getline(ss, item, ' ')) {
        parsed.push_back(item);
    }
    if (parsed[0] == "join_event") {
        users[parsed[1]].joinEvent(parsed[2], allEvents);
    } else if (parsed[0] == "get_events") {

    } else if (parsed[0] == "submit_login") {

    } else if (parsed[0] == "see_my_events") {
        
    }
}