#include "Commands.hpp"

int Commands::sockfd = 0;
std::string Commands::user = "";

std::string Commands::logIn(std::string username, std::string password){
    std::string assembleReq;
    assembleReq = "submit_login " + username + " " + password;
    if (send(Commands::sockfd, (char*)assembleReq.c_str(), assembleReq.size(), 0) < 0) {}
    std::cout << "Response Sent was: " << assembleReq << "\n";
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

std::vector<std::string> Commands::getEvents(std::string option) {
    //std::string assembleReq = "see_my_events " + Commands::user;
    std::string assembleReq;
    std::cout << "Option: " << option << "\n";
    if (option == "my"){
        assembleReq = "see_my_events " + Commands::user;
    }
    else
    {
        assembleReq = "all_events" + Commands::user;
    }
    if (send(Commands::sockfd, assembleReq.c_str(), assembleReq.size(), 0) < 0) {}
    std::cout << "Awaiting Response\n";
    char buffer[1024];
    if (recv(Commands::sockfd, buffer, 1024, 0) < 0) {std::cout << "ah shit\n";}
    std::vector<std::string> returned;
    std::stringstream ss(buffer);
    std::string item;
    while(getline(ss, item, '\n')) {
        returned.push_back(item);
    }
    return returned;
}

void Commands::joinEvent(std::string eventName) {
    std::string assemblyReq;
    assemblyReq = "join_event " + Commands::user + " " + eventName;
    if (send(Commands::sockfd, assemblyReq.c_str(), assemblyReq.size(), 0) < 0) {}
    char buffer[1024];
    if (recv(Commands::sockfd, buffer, 1023, 0) < 0) {std::cout << "ah shit\n";}
    std::cout << buffer << "\n";
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

bool sortSize(const std::vector<std::string>& v1, const std::vector<std::string>& v2)
{
    return std::stoi(v1[6]) < std::stoi(v2[6]);
}

bool sortDate(const std::vector<std::string>& v1, const std::vector<std::string>& v2)
{
    return std::stoi(v1[4]) < std::stoi(v2[4]);
}

bool sortDateRange(const std::vector<std::string>& v1, const std::vector<std::string>& v2)
{
    return std::stoi(v1[5]) < std::stoi(v2[5]);
}

bool sortName(const std::vector<std::string>& v1, const std::vector<std::string>& v2){
    return v1[0].compare(v2[0]);
}

bool sortOrganizer(const std::vector<std::string>& v1, const std::vector<std::string>& v2){
    return v1[1].compare(v2[1]);
}

void Commands::viewEvents(std::string option){
    std::vector<std::string> events = Commands::getEvents("my");
    std::vector<std::vector<std::string>> eventList;
    for (std::string event : events) {
        //substrings names, organizer, location, etc from events
        std::string temp = event;
        std::string name = temp.substr(0, temp.find(" "));
        temp = temp.substr(name.size() + 1);
        std::string organizer = temp.substr(0, temp.find(" "));
        temp = temp.substr(organizer.size() + 1);
        std::string location = temp.substr(0, temp.find(" "));
        temp = temp.substr(location.size() + 1);
        std::string date_start = temp.substr(0, temp.find(" "));
        temp = temp.substr(date_start.size() + 1);
        std::string date_range = temp.substr(0, temp.find(" "));
        temp = temp.substr(date_range.size() + 1);
        std::string description = temp.substr(0, temp.find(" "));
        temp = temp.substr(description.size() + 1);
        int size = std::stoi(temp);
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
        std::sort(eventList.begin(), eventList.end(), sortSize);

        for (int i = 0; i < eventList.size();i++) {
            for (int j = 0; j < eventList[0].size(); j++) {
                std::cout << eventList[i][j] << " ";
            }
            std::cout << "\n";
        }
    }
    else if(option == "name"){
        sort(eventList.begin(), eventList.end(), sortName);

        for (int i = 0; i < eventList.size();i++) {
            for (int j = 0; j < eventList[0].size(); j++) {
                std::cout << eventList[i][j] << " ";
            }
            std::cout << "\n";
        }
    }
    else if(option == "organizer"){
        sort(eventList.begin(), eventList.end(), sortOrganizer);

        for (int i = 0; i < eventList.size();i++) {
            for (int j = 0; j < eventList[0].size(); j++) {
                std::cout << eventList[i][j] << " ";
            }
            std::cout << "\n";
        }
    }
    else if(option == "date_start"){
        std::sort(eventList.begin(), eventList.end(), sortDate);

        for (int i = 0; i < eventList.size();i++) {
            for (int j = 0; j < eventList[0].size(); j++) {
                std::cout << eventList[i][j] << " ";
            }
            std::cout << "\n";
        }
    }
    else if(option == "date_range"){
        std::sort(eventList.begin(), eventList.end(), sortDateRange);

        for (int i = 0; i < eventList.size();i++) {
            for (int j = 0; j < eventList[0].size(); j++) {
                std::cout << eventList[i][j] << " ";
            }
            std::cout << "\n";
        }
    }
}
