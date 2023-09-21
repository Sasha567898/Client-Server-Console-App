#ifndef SERVER_H
#define SERVER_H

#include <string>
#include <vector>
#ifdef _WIN32
#include <winsock2.h>
#else
#include <netinet/in.h>
#endif



#define SERVER_PORT 16666

struct Processes{
int processesID;
char command [256];
};

int initializeServer();
int acceptClient();
void handleClient(int clientSocket);
std::vector <Processes> receiveArrayFromClient(int clientSocket);
int getRandomProcessID(const std::vector<Processes>& processArray);
void cleanupServer();

#endif