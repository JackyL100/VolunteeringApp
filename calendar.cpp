#include "calendar.hpp"

void Calendar::addEvent(std::string eventName, std::unordered_map<std::string, Event> eventList) {
    events.push_back(eventList[eventName]);
}