#ifndef CALENDAR_HPP
#define CALENDAR_HPP

#include "event.hpp"
#include <vector>
#include <string>

class Calendar {
    private:
        std::vector<Event> events;
    public:
        void addEvent(std::string eventName, std::unordered_map<std::string, Event> eventList);
        std::vector<std::string> getEvents();
};

#endif