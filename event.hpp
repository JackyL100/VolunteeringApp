#ifndef EVENT_HPP
#define EVENT_HPP

#include <string>
#include <vector>

class Event {
    private:
        std::string description;
        std::string date_start;
        std::string date_range;
        std::string location;
        std::string organizer;     
        std::string name;
        std::vector<std::string> volunteers;
        int size;

    public:

        std::string getDescription();
        std::string getDate_Start();
        std::string getDate_Range();
        std::string getLocation();
        std::string getOrganizer();
        int getSize();
        std::string toString();

        void setDescription(std::string description);
        void setDate_Start(std::string date_start);
        void setDate_Range(std::string date_range);
        void setLocation(std::string location);
        void setOrganizer(std::string organizer);
        void addVolunteer(std::string userName);

        Event(std::string name, std::string description, std::string organizer, std::string location, std::string date_start, std::string date_range);

};

#endif