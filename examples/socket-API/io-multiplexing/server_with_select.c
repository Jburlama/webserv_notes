#include <errno.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <unistd.h>

int create_server_socket(int port, struct sockaddr_in *addr);
int connect_with_client(int server_socket, struct sockaddr_in *addr);
void deal_with_request(int client_socket);


int main()
{
    int server_socket;
    int client_socket;
    struct sockaddr_in addr;
    fd_set current_socket;
    fd_set ready_socket;


    server_socket = create_server_socket(8080, &addr);

    // inicialize my current set
    FD_ZERO(&current_socket);
    FD_SET(server_socket, &current_socket);
    while (42)
    {
        // because select is destructive
        ready_socket = current_socket;

        printf("******* Waiting for a Request *******\n");
        if (select(FD_SETSIZE, &ready_socket, NULL, NULL, NULL) < 0)
        {
            perror("Select Fail");
            exit(errno);
        }

        for (int i = 0; i < FD_SETSIZE; ++i)
        {
            if (FD_ISSET(i, &ready_socket)) // checking if fd is ready for reading
            {
                if (i == server_socket)
                {
                    // this is a new connection
                    client_socket = connect_with_client(server_socket, &addr);
                    FD_SET(client_socket, &current_socket); // add the client to set
                }
                else
                {
                    deal_with_request(i);
                    close(i); // close client socket fd
                    FD_CLR(i, &current_socket); // rm from set
                }
            }
        }
    }
    close(server_socket);
}


// Create a socket for the server to listen on
// Configure the IP address
// Declare that the socket is ready to listen
int create_server_socket(int port, struct sockaddr_in *addr)
{
    int                 socketfd;

    addr->sin_family = AF_INET;
    addr->sin_port = htons(port);
    addr->sin_addr.s_addr = INADDR_ANY;

    socketfd = socket(AF_INET, SOCK_STREAM, 0);
    if (bind(socketfd, (const struct sockaddr *)addr, sizeof(*addr)) != 0)
    {
        perror("Fail to bind socket\n");
        exit(errno);
    }
    listen(socketfd, SOMAXCONN);

    return socketfd;
}


// Wait for a client to send a request
int connect_with_client(int server_socket, struct sockaddr_in *addr)
{
    int         client_socket;
    socklen_t   addr_len;

    addr_len = sizeof(struct sockaddr_in);
    client_socket = accept(server_socket, (struct sockaddr *)addr, &addr_len);
    return client_socket;
}


// send a response
// Recives the request from the client and prints to stdout
// send back a response with the client request
void deal_with_request(int client_socket)
{
    char *response;
    char buffer[1024];

    memset(buffer, 0, sizeof(buffer));

    response = "Message Receved Sucessfully:\n";
    send(client_socket, response, strlen(response), 0);

    printf("%s\n", response);

    recv(client_socket, buffer, sizeof(buffer) - 1, 0);
    send(client_socket, buffer, strlen(buffer), 0);

    printf("%s\n", buffer);
}
