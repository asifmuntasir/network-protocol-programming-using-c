#include <stdio.h>  /* I/O functions */
#include <string.h> /* string functions */
#include <stdlib.h> /* C standard functions */
#include <unistd.h>
#include <arpa/inet.h>  /* Internet operations */
#include <sys/socket.h> /* socket functions */
#include <sys/types.h>  /* library of basic types */
#include <time.h>       /* time functions */

#define MAX_BUFFER_SIZE 1024

int main(int argc, char *argv[])
{
    const char *server_ip = argv[1];
    const int server_port = atoi(argv[2]);
    struct sockaddr_in server_address;
    char buffer[MAX_BUFFER_SIZE];
    const char *name = "Bipul Rahman";
    const char *student_number = "T00566221";
    char message[MAX_BUFFER_SIZE];

    /* Check that two arguments were passed on the command line */
    if (argc != 3)
    {
        fprintf(stderr, "Usage: %s <server_ip> <port>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Create socket
    int client_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (client_socket == -1)
    {
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }

    // Prepare server address

    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(server_port);
    if (inet_pton(AF_INET, server_ip, &server_address.sin_addr) <= 0)
    {
        perror("Error converting server IP address");
        exit(EXIT_FAILURE);
    }

    // Prepare message (including name and student number)
    snprintf(message, MAX_BUFFER_SIZE, "TimeRequest from %s, %s", name, student_number);

    // Send message to the server
    printf("Sending request to the server...\n");
    if (sendto(client_socket, message, strlen(message), 0, (struct sockaddr *)&server_address, sizeof(server_address)) == -1)
    {
        perror("Error sending message");
        close(client_socket);
        exit(EXIT_FAILURE);
    }

    // Receive response from the server
    printf("Waiting for the server's response...\n");
    socklen_t server_address_len = sizeof(server_address);
    ssize_t bytes_received = recvfrom(client_socket, buffer, MAX_BUFFER_SIZE - 1, 0, (struct sockaddr *)&server_address, &server_address_len);
    if (bytes_received == -1)
    {
        perror("Error receiving response");
        close(client_socket);
        exit(EXIT_FAILURE);
    }

    // Null-terminate the received data
    buffer[bytes_received] = '\0';

    /* Print the time received from the server */
    printf("Server's response: %s\n", buffer);

    // Close the socket and exit
    close(client_socket);

    return 0;
}
