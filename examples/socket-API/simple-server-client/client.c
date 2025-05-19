#include <arpa/inet.h>
#include <errno.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>

// Client
int main(void)
{
    int socketFD;

    // Create IPv4 sockeet
    socketFD = socket(AF_INET, SOCK_STREAM, 0);

    char                *ip;
    struct sockaddr_in  addr;
    int                 result;

    // Create IPv4 address
    ip              = "127.0.0.1";
    addr.sin_family = AF_INET;
    addr.sin_port   = htons(8080);

    // converts str to uint32
    inet_pton(AF_INET, ip, &addr.sin_addr.s_addr);


    result = connect(socketFD, (const struct sockaddr *)&addr, sizeof(addr));
    if (result == 0)
        printf("%s\n", "Connection was successful!");
    else
        printf("%s:%i\n", "Connection Failed!", errno);

    char *msg;

    msg = "GET: \\ HTTP/1.1\r\nHost:google.com\r\n\r\n";
    send(socketFD, msg, strlen(msg), 0);

    char buf[1024] = {0};
    recv(socketFD, buf, 1024, 0);

    printf("Response was:\n %s\n", buf);
}
