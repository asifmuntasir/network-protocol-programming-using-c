#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define SERVER_IP "127.0.0.1"
#define PORT 8080
#define BUFFER_SIZE 1024

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <message>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int client_socket;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];

    // Step A: Create a socket
    if ((client_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }

    // Specify server address and port
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
    server_addr.sin_port = htons(PORT);

    // Step D: Connect to the server
    if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
    {
        perror("Error connecting to server");
        exit(EXIT_FAILURE);
    }

    printf("Connected to server at %s:%d\n", SERVER_IP, PORT);

    // Step A: Send data to the server
    if (send(client_socket, argv[1], strlen(argv[1]), 0) == -1)
    {
        perror("Error sending data");
        exit(EXIT_FAILURE);
    }

    printf("Sent message to server: %s\n", argv[1]);

    // Step B: Receive data from the server
    if (recv(client_socket, buffer, sizeof(buffer), 0) <= 0)
    {
        perror("Error receiving data");
        exit(EXIT_FAILURE);
    }

    // Step C: Print the data received from the server
    printf("Received from server: %s\n", buffer);

    // Close socket
    close(client_socket);

    return 0;
}
