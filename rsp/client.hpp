#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h> //sockaddr, socklen_t
#include <arpa/inet.h>
#include <netdb.h>
#include <iostream>

#include "common.hpp"

namespace net {

    class Client
    {
    public:
        Client(const char* server_addr, void* data);
        Client(const char* server_addr, int port, void* data);
        Client(const Client& orig);
        virtual ~Client();
        
        void init();
        void shutdown();

        //callback setters
        void onConnect(void (*ncc)(Client* client, uint16_t fd, void* data));

    private:
        void* _data;
        int _connFd;

        struct sockaddr_in _servAddr;

        void (*newConnectionCallback) (Client* server, uint16_t fd, void* data);
        void (*disconnectCallback) (uint16_t fd);

        void setup(const char*, int port, void* data);
        void initializeSocket();
        void doConnect();
    };
}
#endif /* CLIENT_HPP */
