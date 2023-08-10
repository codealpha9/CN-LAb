#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <time.h>

void handle_client(int client_socket) {
    char buffer[256];
    time_t current_time;
    struct tm *time_info;
    pid_t process_id;

    // Get current time and process ID
    time(&current_time);
    time_info = localtime(&current_time);
    process_id = getpid();

    // Format the time and process ID into a string
    snprintf(buffer, sizeof(buffer), "Time: %sProcess ID: %d\n", asctime(time_info), process_id);

    // Send the formatted data to the client
    write(client_socket, buffer, strlen(buffer));

    // Close the client socket
    close(client_socket);
}

int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);

    // Create socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("Socket creation failed");
        return 1;
    }

    // Configure server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(10200);

    // Bind the socket
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Binding failed");
        return 1;
    }

    // Listen for incoming connections
    if (listen(server_socket, 5) == -1) {
        perror("Listening failed");
        return 1;
    }

    printf("Server is listening for incoming connections...\n");

    while (1) {
        // Accept incoming connection
        client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_len);
        if (client_socket == -1) {
            perror("Accepting connection failed");
            continue;
        }

        // Create a child process to handle the client
        if (fork() == 0) {
            // Child process
            close(server_socket); // Close the parent's listening socket
            handle_client(client_socket); // Handle the client
            exit(0); // Exit child process
        } else {
            // Parent process
            close(client_socket); // Close the client socket in the parent process
        }
    }

    close(server_socket);
    return 0;
}
