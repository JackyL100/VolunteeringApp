#ifndef CALENDAR_HPP
#define CALENDAR_HPP

#include "event.hpp"
#include <vector>
#include <string>
#include <utility>
#include <memory>

class Calendar {
    private:
        std::vector<std::shared_ptr<Event>> events;
    public:
        void addEvent(std::string eventName, std::unordered_map<std::string, std::shared_ptr<Event>> eventList);
        std::vector<std::string> getEvents();
};

#endif