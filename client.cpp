#include <iostream>
#include <thread>
#include <cstring>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "Ws2_32.lib")

#define PORT 12345

void receive_messages(SOCKET client_socket) {
    char buffer[1024];
    while (true) {
        memset(buffer, 0, 1024);
        int bytes_received = recv(client_socket, buffer, 1024, 0);
        if (bytes_received <= 0) {
            std::cerr << "Connexion au serveur perdue." << std::endl;
            closesocket(client_socket);
            exit(1);
        } else {
            std::cout << "Message recu: " << buffer << std::endl;
        }
    }
}

int main() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "Echec de l'initialisation de Winsock." << std::endl;
        return 1;
    }

    SOCKET client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == INVALID_SOCKET) {
        std::cerr << "Impossible de creer le socket." << std::endl;
        WSACleanup();
        return 1;
    }

    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);  // Replace with server IP address if necessary

    if (connect(client_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        std::cerr << "Connexion au serveur echouee." << std::endl;
        closesocket(client_socket);
        WSACleanup();
        return 1;
    }

    std::cout << "Connecte au serveur." << std::endl;

    std::thread(receive_messages, client_socket).detach();

    char buffer[1024];
    while (true) {
        std::cin.getline(buffer, 1024);
        if (send(client_socket, buffer, strlen(buffer), 0) == SOCKET_ERROR) {
            std::cerr << "Envoi du message echoue." << std::endl;
            break;
        }
    }

    closesocket(client_socket);
    WSACleanup();
    return 0;
}
