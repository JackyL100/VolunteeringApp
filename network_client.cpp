#include "network_client.hpp"

network_client::network_client(char* hostname, int port) {
    std::cout << "Creating network client\n";
    struct sockaddr_in serv_addr;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) error("ERROR OPENING SOCKET");
    bzero(&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    inet_pton(AF_INET, hostname, &serv_addr.sin_addr);
    if (connect(sockfd, (sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) error("ERROR CONNECTING");
}

network_client::~network_client() {
    close(sockfd);
}

void network_client::addToWriteBuffer(const std::string& message) {
    memcpy(&write_buf, message.c_str(), message.size());
    write_buf_size = message.size();
    std::cout << "Added: " << write_buf << " to write_buf\n";
}

void network_client::error(const char* msg) {
    perror(msg);
    exit(1);
}

std::string network_client::getBuffer(bool clear = true) {
    read_buf_size = 0;
    if (clear) {
        std::string str(read_buf);
        memset(read_buf, 0, 4 + MAX_MSG_SIZE);
        return str;
    } else {
        return std::string(read_buf);
    }
}

void network_client::receiveSmall() {
   /*while (true) {
        // try to fill buffer
        assert(read_buf_size < sizeof(read_buf));
        int rv = 0;
        do {
            int cap = sizeof(read_buf) - read_buf_size;
            rv = recv(sockfd, &read_buf[read_buf_size], cap ,0);
        } while (rv < 0 && errno == EINTR);
        if (rv < 0 && errno == EAGAIN) {
            break;
        }
        if (rv < 0) {
            break;
        }
        if (rv == 0) {
            if (read_buf_size > 0) {
                std::cout << "unexpected EOF\n";
            } else {
                std::cout << "EOF\n";
            }
            break;
        }
        read_buf_size += rv;
        assert(read_buf_size <= sizeof(read_buf) - read_buf_size);
    }*/
    int rv = recv(sockfd, read_buf, 4096, 0);
    if (rv <= 0) {
        std::cout << "Error Receiving\n";
    }
    read_buf_size += rv;
}

void network_client::transmit() {
    /*
    while(true) {
        // try to flush buffer
        int rv = 0;
        do {
            int remain = write_buf_size - write_buf_sent;
            rv = send(sockfd, &write_buf[write_buf_sent], remain, 0);
        } while(rv < 0 && errno == EINTR);
        if (rv < 0 && errno == EAGAIN) {
            break;
        }
        if (rv < 0) {
            break;
        }
        write_buf_sent += rv;
        std::cout << write_buf_sent << "\n";
        assert(write_buf_sent <= write_buf_size);
        if (write_buf_sent == write_buf_size) {
            write_buf_sent = 0;
            write_buf_size = 0;
            break;
        }
    }
    memset(write_buf, 0, 4 + MAX_MSG_SIZE);*/
    int n = write_buf_size;
    while(n > 0) {
        ssize_t rv = write(sockfd, write_buf, n);
        if (rv <= 0) {
            std::cout << "Error sending\n";
            break;
        }
        std::cout << rv << "\n";
        assert((ssize_t)rv <= n);
        n -= (size_t)rv;
        //write_buf += rv;
    }
}