#ifndef CLIENT_H
#define CLIENT_H
#ifdef _WIN32
#include <winsock2.h>
#else
#include <netinet/in.h>
#endif
class Client {
public:
    Client();
    ~Client();
    int initializeClientSocket();
    int connectToServer(const char* server_ip, int server_port);
    int sendMessage(const char* message);
    void closeConnection();

    int client_socket;
};

#endif // CLIENT_H