#ifndef CLIENT_HANDLER_HPP
#define CLIENT_HANDLER_HPP

#include <iostream>

#include "request.hpp"
#include "response.hpp"
#include "common.hpp"
#include "net.hpp"
#include "client.hpp"

using namespace std;

namespace net {
    void printResponse(model::Response* res);
    char* readResponseBytes(uint16_t fd);
    void OnClientConnected(net::Client* client, uint16_t fd, void* data);
}

#endif /* CLIENT_HANDLER_HPP */
