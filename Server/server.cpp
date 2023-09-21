#include "server.h"
#include <stdio.h>
#include <stdlib.h>
#include "server.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <vector>
#include <iostream>
#include <cstdlib>  
#include <ctime> 

#ifdef _WIN32
#include <winsock2.h>
using namespace std;
#else
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#endif

int serverSocket;

int initializeServer()
{
#ifdef _WIN32
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        perror("Winsock initialization failed");
        return -1;
    }
#endif 

#ifdef _WIN32
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
#else
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
#endif

    if (serverSocket == -1) {
        perror("Socket creation failed");
        return -1;
    }

    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(SERVER_PORT);

    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
        perror("Binding failed");
        return -1;
    }

    // Listen for incoming connections
    if (listen(serverSocket, 1) == -1) {
        perror("Listening failed");
        return -1;
    }

    return 0;
}

int acceptClient() {
    printf("Waiting for a client to connect...\n");
    struct sockaddr_in clientAddr;
    int addrLen = sizeof(clientAddr);


    int clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &addrLen);


    if (clientSocket == -1) {
        perror("Accepting client failed");
        return -1;
    }


    printf("Client connected.\n");
    return clientSocket;
}

std::vector<Processes>  receiveArrayFromClient(int clientSocket) {
    std::cout << "Waiting for data from the client...\n";

    size_t arraySize = 0;

    while (recv(clientSocket, reinterpret_cast<char*>(& arraySize), sizeof(arraySize), 0) <= 0) {

    }
    arraySize = ntohl(arraySize);
    std::cout << "Received array size: " << arraySize << "\n";

    std::vector<Processes> processesList;
    processesList.resize(arraySize);


  
    if (processesList.size() != arraySize) {
        std::cerr << "Error: Memory allocation failed\n";
        return processesList;
    }


    if (recv(clientSocket, reinterpret_cast<char*>(processesList.data()), arraySize * sizeof(Processes), 0) <= 0) {
        perror("Error receiving array from client");
        return processesList;
    }

    for (const auto& process : processesList) {
        std::cout << "Process ID: " << process.processesID << ", Command: " << process.command << "\n\n";

    }

    return processesList;
}


int getRandomProcessID(const std::vector<Processes>& processArray) {
    if (processArray.empty()) {
        std::cerr << "Error: Process array is empty." << std::endl;
        return -1;
    }
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    size_t randomIndex = std::rand() % processArray.size();
    return processArray[randomIndex].processesID;
}



void cleanupServer() {
  
#ifdef _WIN32
    closesocket(serverSocket);
    WSACleanup(); 
#else
    close(serverSocket);
#endif
}