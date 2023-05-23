#include "server.hpp"

void Server::kill() {
    alive = false;
    std::cout << "Server was killed\n";
}

bool Server::isAlive() {
    return alive;
}

void Server::set_to_non_blocking(int& sockfd) {
    errno = 0;
    int flags = fcntl(sockfd, F_GETFL, 0);
    if (errno) {
        kill();
    }
    flags |= O_NONBLOCK;
    errno = 0;
    fcntl(sockfd, F_SETFL, flags);
    if (errno) {
        kill();
    }
}

Server::Server(int port, int max_clients, bool use_dummy_data) {
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) error("SERVER ERROR OPENING SOCKET");
    memset(&serv_addr,0,sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(port);
    if (bind(sockfd, (sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {error("ERROR ON BINDING");}
    listen(sockfd, max_clients);
    alive = true;
    set_to_non_blocking(sockfd);
    //acceptingThread = std::thread([this](){while(this->alive){accepting_new_clients();}});
    /*for (int i = 0; i < 5; i++) {
        thread_pool[i] = std::thread(assign_load);
    }*/
    std::cout << "Server is accepting clients now\n";
    if (use_dummy_data) {
        users["Bob"] = std::make_shared<UserProfile>("Bob", "1234");
        allEvents["WWF Fundraiser"] = std::make_shared<Event>("WWF Fundraiser", "Raise money for animal conservation", "WWF", "New York", "01/00/00", "1");
        users["Bob"]->joinEvent("WWF Fundraiser", allEvents);
        addUserToEvent("WWF Fundraiser", "Bob"); 
        allEvents["WWF Fundraiser 2"] = std::make_shared<Event>("WWF Fundraiser 2", "Raise money for animal conservation", "WWF", "New York", "02/00/00", "1");
        users["Bob"]->joinEvent("WWF Fundraiser 2", allEvents);
        addUserToEvent("WWF Fundraiser 2", "Bob"); 
    }
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

void Server::decide_io(std::shared_ptr<Connection> connection) {
    if (connection->state == STATE_REQ) {
        reading(connection);
        std::string req{connection->read_buf};
        memset(connection->read_buf, 0, 4 + MAX_MSG_SIZE);
        connection->read_buf_size = 0;
        process_request(connection, req);
    } else if (connection->state == STATE_RES) {
        std::cout << "trying to respond\n";
        respond(connection);
        memset(connection->write_buf, 0, 4 + MAX_MSG_SIZE);
        connection->read_buf_size = 0;
    }
}


void Server::reading(std::shared_ptr<Connection> connection) {
    while (true) {
        // try to fill buffer
        assert(connection->read_buf_size < sizeof(connection->read_buf));
        int rv = 0;
        do {
            int cap = sizeof(connection->read_buf) - connection->read_buf_size;
            rv = recv(connection->sockfd, &connection->read_buf[connection->read_buf_size], cap ,0);
        } while (rv < 0 && errno == EINTR);
        if (rv < 0 && errno == EAGAIN) {
            break;
        }
        if (rv < 0) {
            connection->state = STATE_END;
            break;
        }
        if (rv == 0) {
            if (connection->read_buf_size > 0) {
                std::cout << "unexpected EOF\n";
            } else {
                std::cout << "EOF\n";
            }
            connection->state = STATE_END;
            break;
        }
        connection->read_buf_size += rv;
        assert(connection->read_buf_size <= sizeof(connection->read_buf) - connection->read_buf_size);
    }
}

void Server::respond(std::shared_ptr<Connection> connection) {
    while(true) {
        // try to flush buffer
        int rv = 0;
        do {
            int remain = connection->write_buf_size - connection->write_buf_sent;
            rv = send(connection->sockfd, &connection->write_buf[connection->write_buf_sent], remain, 0);
        } while(rv < 0 && errno == EINTR);
        if (rv < 0 && errno == EAGAIN) {
            break;
        }
        if (rv < 0) {
            connection->state = STATE_END;
            break;
        }
        connection->write_buf_sent += rv;
        std::cout << connection->write_buf_sent << "\n";
        assert(connection->write_buf_sent <= connection->write_buf_size);
        if (connection->write_buf_sent == connection->write_buf_size) {
            connection->state = STATE_REQ;
            connection->write_buf_sent = 0;
            connection->write_buf_size = 0;
            break;
        }
    }
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
    set_to_non_blocking(client_socket);
    connections[client_socket] = std::make_shared<Connection>(client_socket);
    //readingThreads.push_back(std::thread([&](){while(alive){if (!reading(client_socket)) {std::cout << "Broken\n";break;}}}));
}

void Server::addUserToEvent(std::shared_ptr<Connection> connection, std::string eventName) {
    std::string response;
    if (allEvents.find(eventName) != allEvents.end()) {
        loggedInProfiles[connection->sockfd]->joinEvent(eventName, allEvents);
        allEvents[eventName]->addVolunteer(loggedInProfiles[connection->sockfd]->getUsername());
        response = "Successfully added to event";
    } else {
        response = "This event doesnt exist";
    }
    memcpy(&connection->write_buf, response.c_str(), response.size());
}

void Server::addUserToEvent(std::string eventName, std::string userName) {
    if (allEvents.find(eventName) != allEvents.end()) {
        users[userName]->joinEvent(eventName, allEvents);
        allEvents[eventName]->addVolunteer(userName);
    } 
}


void Server::sendEventList(std::shared_ptr<Connection> connection) {
    std::string all_events_string = "";
    for (const auto& [key, value] : allEvents) {
        all_events_string += value->toString();
    }
    memcpy(&connection->write_buf, all_events_string.c_str(), all_events_string.size());
    connection->write_buf_size = all_events_string.size();
    connection->state = STATE_RES;
}

bool Server::checkLogIn(std::string userName, std::string password) {
    auto it = users.find(userName);
    if (it != users.end()) {
        if (it->second->checkPassword(password)) {
            std::cout << "Login successful\n";
            return true;
        }
        std::cout << "Attempted Password: " << password << "|" << "Password: " << it->second->getPassword() << "\n";
    }
    std::cout << "Login Fail\n";
    return false;
}

void Server::sendUserEvents(std::shared_ptr<Connection> connection, std::string userName) {
    if (users.find(userName) != users.end()) {
        std::vector<std::string> user_events = users[userName]->getEvents();
        std::string user_events_string = "";
        for (std::string& event: user_events) {
            user_events_string += event;
        }
        memcpy(&connection->write_buf, user_events_string.c_str(), user_events_string.size());
        connection->write_buf_size = user_events_string.size();
        
    } else {
        std::cout << "user doesnt exist\n";
        std::string response = "user doesnt exist";
        memcpy(&connection->write_buf, response.c_str(), response.size());
        connection->write_buf_size = response.size();
    }
    connection->state = STATE_RES;
}

void Server::createNewAccount(std::string newName, std::string newPassword) {
    users[newName] = std::make_shared<UserProfile>(newName, newPassword);
}
/*
void Server::createNewEvent(std::vector<std::string> info) {

}
*/

void Server::processLogIn(std::shared_ptr<Connection> connection, std::string userName, std::string password) {
    if (checkLogIn(userName, password)) {
        // send username to client
        loggedInProfiles[connection->sockfd] = users[userName];
        memcpy(&connection->write_buf, userName.c_str(), userName.size());
        connection->write_buf_size = userName.size();
    } else {
        // tell them they suck
        std::string response = "DENIED haha begone incorrect user";
        memcpy(&connection->write_buf, response.c_str(), response.size());
        connection->write_buf_size = response.size();
    }
    connection->state = STATE_RES;
}

void Server::processSignUp(std::shared_ptr<Connection> connection, std::string userName, std::string password) {
    if (users.find(userName) == users.end()) {
        createNewAccount(userName, password);
        loggedInProfiles[connection->sockfd] = users[userName];
        memcpy(&connection->write_buf, userName.c_str(), userName.size());
    } else {
        std::string response = "This account exists\n";
        memcpy(&connection->write_buf, response.c_str(), response.size());
    }
    connection->state = STATE_RES;
}

void Server::process_request(std::shared_ptr<Connection> connection, std::string request) {
    // request structure: {request_type, parameters...}
    std::vector<std::string> parsed;
    std::stringstream ss(request);
    std::string item;
    while(getline(ss, item, ' ')) {
        parsed.push_back(item);
        std::cout << item << " ";
    }
    std::cout << "\n";
    if (parsed[0] == "join_event") {
        addUserToEvent(connection, parsed[1]);
    } else if (parsed[0] == "all_events") {
        sendEventList(connection);
    } else if (parsed[0] == "submit_login") {
        std::cout << "Someone is trying to login\n";
        processLogIn(connection, parsed[1], parsed[2]);
    } else if (parsed[0] == "get_my_events") {
        std::cout << "tried to see events\n";
        sendUserEvents(connection, parsed[1]);
    } else if (parsed[0] == "signup") {
        processSignUp(connection, parsed[1], parsed[2]);
    } else if (parsed[0] == "create_event") {
        std::vector<std::string> event_info(6);
    } else {
        std::cout << "Received: " << request << "\n";
    }
}