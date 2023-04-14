#ifndef USER_PROFILE_HPP
#define USER_PROFILE_HPP

#include "calendar.hpp"
#include <string>
#include <vector>
#include <unordered_map>
#include <utility>
#include <memory>

class UserProfile {
    private:
        std::string username;

        std::string password;

        Calendar current_events;

    public:
        UserProfile(std::string username, std::string password);

        std::string getUsername();

        std::string getPassword();

        bool checkPassword(std::string attempt);

        void joinEvent(std::string eventName, std::unordered_map<std::string, std::shared_ptr<Event>> eventList);

        std::vector<std::string> getEvents(); 
};

#endif