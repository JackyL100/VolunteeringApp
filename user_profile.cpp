#include "user_profile.hpp"

UserProfile::UserProfile(std::string username, std::string password) {
    this->username = username;
    this->password = password;
}

std::string UserProfile::getUsername() {
    return username;
}

std::string UserProfile::getPassword() {
    return password;
}

bool UserProfile::checkPassword(std::string attempt) {
    if (attempt == password) {
        return true;
    } else {
        return false;
    }
}

void UserProfile::joinEvent(std::string eventName, std::unordered_map<std::string, std::shared_ptr<Event>> eventList) {
    current_events.addEvent(eventName, eventList);
}

std::vector<std::string> UserProfile::getEvents() {
    return current_events.getEvents();
}