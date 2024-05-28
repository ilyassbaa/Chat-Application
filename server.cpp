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

void gerer_client(SOCKET client_socket) {
    char buffer[1024];
    while (true) {
        memset(buffer, 0, 1024);
        int bytes_received = recv(client_socket, buffer, 1024, 0);
        if (bytes_received <= 0) {
            closesocket(client_socket);
            clients.erase(std::remove(clients.begin(), clients.end(), client_socket), clients.end());
            break;
        } else {
            std::cout << "Message reçu: " << buffer << std::endl;
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
        std::cerr << "Échec de l'initialisation de Winsock." << std::endl;
        return 1;
    }

    SOCKET serveur_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (serveur_socket == INVALID_SOCKET) {
        std::cerr << "Impossible de créer le socket." << std::endl;
        WSACleanup();
        return 1;
    }

    sockaddr_in adresse_serveur;
    adresse_serveur.sin_family = AF_INET;
    adresse_serveur.sin_port = htons(PORT);
    adresse_serveur.sin_addr.s_addr = INADDR_ANY;

    if (bind(serveur_socket, (struct sockaddr*)&adresse_serveur, sizeof(adresse_serveur)) == SOCKET_ERROR) {
        std::cerr << "Échec du bind." << std::endl;
        closesocket(serveur_socket);
        WSACleanup();
        return 1;
    }

    if (listen(serveur_socket, 10) == SOCKET_ERROR) {
        std::cerr << "Échec de l'écoute." << std::endl;
        closesocket(serveur_socket);
        WSACleanup();
        return 1;
    }

    std::cout << "Serveur en écoute sur le port " << PORT << std::endl;

    while (true) {
        sockaddr_in adresse_client;
        int taille_client = sizeof(adresse_client);
        SOCKET client_socket = accept(serveur_socket, (struct sockaddr*)&adresse_client, &taille_client);
        if (client_socket == INVALID_SOCKET) {
            std::cerr << "Échec de l'acceptation." << std::endl;
            continue;
        }

        clients.push_back(client_socket);
        std::thread(gerer_client, client_socket).detach();
    }

    closesocket(serveur_socket);
    WSACleanup();
    return 0;
}
