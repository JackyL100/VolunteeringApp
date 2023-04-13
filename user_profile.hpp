#ifndef USER_PROFILE_HPP
#define USER_PROFILE_HPP

#include "calendar.hpp"
#include <string>
#include <vector>
#include <unordered_map>

class UserProfile {
    private:
        std::string username;

        std::string password;

        Calendar current_events;

    public:
        UserProfile(std::string username, std::string password);

        std::string getUsername();

        bool checkPassword(std::string attempt);

        void joinEvent(std::string eventName, std::unordered_map<std::string, Event> eventList);

        std::vector<std::string> getEvents(); 
};

#endif