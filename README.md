# Chat Application

This is a simple chat application implemented in C++ for communication between two machines.

## Features

- Allows communication between two machines over a network.
- Supports text-based messaging.
- Uses TCP/IP sockets for communication.

## Prerequisites

Before running the client and server executables, please ensure that your system meets the following requirements:

- **Operating System Compatibility**: The executables are designed to run on Windows operating systems. Ensure that you are using a Windows-based system.

- **Microsoft Visual C++ Redistributable**: If not already installed, you may need to install the Microsoft Visual C++ Redistributable package corresponding to the version used for compiling the executables.

- **Winsock Library for Windows**: The Winsock library, which provides essential networking functionality on Windows, should be available on your system. It is typically included with modern versions of Windows.

- **Firewall Configuration**: Check your firewall settings to ensure that the executables are allowed to establish network connections. Make sure to configure the firewall to permit communication over the required ports.

- **Network Access**: Ensure that your system has network connectivity to the target server. If you are running the executables over a local network, ensure that the network configuration (IP addresses, ports, etc.) is correctly set up.

- **C++ Compiler (Optional)**: If you prefer to compile the source code yourself, you will need a C++ compiler such as g++.

If you encounter any difficulties running the executables or need further assistance, please refer to the troubleshooting section below or feel free to reach out to the project maintainers for support.

## Usage

1. Clone the repository:

   ```bash
    gh repo clone ilyassbaa/Chat-Application

2. Compile the source code:

   ```bash
    g++ server.cpp -o server.exe -lws2_32   # For Windows
    g++ client.cpp -o client.exe -lws2_32   # For Windows

3. Run the server:

   ```bash
    ./server.exe

4. Run the client on another machine:

   ```bash
    ./client.exe server_ip
 Replace server_ip with the IP address of the machine where the server is running.
 
