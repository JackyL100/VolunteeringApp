#include "Commands.hpp"

int Commands::sockfd = 0;
std::string Commands::user = "";

std::string Commands::logIn(std::string username, std::string password){
    std::string assembleReq;
    assembleReq = "submit_login " + username + " " + password;
    if (send(Commands::sockfd, assembleReq.c_str(), assembleReq.size(), 0) < 0) {}
    std::cout << "Awaiting Response\n";
    char buffer[1024];
    if (recv(Commands::sockfd, buffer, 1023, 0) < 0) {std::cout << "ah shit\n";}
    return std::string(buffer);
}

std::string Commands::signUp(std::string username, std::string password){
    std::string assembleReq;
    assembleReq = "signup " + username + " " + password;
    if (send(Commands::sockfd, assembleReq.c_str(), assembleReq.size(), 0) < 0) {}
    std::cout << "Awaiting Response\n";
    char buffer[1024];
    if (recv(Commands::sockfd, buffer, 1023, 0) < 0) {std::cout << "ah shit\n";}
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
    if (send(Commands::sockfd, assemblyReq.c_str(), assemblyReq.size(), 0) < 0) {}
}

void Commands::viewEvents(std::string option){
    std::vector<std::string> events = Commands::getEvents();
    std::vector<std::vector<std::string>> eventList;
    for (std::string event : events) {
        //substrings names, organizer, location, etc from events
        std::string name = event.substr(0, event.find(" ") - 1);
        std::string organizer = event.substr(event.find(name.size(), 1) - 1, event.find(" ", name.size() + 1));
        std::string location = event.substr(event.find(organizer.size(), 1) - 1, event.find(" ", organizer.size() + 1));
        std::string date_start = event.substr(event.find(location.size(), 1) - 1, event.find(" ", location.size() + 1));
        std::string date_range = event.substr(event.find(date_start.size(), 1) - 1, event.find(" ", date_start.size() + 1));
        std::string description = event.substr(event.find(date_range.size(), 1) - 1, event.find(" ", date_range.size() + 1));
        int size = std::stoi(description.substr(0, description.find(" ") - 1));
        //puts them into a vector
        std::vector<std::string> singleEvent;
        singleEvent.push_back(name);
        singleEvent.push_back(organizer);
        singleEvent.push_back(location);
        singleEvent.push_back(date_start);
        singleEvent.push_back(date_range);
        singleEvent.push_back(description);
        singleEvent.push_back(std::to_string(size));
        //puts this vector into a 2d vector
        eventList.push_back(singleEvent);
    }
    if (option == "size"){
        //sorts by a size
        sort(eventList.begin(), eventList.end(), sortSize);

        for (int i = 0; i < eventList.size() - 1;i++) {
            for (int j = 0; j < eventList[0].size() - 1; j++) {
                std::cout << eventList[i][j] << " ";
            }
            std::cout << "\n";
        }
    }
    else if(option == ""){

    }
}

bool sortSize(const std::vector<int>& v1, const std::vector<int>& v2)
{
    return v1[6] < v2[6];
}

bool sortDate(const std::vector<int>& v1, const std::vector<int>& v2)
{
    return v1[6] < v2[6];
}