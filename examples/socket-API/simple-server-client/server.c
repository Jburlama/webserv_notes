#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

// Server
// Key function:
//  socket():15
//  bind():30
//  listen():40
//  accept():44
int main(void)
{
    int socketFD;

    // Create IPv4 sockeet
    socketFD = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in  addr;
    int                 result;

    // Bind to IPv4 address
    addr.sin_family = AF_INET;
    addr.sin_port   = htons(8080);

    // can be localhost, INADDR_ANY = 0.0.0.0, any incoming address
    addr.sin_addr.s_addr = INADDR_ANY;

    result = bind(socketFD, (const struct sockaddr *)&addr, sizeof(addr));
    if (result == 0)
        printf("%s\n", "Socket was bound successful!");


    int                 listenResult;
    struct sockaddr_in  client_addr;
    int                 client_addr_len;
    int                 clientSocketFD;

    client_addr_len = sizeof(client_addr);
    listenResult = listen(socketFD, 10);

    // Will get the client socket addr, This is blocking a fucntion


    char *msg;
    char buf[1024] = {0};

    while (1)
    {
        msg = "msg receved\n";
        clientSocketFD = accept(socketFD, (struct sockaddr *)&client_addr, (socklen_t *)&client_addr_len);
        send(clientSocketFD, msg, strlen(msg), 0);
        memset(buf, 0, sizeof(buf));
        recv(clientSocketFD, buf, 1024, 0);
        close(clientSocketFD);
    
        printf("Response was:\n %s\n", buf);
    }
    close(socketFD);

}
