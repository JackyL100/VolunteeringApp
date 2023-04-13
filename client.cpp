#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include "commands.hpp"

void get_commands(){
    std::cout << "!login username password\n";
    std::cout << "!signup username password\n";
    std::cout << "!get_events";
    std::cout << "!exit\n";
    return;
}

void error(const char* msg) {
    perror(msg);
    exit(1);
}

int main(int argc, char* argv[])
{
    if (argc < 3) {
        std::cout << "Hostname Port\n";
        exit(0);
    }

    struct sockaddr_in serv_addr;
    Commands::sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (Commands::sockfd < 0) error("ERROR OPENING SOCKET");
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(atoi(argv[2]));
    inet_pton(AF_INET, argv[1], &serv_addr.sin_addr);
    if (connect(Commands::sockfd, (sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) error("ERROR CONNECTING");

    std::cout << "Welcome to our Volunteering App!\n";
    std::cout << "You are able to create and view events on your calendar and also and view the events that you are attending.\n";
    std::cout << "Please sign/log in to continue\n";
    std::cout << "Enter ""!help"" for commands\n";
    
    int x = 0;
    while (x < 1){
        std::string input;
        std::cout << "Enter a command\n";
        std::getline(std::cin, input);
        if (input == "!help"){
            get_commands();
        }
        else if (input.find("!login") != std::string::npos){
            std::string username = input.substr(input.find(" ", 0)+1,input.find(" ", 1)-2);
            std::string password = input.substr(input.find(username)+username.length()+1);
            Commands::logIn(username, password);
        } 
        else if (input.find("!signup") != std::string::npos) {
            std::string username = input.substr(input.find(" ", 0)+1,input.find(" ", 1)-2);
            std::string password = input.substr(input.find(username)+username.length()+1);
            Commands::signUp(username, password);
        }
        else if (input.find("!get_events") != std::string::npos) {
            std::vector<std::string> events = Commands::getEvents();
            for (std::string event : events) {

            }
        }
        else if (input.find("!create_event") != std::string::npos) {
            std::vector<std::string> event_data(5);
            std::cout << "Enter the name of the Volunteering Event: ";
            std::cin >> event_data[0];
            std::cout << "Enter the description of the Volunteering Event: ";
            std::cin >> event_data[1];
            std::cout << "Enter the location of the Volunteering Event: ";
            std::cin >> event_data[2];
            std::cout << "Enter the starting date of the Volunteering Event: ";
            std::cin >> event_data[3];
            std::cout << "Enter how long the Volunteering Event will last: ";
            std::cin >> event_data[4];
        }
        else if (input == "!exit"){
            x = 2;
            std::cout << "Exiting\n";
        }
        else
        {
            std::cout << "Not a valid command\n";
        }
    }
}   
