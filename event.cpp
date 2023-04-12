#include "event.hpp"

Event::Event(std::string description, std::string date_start, std::string date_range, std::string location, std::string organizer) {
    this->description = description;
    this->date_start = date_start;
    this->date_range = date_range;
    this->location = location;
    this->organizer = organizer;
}

std::string Event::getDescription(){
    return description;
}

std::string Event::getDate_Start(){
    return date_start;
}

std::string Event::getDate_Range(){
    return date_range;
}

std::string Event::getLocation(){
    return location;
}

std::string Event::getOrganizer(){
    return organizer;
}

void Event::setDescription(std::string description){
    description = description;
}

void Event::setDescription(std::string date_start){
    date_start = date_start;
}

void Event::setDescription(std::string date_range){
    date_range = date_range;
}

void Event::setDescription(std::string location){
    location = location;
}

void Event::setDescription(std::string organizer){
    organizer = organizer;
}