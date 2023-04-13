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

std::string Server::get_new_message(bool remove) {
    std::string new_message = incoming_requests.front();
    std::cout << "New Message" << new_message << "\n";
    if (remove) {
        incoming_requests.pop();
    }
    return new_message;
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

void Server::addUserToEvent(std::string eventName, std::string userName) {
    allEvents[eventName].addVolunteer(userName);
}

void Server::sendEventList() {

}

bool Server::checkLogIn(std::string userName, std::string password) {
    auto it = users.find(userName);
    if (it != users.end()) {
        if (it->second.checkPassword(password)) {
            return true;
        }
    }
    return false;
}

void Server::sendUserEvents(std::string userName) {
    std::vector<std::string> user_events = users[userName].getEvents();

}

void Server::createNewAccount(std::string newName, std::string newPassword) {
    users[newName] = UserProfile(newName, newPassword);
}

void Server::createNewEvent(std::vector<std::string> info) {

}

void Server::process_request(std::string request) {
    // request structure: {request_type, socket, who is making request, parameters...}
    std::vector<std::string> parsed;
    std::stringstream ss(request);
    std::string item;
    while(getline(ss, item, ' ')) {
        parsed.push_back(item);
    }
    if (parsed[0] == "join_event") {
        users[parsed[1]].joinEvent(parsed[2], allEvents);
        addUserToEvent(parsed[2], parsed[1]);
    } else if (parsed[0] == "get_events") {

    } else if (parsed[0] == "submit_login") {
        if (checkLogIn) {
            // send username to client
        } else {
            // tell them they suck
        }
    } else if (parsed[0] == "see_my_events") {
        sendEventList();
    } else if (parsed[0] == "signup") {
        users[parsed[1]] = UserProfile(parsed[1], parsed[2]);
    } else if (parsed[0] == "create_event") {
        std::vector<std::string> event_info(6);
    }
}