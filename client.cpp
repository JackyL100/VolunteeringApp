#include <iostream>

int main(int argc, char* argv[])
{
    if (argc < 3) {
        std::cout << "Hostname Port\n";
        exit(0);
    }
    std::cout << "Welcome to our Volunteering App!\n";
    std::cout << "You are able to create and view events on your calendar and also and view the events that you are attending.\n";
    std::cout << "Please sign/log in to continue";
    

}