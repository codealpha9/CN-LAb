#include "common.h"
#define SERVER_IP "172.16.59.75"
#define SERVER_PORT 10200  

int main() {
    int clientSocket;
    struct sockaddr_in serverAddr;

    // Create socket
    clientSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (clientSocket == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Set up server address
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr(SERVER_IP);
    serverAddr.sin_port = htons(SERVER_PORT);

    char matrix[10][100]; // Adjust matrix dimensions as needed

    printf("Enter matrix rows:\n");
    for (int i = 0; i < 4; i++) {
        printf("Row %d: ", i + 1);
        fgets(matrix[i], sizeof(matrix[i]), stdin);

        sendto(clientSocket, matrix[i], strlen(matrix[i]), 0, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
    }

    printf("Matrix rows sent.\n");

    close(clientSocket);

    return 0;
}
