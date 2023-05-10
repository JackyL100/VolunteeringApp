//#include "Commands.hpp"
#include <client.hpp>
#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <unistd.h>

// compile command: g++ Commands.cpp client.cpp app_client.cpp -std=c++17 -o client.out

int main(int argc, char* argv[])
{
    if (argc < 3) {
        std::cout << "Hostname Port\n";
        exit(0);
    }
    std::cout << "Welcome to our Volunteering App!\n";
    std::cout << "You are able to create and view events on your calendar and also and view the events that you are attending.\n";
    std::cout << "Please sign/log in to continue\n";
    std::cout << "Enter ""!help"" for commands\n";
    
    bool loggedIn = false;
    std::vector<std::string> vec_input;

    app_client client(argv[1], atoi(argv[2]));

    while (true){
        std::string input;
        std::string token;
        std::cout << "Enter a command\n";
        std::getline(std::cin, input);
        std::stringstream ss(input);
        while (std::getline(ss, token, ' ')) {
            vec_input.push_back(token);
        }
        if (vec_input.size() == 0) {
            continue;
        }
        if (vec_input[0] == "!help"){
            client.help();
        }
        else if (!loggedIn && vec_input[0] == "!login"){
            if (vec_input.size() != 3) {
                continue;
            }
            loggedIn = client.logIn(vec_input[1], vec_input[2]);
        } 
        else if (!loggedIn && vec_input[0] == "!signup") {
            if (vec_input.size() != 3) {
                continue;
            }
            loggedIn = client.signUp(vec_input[1], vec_input[2]);
        }
        else if (vec_input[0] == "!get_events") {
            if (vec_input.size() != 2) {
                continue;
            }
            client.viewEvents(vec_input[1]);
        }
        else if (vec_input[0] == "!create_event") {
            client.createEvent();
        } 
        else if (vec_input[0] == "!join_event") {
            if (vec_input.size() == 1) {
                client.joinEvent(vec_input[1]);
            } 
            else {
                std::cout << "Event Name\n";
            } 
        }
        else if (vec_input[0] == "!exit"){
            break;
            std::cout << "Exiting\n";
        }
        else
        {
            std::cout << "Not a valid command\n";
        }
        vec_input.clear();
    }
}   
