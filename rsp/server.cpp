#include "server.hpp"

net::Server::Server(void* data)
{
    setup(data, DEFAULT_PORT);
}

net::Server::Server(void* data, int port)
{
    setup(data, port);
}

net::Server::Server(const Server& orig)
{
}

net::Server::~Server()
{
	std::cout << "[SERVER] [DESTRUCTOR] Destroying Server...\n";
	close(_listenFd);
}

// Create a socket and set up the address/port
// Don't modify _data
void 
net::Server::setup(void* data, int port)
{
    _data = data;
    
    // TODO: Complete
}

// Configure the socket to use the SO_REUSEADDR option.
// When a server shutdowns and restarts quickly, it needs to wait for some seconds before it could bind to the same addr/port.
// This is common in TCP servers.
void 
net::Server::initializeSocket()
{
	std::cout << "[SERVER] initializing socket\n";

	// TODO: Complete
	int optValue = 1;
	int retTest = -1;

	printf("[SERVER] setsockopt() ret %d\n", retTest);

	if (retTest < 0) {
        perror("[SERVER] [ERROR] setsockopt() failed");
		shutdown();
    }
}

// Bind the socket to address/port
void 
net::Server::bindSocket()
{
	// TODO: Complete
}

// Listen to incoming connections
void 
net::Server::startListen()
{
	// TODO: Complete
}

// Close the listening socket
void 
net::Server::shutdown()
{
	// TODO: Complete
}

// Accept incoming connections.
// You need to call "newConnectionCallback" once a connection is established.
void 
net::Server::handleNewConnection()
{
  	std::cout << "[SERVER] [CONNECTION] Waiting for a new connection\n";

    // TODO: Complete
}

// Handle incoming connections.
void 
net::Server::loop()
{
    // TODO: Complete
}

void 
net::Server::init()
{
    initializeSocket();
    bindSocket();
    startListen();
}

void 
net::Server::onConnect(void(*ncc)(Server*, uint16_t, void*))
{
    newConnectionCallback = ncc;
}