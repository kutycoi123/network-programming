#include <iostream>
#include <string>
#include <getopt.h>

#include "server_handler.hpp"

void 
showHelp(std::string name, std::string msg) {

    if(!msg.empty())
        std::cerr << msg << std::endl << std::endl;
    std::cerr << std::endl << "Usage: " << name << " " << "-p port"
              << std::endl;
}

void
parseCommandLine(int argc, char** argv, int* port) {
    int optionIndex = 0;
    while((optionIndex = getopt(argc, argv, ":p:")) != -1) {
        switch(optionIndex) {
            case 'p':
                *port = atoi(optarg);
                break;
            case ':':
            case '?':
            default:
                *port = -1;
                showHelp(argv[0], "");
                break;
        }
    }
}

int main(int argc, char** argv) {
    int port = 0;
    parseCommandLine(argc, argv, &port);
    
    if (port == -1) {
        return 1;
    }

    if (port == 0) {
        port = DEFAULT_PORT;
    }

    model::SetManager sm;
    net::Server* s = new net::Server((void*)&sm, port);
    s->onConnect(net::OnNewClientConnected);
    s->init();
    s->loop();
}