#ifndef SERVER_HANDLER_HPP
#define SERVER_HANDLER_HPP

#include <iostream>

#include "request.hpp"
#include "response.hpp"
#include "server.hpp"
#include "net.hpp"
#include "set.hpp"

using namespace std;

namespace net {
    void OnNewClientConnected(net::Server* server, uint16_t fd, void* data);
}

#endif /* SERVER_HANDLER_HPP */
