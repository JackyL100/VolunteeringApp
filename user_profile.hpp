#ifndef USER_PROFILE_HPP
#define USER_PROFILE_HPP

#include "calendar.hpp"
#include <string>
#include <vector>

class UserProfile {
    private:
        std::string username;

        std::string password;

        Calendar current_events;

    public:
        UserProfile(std::string username, std::string password);

        void displayCalendar();

        std::string getUsername();

};

#endif