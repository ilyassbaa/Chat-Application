#include <iostream>
#include <thread>
#include <cstring>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "Ws2_32.lib")

#define PORT 12345

void recevoir_messages(SOCKET client_socket) {
    char buffer[1024];
    while (true) {
        memset(buffer, 0, 1024);
        int bytes_received = recv(client_socket, buffer, 1024, 0);
        if (bytes_received <= 0) {
            std::cerr << "Connexion au serveur perdue." << std::endl;
            closesocket(client_socket);
            exit(1);
        } else {
            std::cout << "Message reçu: " << buffer << std::endl;
        }
    }
}

int main() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "Échec de l'initialisation de Winsock." << std::endl;
        return 1;
    }

    SOCKET client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == INVALID_SOCKET) {
        std::cerr << "Impossible de créer le socket." << std::endl;
        WSACleanup();
        return 1;
    }

    sockaddr_in adresse_serveur;
    adresse_serveur.sin_family = AF_INET;
    adresse_serveur.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &adresse_serveur.sin_addr);  // Remplacez par l'adresse IP du serveur si nécessaire

    if (connect(client_socket, (struct sockaddr*)&adresse_serveur, sizeof(adresse_serveur)) == SOCKET_ERROR) {
        std::cerr << "Connexion au serveur échouée." << std::endl;
        closesocket(client_socket);
        WSACleanup();
        return 1;
    }

    std::cout << "Connecté au serveur." << std::endl;

    std::thread(recevoir_messages, client_socket).detach();

    char buffer[1024];
    while (true) {
        std::cin.getline(buffer, 1024);
        if (send(client_socket, buffer, strlen(buffer), 0) == SOCKET_ERROR) {
            std::cerr << "Envoi du message échoué." << std::endl;
            break;
        }
    }

    closesocket(client_socket);
    WSACleanup();
    return 0;
}
