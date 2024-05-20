#include <iostream>
#include <thread>
#include <algorithm> 
#include <vector>    
#include <cstring>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "Ws2_32.lib")

#define PORT 12345

std::vector<SOCKET> clients;

void handle_client(SOCKET client_socket) {
    char buffer[1024];
    while (true) {
        memset(buffer, 0, 1024);
        int bytes_received = recv(client_socket, buffer, 1024, 0);
        if (bytes_received <= 0) {
            closesocket(client_socket);
            clients.erase(std::remove(clients.begin(), clients.end(), client_socket), clients.end());
            break;
        } else {
            std::cout << "Message recu: " << buffer << std::endl;
            for (SOCKET client : clients) {
                if (client != client_socket) {
                    send(client, buffer, bytes_received, 0);
                }
            }
        }
    }
}

int main() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "Echec de l'initialisation de Winsock." << std::endl;
        return 1;
    }

    SOCKET server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == INVALID_SOCKET) {
        std::cerr << "Impossible de creer le socket." << std::endl;
        WSACleanup();
        return 1;
    }

    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        std::cerr << "Bind echoue." << std::endl;
        closesocket(server_socket);
        WSACleanup();
        return 1;
    }

    if (listen(server_socket, 10) == SOCKET_ERROR) {
        std::cerr << "ecoute echouee." << std::endl;
        closesocket(server_socket);
        WSACleanup();
        return 1;
    }

    std::cout << "Serveur en ecoute sur le port " << PORT << std::endl;

    while (true) {
        sockaddr_in client_addr;
        int client_size = sizeof(client_addr);
        SOCKET client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_size);
        if (client_socket == INVALID_SOCKET) {
            std::cerr << "Accept echoue." << std::endl;
            continue;
        }

        clients.push_back(client_socket);
        std::thread(handle_client, client_socket).detach();
    }

    closesocket(server_socket);
    WSACleanup();
    return 0;
}
