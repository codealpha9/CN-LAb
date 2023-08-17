#include "common.h"
#define MAX_ROWS 4
#define ROW_SIZE 100

int main() {
    int serverSocket;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t addrLen = sizeof(clientAddr);

    char matrix[MAX_ROWS][ROW_SIZE];
    int received_rows = 0;

    // Create socket
    serverSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (serverSocket == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Set up server address
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(10200); // Choose a suitable port

    // Bind socket to address
    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
        perror("Bind failed");
        close(serverSocket); // Close the socket before exiting
        exit(EXIT_FAILURE);
    }

    printf("Server waiting for matrix rows...\n");

    while (received_rows < MAX_ROWS) {
        char buffer[ROW_SIZE];
        ssize_t recvSize = recvfrom(serverSocket, buffer, sizeof(buffer), 0, (struct sockaddr*)&clientAddr, &addrLen);
        if (recvSize == -1) {
            perror("Receive error");
            continue;
        }

        strncpy(matrix[received_rows], buffer, ROW_SIZE);
        matrix[received_rows][recvSize] = '\0'; // Null-terminate the received row
        received_rows++;

        printf("Received row %d\n", received_rows);
    }

    printf("Matrix received from clients:\n");
    for (int i = 0; i < MAX_ROWS; i++) {
        printf("%s\n", matrix[i]);
    }

    close(serverSocket);

    return 0;
}
