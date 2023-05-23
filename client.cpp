#include "client.hpp"

app_client::app_client(char* hostname, int port) {
    net_client = std::make_unique<network_client>(hostname, port);
}

bool app_client::processLogInResponse(const std::string& response) {
    if (response == "DENIED haha begone incorrect user") {
        std::cout << "failed to login\n";
        return false;
    } else {
        userName = response;
        std::cout << "Logged In!\n";
        std::cout << "You are " << userName << "\n";
        return true;
    }
}

bool app_client::logIn(const std::string& username, const std::string& password) {
    std::string assembleReq;
    assembleReq = "submit_login " + username + " " + password;
    net_client->addToWriteBuffer(assembleReq);
    net_client->transmit();
    std::cout << "Response Sent was: " << assembleReq << "\n";
    std::cout << "Awaiting Response\n";
    net_client->receiveSmall();
    return processLogInResponse(net_client->getBuffer(true));
}

bool app_client::processSignUpResponse(const std::string& response) {
    if (response == "This account already exists") {
        std::cout << response << "\n";
        return false;
    } else {
        userName = response;
        std::cout << "You successfully signed up!\n";
        std::cout << "You are " << userName << "\n";
        return true;
    }
}

bool app_client::signUp(const std::string& username, const std::string& password) {
    std::string assembleReq;
    assembleReq = "signup " + username + " " + password;
    net_client->addToWriteBuffer(assembleReq);
    net_client->transmit();
    std::cout << "Awaiting Response\n";
    net_client->receiveSmall();
    return processSignUpResponse(net_client->getBuffer(true));
}

std::string app_client::getUsername() {
    return userName;
}

void app_client::help() {
    std::cout << "!login username password\n";
    std::cout << "!signup username password\n";
    std::cout << "!get_events filter\n";
    std::cout << "!create_event\n";
    std::cout << "!join_event\n";
    std::cout << "!exit\n";
}

std::vector<std::string> app_client::createEventScreen() {
    std::vector<std::string> event_data(5);
    std::cout << "Enter the name of the Volunteering Event: ";
    std::cin >> event_data[0];
    std::cout << "Enter the description of the Volunteering Event: ";
    std::cin >> event_data[1];
    std::cout << "Enter the location of the Volunteering Event: ";
    std::cin >> event_data[2];
    std::cout << "Enter the starting date of the Volunteering Event: ";
    std::cin >> event_data[3];
    std::cout << "Enter how long the Volunteering Event will last: ";
    std::cin >> event_data[4];
    return event_data;
}

void app_client::joinEvent(const std::string& eventName) {
    std::string assemblyReq;
    assemblyReq = "join_event " + userName + " " + eventName;
    net_client->addToWriteBuffer(assemblyReq);
    net_client->transmit();
    net_client->receiveSmall();
    std::cout << net_client->getBuffer(true);
}

void app_client::createEvent() {
    std::vector<std::string> eventInfo = createEventScreen();
    std::string assemblyReq;
    assemblyReq = "create_event " + userName;
    for (std::string info : eventInfo) {
        assemblyReq += " ";
        assemblyReq += info;
    }
    net_client->addToWriteBuffer(assemblyReq);
    net_client->transmit();
}

std::vector<std::string> app_client::getEvents() {
    std::string request = "get_my_events " + userName;
    net_client->addToWriteBuffer(request);
    net_client->transmit();
    net_client->receiveSmall();
    std::stringstream ss(net_client->getBuffer(true));
    std::string event;
    std::vector<std::string> eventList;
    while(getline(ss, event, '\n')) {
        eventList.push_back(event);
    }
    return eventList;
}

void app_client::viewEvents() {
    std::vector<std::string> events = getEvents();
    for (int i = 0; i < events.size(); i++) {
        std::cout << i + 1 << ": " << events[i] << "\n";
    }
}