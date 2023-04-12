#include "user_profile.hpp"

UserProfile::UserProfile(std::string username, std::string password) {
    this->username = username;
    this->password = password;
}

std::string UserProfile::getUsername() {
    return username;
}

bool UserProfile::checkPassword(std::string attempt) {
    if (attempt == password) {
        return true;
    } else {
        return false;
    }
}

void UserProfile::joinEvent(std::string eventName, std::unordered_map<std::string, Event> eventList) {
    current_events.addEvent(eventName, eventList);
}