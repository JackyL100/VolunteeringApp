#ifndef NETWORK_CLIENT_HPP
#define NETWORK_CLIENT_HPP

#include <string>
#include <vector>
#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <unistd.h>

const int MAX_MSG_SIZE = 4096;

class network_client {
    private:
        int sockfd;
        int read_buf_size = 0;
        char read_buf[4 + MAX_MSG_SIZE];

        int write_buf_size = 0;
        int write_buf_sent = 0;
        char write_buf[4 + MAX_MSG_SIZE];
        void error(const char* msg);
    public:
        network_client(char* hostname, int port);
        ~network_client();
        void addToWriteBuffer(const std::string& message);
        std::string getBuffer(bool clear);
        void receive();
        void transmit();
};

#endif