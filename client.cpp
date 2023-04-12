#include <iostream>
#include "commands.hpp"
using namespace std;

void commands(){
    std::cout << "!login username password\n";
    std::cout << "!signup username password\n";
    std::cout << "!exit\n";
    return;
}

int main(int argc, char* argv[])
{
    // if (argc < 3) {
    //     std::cout << "Hostname Port\n";
    //     exit(0);
    // }

    std::cout << "Welcome to our Volunteering App!\n";
    std::cout << "You are able to create and view events on your calendar and also and view the events that you are attending.\n";
    std::cout << "Please sign/log in to continue\n";
    std::cout << "Enter ""!help"" for commands\n";
    
    int x = 0;
    while (x < 1){
        string input;
        cout << "Enter a command\n";
        getline(cin, input);
        if (input == "!help"){
            commands();
        }
        else if (input.find("!login") != string::npos){
            string username = input.substr(input.find(" ", 0)+1,input.find(" ", 1)-2);
            string password = input.substr(input.find(username)+username.length()+1);
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
