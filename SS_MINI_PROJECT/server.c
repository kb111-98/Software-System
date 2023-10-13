#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "admin.h"

#define PORT 8080
#define BUFFER_SIZE 1024

void handleClient(int client_socket);

int main() {
    int server_socket, client_socket;
    struct sockaddr_in serverAddress, clientAddress;
    socklen_t clientAddrLen = sizeof(clientAddress);

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("Socket creation failed");
        exit(1);
    }

	printf("Socket created successfully..\n");


    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(PORT);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_socket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) == -1) {
        perror("Socket binding failed");
        exit(1);
    }

	printf("Socket bind successfully..\n");


    if (listen(server_socket, 5) == -1) {
        perror("Listening failed");
        exit(1);
    }

    printf("Server is listening on port %d...\n", PORT);

    while (1) {
        client_socket = accept(server_socket, (struct sockaddr *)&clientAddress, &clientAddrLen);
        if (client_socket == -1) {
            perror("Accepting connection failed");
            continue;
        }

        pid_t childPID = fork();
        if (childPID < 0) {
            perror("Fork failed");
        } else if (childPID == 0) {
            // This is the child process
            close(server_socket); // Close the server socket in the child process
            handleClient(client_socket);
            close(client_socket);
            exit(0); // Terminate the child process
        } else {
            // This is the parent process
            close(client_socket);    // Close the client socket in the parent process
            waitpid(-1, NULL, WNOHANG); // Cleanup zombie child processes
        }
    }

    // Close the server socket (never reached in this example)
    close(server_socket);
    return 0;
}

void handleClient(int client_socket) {
    char buffer[BUFFER_SIZE];
    int bytesRead;

    // Prompt the client to choose a role
    char role_Menu[] = "\nSelect your role:\n1. Admin\n2. Faculty\n3. Student\n4. Exit the client\n";
    while (1) {
        memset(buffer, 0, sizeof(buffer));
        send(client_socket, role_Menu, strlen(role_Menu), 0);

        // Receive the client's role choice
        bytesRead = recv(client_socket, buffer, sizeof(buffer), 0);
        if (bytesRead <= 0) {
            perror("Error receiving role choice");
            close(client_socket);
            return;
        }
		int ch = atoi(buffer);
        

        // Process the client's role choice using a switch statement
        switch (ch) {
            case 1:
                // Handle Admin role
                // Implement the admin-related functionality here
				if (admin_Authentication(client_socket)) {
        			admin_Fun(client_socket);
    			} else {
        			send(client_socket, "Admin authentication failed. Exiting.\n", strlen("Admin authentication failed. Exiting.\n"), 0);
       				close(client_socket);
       				return;
    			}
                break;
            case 2:
                // Handle Professor role
                // Implement the professor-related functionality here
                break;
            case 3:
                // Handle Student role
                // Implement the student-related functionality here
                break;
            case 4:

                break;
            default:
                char response4[] = "You are Exiting Now !!.\n";
                send(client_socket, response4, strlen(response4), 0);
                close(client_socket);
                return;
        }
    }
    // Close the client socket
    close(client_socket);
}

