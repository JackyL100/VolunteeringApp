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

std::vector<std::string> Commands::getEvents(std::string option) {
    std::string assembleReq;
    if (option == "my"){
        assembleReq = "see_my_events " + Commands::user;
    }
    else
    {
        assembleReq = "all_events";
    }
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
    std::vector<std::string> events = Commands::getEvents("all");
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
        sort(eventList.begin(), eventList.end(), sortSize);

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
        sort(eventList.begin(), eventList.end(), sortDate);

        for (int i = 0; i < eventList.size();i++) {
            for (int j = 0; j < eventList[0].size(); j++) {
                std::cout << eventList[i][j] << " ";
            }
            std::cout << "\n";
        }
    }
    else if(option == "date_range"){
        sort(eventList.begin(), eventList.end(), sortDateRange);

        for (int i = 0; i < eventList.size();i++) {
            for (int j = 0; j < eventList[0].size(); j++) {
                std::cout << eventList[i][j] << " ";
            }
            std::cout << "\n";
        }
    }
}

bool sortSize(const std::vector<std::string>& v1, const std::vector<std::string>& v2)
{
    return std::stoi(v1[6]) < std::stoi(v2[6]);
}

bool sortDate(const std::vector<std::string>& v1, const std::vector<std::string>& v2)
{
    std::vector<std::string> arrlist1;
    std::vector<std::string> arrlist2;
    std::string first = v1[3];
    std::string second = v2[3];
    //splits first date into 3 
    arrlist1.push_back(first.substr(0, first.find("/")));
    first = first.substr(first.find("/") + 1);
    arrlist1.push_back(first.substr(0, first.find("/")));
    first = first.substr(first.find("/") + 1);
    arrlist1.push_back(first);
    //splites second date into 3
    arrlist2.push_back(second.substr(0, second.find("/")));
    second = second.substr(second.find("/") + 1);
    arrlist2.push_back(second.substr(0, second.find("/")));
    second = second.substr(second.find("/") + 1);
    arrlist2.push_back(second);
    //compares years
    if (std::stoi(arrlist1[2]) != std::stoi(arrlist2[2])) {
        
        if (std::stoi(arrlist1[2]) < std::stoi(arrlist2[2])) {
            return true;
        }
        else {
            return false;
        }
    }
    //compares months
    else if (std::stoi(arrlist1[0]) != std::stoi(arrlist2[0])) {

        if (std::stoi(arrlist1[0]) < std::stoi(arrlist2[0])) {
            return true;
        }
        else {
            return false;
        }
    }
    //compares days
    else if (std::stoi(arrlist1[1]) != std::stoi(arrlist2[1])) {

        if (std::stoi(arrlist1[1]) < std::stoi(arrlist2[1])) {
            return true;
        }
        else {
            return false;
        }
    }
    else {
        return true;
    }
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