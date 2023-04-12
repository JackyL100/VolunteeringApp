#ifndef EVENT_HPP
#define EVENT_HPP

#include <string>

class Event {
    private:
        
    public:
        std::string description;
        std::string date_start;
        std::string date_range;
        std::string location;
        std::string organizer;

        std::string getDescription();
        std::string getDate_Start();
        std::string getDate_Range();
        std::string getLocation();
        std::string getOrganizer();

        void setDescription(std::string description);
        void setDate_Start(std::string date_start);
        void setDate_Range(std::string date_range);
        void setLocation(std::string location);
        void setOrganizer(std::string organizer);

};

#endif