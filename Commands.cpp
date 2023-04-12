#include "commands.hpp"
#include "user_profile.hpp"
#include "server.hpp"
using namespace std;

logIn(string username, string password){
    UserProfile(username, password);
}

signUp(string username, string password){
    UserProfile(username, password);
    
}