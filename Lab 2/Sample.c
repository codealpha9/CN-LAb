#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h> // Added for close() and fork()
#include <arpa/inet.h> // Added for inet_addr()

#define PORTNO 10200

int main() {
    int sockfd, newsockfd, portno, clilen, n = 1;
    char buf[256];
    struct sockaddr_in seraddr, cliaddr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("Socket creation failed");
        return 1;
    }

    seraddr.sin_family = AF_INET;
    seraddr.sin_addr.s_addr = inet_addr("172.16.59.10");
    seraddr.sin_port = htons(PORTNO);

    if (bind(sockfd, (struct sockaddr *)&seraddr, sizeof(seraddr)) == -1) {
        perror("Binding failed");
        return 1;
    }

    // Create a connection queue, ignore child exit details, and wait for clients
    if (listen(sockfd, 5) == -1) {
        perror("Listening failed");
        return 1;
    }

    while (1) {
        // Accept the connection
        clilen = sizeof(cliaddr);
        newsockfd = accept(sockfd, (struct sockaddr *)&cliaddr, &clilen);
        if (newsockfd == -1) {
            perror("Acceptance failed");
            continue; // Continue to the next iteration
        }

        // Fork to create a process for this client
        if (fork() == 0) {
            // If you’re the child, you can now read/write to the client on newsockfd.
            close(sockfd); // Close listening socket in child
            n = read(newsockfd, buf, sizeof(buf));
            if (n <= 0) {
                perror("Read error or connection closed");
            } else {
                printf("\nMessage from Client: %s\n", buf);
                n = write(newsockfd, buf, n); // Writing n bytes back
                if (n == -1) {
                    perror("Write error");
                }
            }
            close(newsockfd);
            return 0; // End the child process
        } else {
            // You must be the parent and your work for this client is finished
            close(newsockfd); // Close new connection socket in parent
        }
    }

    close(sockfd);
    return 0;
}
