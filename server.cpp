#include <iostream>
#include <thread>
#include <algorithm> 
#include <vector>    
#include <cstring>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "Ws2_32.lib")

#define PORT 12345

// Function to handle communication with a connected client
void handle_client(SOCKET client_socket) {
   char client_name[256];
    int name_len = recv(client_socket, client_name, sizeof(client_name), 0);
    if (name_len > 0) {
        client_name[name_len] = '\0'; 
        std::cout << "Client connected: " << client_name << std::endl;
    }

    // Buffer to store received messages
    char buffer[1024];
    int bytes_received;
    
    // Continuously receive messages from the client
    while ((bytes_received = recv(client_socket, buffer, sizeof(buffer), 0)) > 0) {
        buffer[bytes_received] = '\0';
        std::cout << client_name << ": " << buffer << std::endl;
    }
    // Close the client socket when done
    closesocket(client_socket);
}

int main() {
    // Initialize Winsock
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed." << std::endl;
        return 1;
    }

    // Create a socket for the server
    SOCKET server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == INVALID_SOCKET) {
        std::cerr << "Socket creation failed." << std::endl;
        WSACleanup();
        return 1;
    }

    // Configure server address structure
    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT); // Server will listen on port
    server_addr.sin_addr.s_addr = INADDR_ANY; // Listen on any available network interface

    // Bind the socket to the configured address and port
    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        std::cerr << "Binding socket failed." << std::endl;
        closesocket(server_socket);
        WSACleanup();
        return 1;
    }
    
    // Start listening for incoming connection requests
    if (listen(server_socket, SOMAXCONN) == SOCKET_ERROR) {
        std::cerr << "Listening on socket failed." << std::endl;
        closesocket(server_socket);
        WSACleanup();
        return 1;
    }

    std::cout << "Server listening on port " << PORT << std::endl;

// Accept client connections
    SOCKET client_socket;
    sockaddr_in client_addr;
    int client_addr_size = sizeof(client_addr);

    // Main server loop to accept and handle client connections
    while ((client_socket = accept(server_socket, (sockaddr*)&client_addr, &client_addr_size)) != INVALID_SOCKET) {
        handle_client(client_socket);
    }

    // Cleanup and close the server socket
    closesocket(server_socket);
    WSACleanup();
    return 0;
}
