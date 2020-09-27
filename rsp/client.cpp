#include "client.hpp"

net::Client::Client(const char* server_addr, void* data)
{
    setup(server_addr, DEFAULT_PORT, data);
}

net::Client::Client(const char* server_addr, int port, void* data)
{
    setup(server_addr, port, data);
}

net::Client::Client(const Client& orig)
{
}

net::Client::~Client()
{
	std::cout << "[CLIENT] [DESTRUCTOR] Destroying Client...\n";
	close(_connFd);
}

// Create a socket and sets the server address
// Don't modify _data
void 
net::Client::setup(const char* server_addr, int port, void* data)
{
    _data = data;
    // TODO: Complete
}

void 
net::Client::initializeSocket()
{
	std::cout << "[CLIENT] initializing socket\n";
}

// Shutdown the socket
void 
net::Client::shutdown()
{
	// TODO: Complete
}

// Connect to the server. You need to call "newConnectionCallback" once the connection is established
void 
net::Client::doConnect()
{
    // TODO: Complete
}

void 
net::Client::init()
{
    initializeSocket();
    doConnect();
}

void 
net::Client::onConnect(void(*ncc)(Client*, uint16_t, void*))
{
    newConnectionCallback = ncc;
}