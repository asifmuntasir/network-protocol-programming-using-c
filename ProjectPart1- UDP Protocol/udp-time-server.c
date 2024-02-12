#include <stdio.h>  /* I/O functions */
#include <string.h> /* string functions */
#include <stdlib.h> /* C standard functions */
#include <time.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define MAX_BUFFER_SIZE 2064

int main()
{
    // Create socket
    int server_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (server_socket == -1)
    {
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }

    // Prepare server address
    struct sockaddr_in server_address;
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(9988);

    // Bind the socket
    if (bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) == -1)
    {
        perror("Error binding socket");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    // Print server information
    printf("Server is waiting on port 9988...\n");

    // Receive message from the client
    char buffer[MAX_BUFFER_SIZE];
    struct sockaddr_in client_address;
    socklen_t client_address_len = sizeof(client_address);
    ssize_t bytes_received = recvfrom(server_socket, buffer, MAX_BUFFER_SIZE - 1, 0, (struct sockaddr *)&client_address, &client_address_len);
    if (bytes_received == -1)
    {
        perror("Error receiving message");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    // Null-terminate the received data
    buffer[bytes_received] = '\0';

    // Get current time
    time_t current_time;
    time(&current_time);
    struct tm *local_time = localtime(&current_time);
    char time_str[MAX_BUFFER_SIZE];
    strftime(time_str, MAX_BUFFER_SIZE, "Time: %Y-%m-%d %H:%M:%S", local_time);

    // Prepare response message (including name, student number, and current time)
    const char *name = "Bipul Rahman";
    const char *student_number = "T00566221";
    char response[MAX_BUFFER_SIZE];
    printf("%s\n", time_str);
    snprintf(response, MAX_BUFFER_SIZE, "%s, %s\n", name, student_number);

    // Send the response to the client
    if (sendto(server_socket, response, strlen(response), 0, (struct sockaddr *)&client_address, client_address_len) == -1)
    {
        perror("Error sending response");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    // Print server's response
    printf("Response sent to the client: %s\n", response);

    // Close the socket and exit
    close(server_socket);

    return 0;
}
