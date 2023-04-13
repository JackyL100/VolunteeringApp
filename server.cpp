#include "server.hpp"

void Server::kill() {
    alive = false;
}

bool Server::isAlive() {
    return alive;
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
    acceptingThread = std::thread([this](){while(this->alive){accepting_new_clients();}});
    std::cout << "Server is accepting clients now\n";
}

std::string Server::get_new_message(bool remove) {
    while(incoming_requests.empty()) {}
    std::string new_message = incoming_requests.front();
    std::cout << "New Message" << new_message << "\n";
    if (remove) {
        incoming_requests.pop();
    }
    return new_message;
}

bool Server::reading(int client_socket) {
    char buffer[1024];
    if (recv(client_socket, buffer, 1023, 0) < 0) {
        std::string disconnected_user;
        for (const auto& [key, value] : connections) {
            if (value == client_socket) {
                disconnected_user = key;
            }
        }
        connections.erase(disconnected_user);
        return false;
    }
    std::string str(buffer);
    if (str.find("login") != std::string::npos) {
        str.insert(6, std::to_string(client_socket) + " ");
    } else if (str.find("signup") != std::string::npos) {
        str.insert(7, std::to_string(client_socket) + " ");
    }
    return true;
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
    readingThreads.push_back(std::thread([&](){while(alive){if (!reading(client_socket)) {break;}}}));
}

void Server::addUserToEvent(std::string eventName, std::string userName) {
    allEvents[eventName]->addVolunteer(userName);
}

void Server::sendEventList() {

}

bool Server::checkLogIn(std::string userName, std::string password) {
    auto it = users.find(userName);
    if (it != users.end()) {
        if (it->second->checkPassword(password)) {
            return true;
        }
    }
    return false;
}

void Server::sendUserEvents(std::string userName) {
    std::vector<std::string> user_events = users[userName]->getEvents();

}

void Server::createNewAccount(std::string newName, std::string newPassword) {
    users[newName] = std::make_unique<UserProfile>(newName, newPassword);
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
        if (allEvents.find(parsed[2]) != allEvents.end()) {
            users[parsed[1]]->joinEvent(parsed[2], allEvents);
            addUserToEvent(parsed[2], parsed[1]); 
        } else {
            if (send(connections[parsed[1]], "That event doesn't exist", 25, 0) < 0) {std::cout << "Error sending to " << parsed[1] << "\n";}
        }
    } else if (parsed[0] == "get_events") {

    } else if (parsed[0] == "submit_login") {
        if (checkLogIn(parsed[2], parsed[3])) {
            // send username to client
            connections[parsed[2]] = std::stoi(parsed[1]);
            if (send(connections[parsed[2]], parsed[2].c_str(), parsed[2].size(), 0) < 0) {std::cout << "Error sending to " << parsed[2] << "\n";}
        } else {
            // tell them they suck
            if (send(connections[parsed[2]], "DENIED haha begone incorrect user", 34, 0) < 0) {std::cout << "Error sending to" << parsed[2] << "\n";}
        }
    } else if (parsed[0] == "see_my_events") {
        sendEventList();
    } else if (parsed[0] == "signup") {
        //users[parsed[2]] = UserProfile(parsed[2], parsed[3]);
        createNewAccount(parsed[2], parsed[3]);
        connections[parsed[2]] = std::stoi(parsed[1]);
        if (send(connections[parsed[2]], parsed[2].c_str(), parsed[2].size(), 0) < 0) {std::cout << "Error sending to " << parsed[2] << "\n";}
    } else if (parsed[0] == "create_event") {
        std::vector<std::string> event_info(6);
    }
}