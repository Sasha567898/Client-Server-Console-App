#include "client.h"
#include <cstdio>
#include <cstring>

#ifdef _WIN32
#include <Winsock2.h>
#include <Ws2tcpip.h>
#else
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#endif

Client::Client() {
#ifdef _WIN32
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        perror("Winsock initialization failed");
        exit(EXIT_FAILURE);
    }
#endif
    client_socket = -1;
}

Client::~Client() {
    closeConnection();
#ifdef _WIN32
    WSACleanup();
#endif
}

int Client::initializeClientSocket() {
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        perror("Socket creation failed");
        return -1;
    }
    return 0;
}



int Client::connectToServer(const char* server_ip, int server_port) {
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(server_port);

    // Resolve the server IP address
    
    #ifdef _WIN32
     server_addr.sin_addr.s_addr = inet_addr(server_ip);
    #else
    inet_pton(AF_INET, server_ip, &(server_addr.sin_addr));
    #endif

    


    if (connect(client_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("Connection failed");
        return -1;
    }

    return 0;
}


void Client::closeConnection() {
    if (client_socket != -1) {
#ifdef _WIN32
        closesocket(client_socket);
#else
        close(client_socket);
#endif
        client_socket = -1;
    }
}