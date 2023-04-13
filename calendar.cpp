#include "calendar.hpp"

void Calendar::addEvent(std::string eventName, std::unordered_map<std::string, std::shared_ptr<Event>> eventList) {
    events.push_back(eventList[eventName]);
}

std::vector<std::string> Calendar::getEvents() {
    std::vector<std::string> string_events;
    for (auto& event : events) {
        string_events.push_back(event->toString());
    }
    return string_events;
}