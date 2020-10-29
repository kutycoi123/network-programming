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
    _servAddr.sin_port = htons(port);
    _servAddr.sin_family = AF_INET;
    _servAddr.sin_addr.s_addr = INADDR_ANY;
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
  if ((_listenFd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
    perror("Socket creation failed\n");
    shutdown();
  }
  retTest = setsockopt(_listenFd, SOL_SOCKET, SO_REUSEADDR, &optValue, sizeof(optValue));
	printf("[SERVER] setsockopt() ret %d\n", retTest);

	if (retTest < 0) {
      perror("[SERVER] [ERROR] setsockopt() failed\n");
		  shutdown();
  }
}

// Bind the socket to address/port
void 
net::Server::bindSocket()
{
	// TODO: Complete
  if (bind(_listenFd, (struct sockaddr *)&_servAddr, sizeof(_servAddr)) < 0) {
    perror("Bind failed\n");
    shutdown();
  }
}

// Listen to incoming connections
void 
net::Server::startListen()
{
	// TODO: Complete
  if (listen(_listenFd, 10) < 0) {
    perror("Listen failed\n");
    shutdown();
  }
}

// Close the listening socket
void 
net::Server::shutdown()
{
	// TODO: Complete
  close(_listenFd);
}

// Accept incoming connections.
// You need to call "newConnectionCallback" once a connection is established.
void 
net::Server::handleNewConnection()
{
  	std::cout << "[SERVER] [CONNECTION] Waiting for a new connection\n";
    int addrlen = sizeof(_servAddr);  
    // TODO: Complete
    if ((_connFd = accept(_listenFd, (struct sockaddr *)&_servAddr, (socklen_t*)&addrlen)) < 0) {
      perror("Accept failed\n");
      shutdown();
      exit(EXIT_FAILURE);
    }
    newConnectionCallback(this, _connFd, _data);
}

// Handle incoming connections.
void 
net::Server::loop()
{
    // TODO: Complete
    while (1) {
      handleNewConnection();
    }
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
