#include <iostream>
#include <string>
#include <map>
#include <getopt.h>

#include "client_handler.hpp"

using namespace std;

std::map<std::string, char>
initCmdMap() {
    static std::pair<std::string, char> mapData[] = {
        std::make_pair("create_set", OPT_CRT_SET),
        std::make_pair("add_item", OPT_ADD_ITEM),
        std::make_pair("rem_item", OPT_REM_ITEM),
        std::make_pair("get_items", OPT_GET_ITEMS),
        std::make_pair("clear_set", OPT_CLR_SET),
        std::make_pair("rem_set", OPT_REM_SET),
        std::make_pair("get_size", OPT_GET_SIZE),
        std::make_pair("get_sets", OPT_GET_SETS)
    };
    return std::map<std::string, char>(mapData, mapData+sizeof(mapData)/sizeof(*mapData));
}

void 
showHelp(std::string name, std::map<std::string, char> opt_map, std::string msg) {
    std::string commands = "";
    for(std::map<std::string, char>::iterator it=opt_map.begin(); it != opt_map.end(); ++it) {
        commands += it->first;
        if(++it != opt_map.end()) {
            commands += ", ";
        }
        --it;
    }
    if(!msg.empty())
        std::cerr << msg << std::endl << std::endl;
    std::cerr << std::endl << "Usage: " << name << " " << "-s server -p port -c command -t set_id -i item"
              << std::endl
              << "       Available commands: " << commands
              << std::endl
              << "       Set ID: a 32-byte ID"
              << std::endl
              << "       Item: integer values only"
              << std::endl;
}

model::Request*
make_request(std::map<std::string, char> opt_map, std::string name, const char* cmd, const char* setId, const char* item) {
    std::map<std::string, char>::iterator it = opt_map.find(cmd);
    if(it == opt_map.end()) {
        showHelp(name, opt_map, "This command is not supported");
        return nullptr;
    }

    char opt = it->second;
    if(opt != OPT_CRT_SET && opt != OPT_GET_SETS) {
        if(strlen(setId) == 0) {
            showHelp(name, opt_map, "This command requires a set ID");
            return nullptr;
        }
        if(opt == OPT_ADD_ITEM || opt == OPT_REM_ITEM) {
            if(strlen(item) == 0) {
                showHelp(name, opt_map, "This command requires a set ID and an Item");
                return nullptr;
            }
        }
    }
    
    return new model::Request(opt, setId, atoi(item));
}

void
parseCommandLine(int argc, char** argv, std::map<std::string, char> opt_map, 
std::string & server, int* port, std::string & cmd, std::string & setId, std::string & item) {
    int optionIndex = 0;
    while((optionIndex = getopt(argc, argv, ":s:p:c:t:i:")) != -1) {
        switch(optionIndex) {
            case 's':
                server = optarg;
                break;
            case 'p':
                *port = atoi(optarg);
                break;
            case 'c':
                cmd = optarg;
                break;
            case 't':
                setId = optarg;
                break;
            case 'i':
                item = optarg;
                break;
            case ':':
            case '?':
            default:
                showHelp(argv[0], opt_map, "");
                break;
        }
    }
}

int main(int argc, char** argv) {
    std::map<std::string, char> opt_map = initCmdMap();
    std::string server;
    int port = -1;
    std::string cmd;
    std::string setId;
    std::string item;
    parseCommandLine(argc, argv, opt_map, server, &port, cmd, setId, item);
    
    if(server.empty()) {
        server = "127.0.0.1";
    } if (port < 0) {
        port = DEFAULT_PORT;
    }

    std::cout << "[CLIENT] Using address: tcp://" << server << ":" << port << std::endl; 

    model::Request* req = make_request(opt_map, argv[0], cmd.c_str(), setId.c_str(), item.c_str());
    if(!req) {
        return 1;
    }

    net::Client* c = new net::Client(server.c_str(), port, (void*)req);
    c->onConnect(net::OnClientConnected);
    c->init();
}