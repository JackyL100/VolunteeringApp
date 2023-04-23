#ifndef CONNECTION_HPP
#define CONNECTION_HPP

const int MAX_MSG_SIZE = 4096;


const int STATE_REQ = 0;
const int STATE_RES = 1;
const int STATE_END = 2;

class Connection {
    public:
        int sockfd = -1;
        int state = 0;

        int read_buf_size = 0;
        char read_buf[4 + MAX_MSG_SIZE];

        int write_buf_size = 0;
        int write_buf_sent = 0;
        char write_buf[4 + MAX_MSG_SIZE];

        Connection(int fd) {
            sockfd = fd;
            state = STATE_REQ;
            read_buf_size = 0;
            write_buf_size = 0;
            write_buf_sent = 0;
        }
};

#endif