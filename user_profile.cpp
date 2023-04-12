#include "user_profile.hpp"

UserProfile::UserProfile(std::string username, std::string password) {
    this->username = username;
    this->password = password;
}

std::string UserProfile::getUsername() {
    return username;
}