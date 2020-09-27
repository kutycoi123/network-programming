#ifndef NET_HPP
#define NET_HPP

#include <iostream>

#include "request.hpp"
#include "response.hpp"
#include "common.hpp"
#include "client.hpp"
#include "server.hpp"
#include "set.hpp"

using namespace std;

namespace net {
    uint16_t SendBytes(uint16_t fd, const char *messageBuffer, int count);
    uint16_t SendBytes(uint16_t fd, char *messageBuffer, int count);
    int ReadBytes(uint16_t fd, char* buffer, unsigned int n);
}

#endif /* NET_HPP */
