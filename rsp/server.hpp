#ifndef SERVER_HPP
#define SERVER_HPP


#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <iostream>

#include "common.hpp"



namespace net {

    class Server
    {
    public:
        Server(void* data);
        Server(void* data, int port);
        Server(const Server& orig);
        virtual ~Server();
        
        void shutdown();
        void init();
        void loop();

        //callback setters
        void onConnect(void (*ncc)(Server* server, uint16_t fd, void* data));

    private:

        void* _data;

        //socket file descriptors
        int _listenFd;
        int _connFd;

        //client connection data
        struct sockaddr_storage _clientAddr;
        
        //server socket details
        struct sockaddr_in _servAddr;
        
        void (*newConnectionCallback) (Server* server, uint16_t fd, void*);

        //function prototypes
        void setup(void* data, int port);
        void initializeSocket();
        void bindSocket();
        void startListen();
        void handleNewConnection();
    };
}
#endif /* SERVER_HPP */
