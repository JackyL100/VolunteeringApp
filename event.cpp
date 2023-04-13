#include "event.hpp"

Event::Event(std::string name, std::string description, std::string organizer, std::string location, std::string date_start, std::string date_range) {
    this->name = name;
    this->description = description;
    this->date_start = date_start;
    this->date_range = date_range;
    this->location = location;
    this->organizer = organizer;
}

std::string Event::getDescription() {
    return description;
}

std::string Event::getDate_Start() {
    return date_start;
}

std::string Event::getDate_Range() {
    return date_range;
}

std::string Event::getLocation() {
    return location;
}

std::string Event::getOrganizer() {
    return organizer;
}

std::string Event::toString() {
    return name + " " + organizer + " " + location + " " + date_start + " " + date_range + " " + description + "\n";
}

void Event::setDescription(std::string description) {
    description = description;
}

void Event::setDate_Start(std::string date_start) {
    date_start = date_start;
}

void Event::setDate_Range(std::string date_range) {
    date_range = date_range;
}

void Event::setLocation(std::string location) {
    location = location;
}

void Event::setOrganizer(std::string organizer) {
    organizer = organizer;
}

void Event::addVolunteer(std::string userName) {
    volunteers.push_back(userName);
}