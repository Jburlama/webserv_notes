[[socket()]]
[[what is a socket]]

## getaddrinfo, freeaddrinfo, gai_strerror - **network address and service translation**
[Beej’s Guide to Network Programming](https://beej.us/guide/bgnet/html/#getaddrinfoprepare-to-launch)

```sh
man getaddrinfo
```

```c
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

int getaddrinfo(const char *node,     // e.g. "www.example.com" or IP
                const char *service,  // e.g. "http" or port number
                const struct addrinfo *hints,
                struct addrinfo **res);
```

You give this function three input parameters, and it gives you a pointer to a linked-list, `res`, of results.
The `node` parameter is the host name to connect to, or an IP address.
Next is the parameter `service`, which can be a port number, like “80”, or the name of a particular service (found in The IANA Port List21 or the /etc/services file on your Unix machine) like “http” or “ftp” or “telnet” or “smtp” or whatever.
Finally, the `hints` parameter points to a struct addrinfo that you’ve already filled out with relevant information.
When we’re eventually all done with the linked list that getaddrinfo() so graciously allocated for us, we can (and should) free it all up with a call to `freeaddrinfo()`.

RETURN VALUE
getaddrinfo() returns 0 if it succeeds, or nonzero error code.

---


## socket - **create an endpoint for communication**
[Beej’s Guide to Network Programming](https://beej.us/guide/bgnet/html/#socket)
[[socket()]]

```sh
man socket
```

```c
#include <sys/types.h>
#include <sys/socket.h>

int socket(int domain, int type, int protocol);
```
- **domain**: specifies a communication domain; ie: AF_INET -> IPv4.
- **type**: specifies the type of the socket; ie: SOCK_STREAM -> TCP socket.
- ***protocol***: specifies the protocol layer. Mostly 0 is used to specifies the IP layer.


socket()  creates  an endpoint for communication and returns a file descriptor that refers to that endpoint. 
The file descriptor returned by a successful call will be the lowest-numbered file descriptor not currently open for the process.

RETURN VALUE
On success, a file descriptor for the new socket is returned.
On error, -1 is returned, and errno is set appropriately.


---

## connect - initiate a connection on a socket
[Beej’s Guide to Network Programming](https://beej.us/guide/bgnet/html/#connect)

```sh
man connect
```

```c
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

int connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
```
- ***sockfd***: Is the file descriptor return by sokcet().
- ***sockaddr***: Describes a socket address.
    Even though sockaddr is used has a body for the declaration, we should use a sockaddr acording to the type of socket
    ie: dockaddr_in for IPv4, dockaddr_in6 for IPv6 ...
- ***addrlen***: Is the sizeof the addr.


RETURN VALUE
If the connection or binding succeeds, zero is returned.
On error, -1 is returned, and errno is set appropriately.

[man sockaddr](https://www.man7.org/linux/man-pages/man3/sockaddr.3type.html)

```c
struct sockaddr_in {
    sa_family_t     sin_family;     /* AF_INET */
    in_port_t       sin_port;       /* Port number */
    struct in_addr  sin_addr;       /* IPv4 address */
};
```
- Example of addr:
```c
char *ip = "192.168.1.12"
struct sockaddr_in addr;

addr.sin_family = AF_INET;
addr.sin_port = htons(80);
// will conver char* to uint32
inet_pton(AF_INET, ip, &addr.sin_addr.s_addr) // addr.sin_addr.s_addr -> uint32 IP addr
```



---


## htonl,  htons,  ntohl, ntohs - convert values between host and network byte order
```sh
man htons
```

```c
#include <arpa/inet.h>

uint32_t htonl(uint32_t hostlong);

uint16_t htons(uint16_t hostshort);

uint32_t ntohl(uint32_t netlong);

uint16_t ntohs(uint16_t netshort);
```

The htonl() function converts the unsigned integer hostlong from  host byte order to network byte order.
The  htons()  function  converts  the unsigned short integer hostshort from host byte order to network byte order.
The ntohl() function converts the unsigned integer netlong  from  network byte order to host byte order.
The ntohs() function converts the unsigned short integer netshort from network byte order to host byte order.


---


## bind - bind a name to a socket
[Beej’s Guide to Network Programming](https://beej.us/guide/bgnet/html/#bind)

```sh
man bind
```
[[bind()]]
[[bind() vs connect()]]

```c
#include <sys/types.h>
#include <sys/socket.h>

int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
```

**Example:**
```c
struct addrinfo hints, *res;
int sockfd;

// first, load up address structs with getaddrinfo():

memset(&hints, 0, sizeof hints);
hints.ai_family = AF_UNSPEC;  // use IPv4 or IPv6, whichever
hints.ai_socktype = SOCK_STREAM;
hints.ai_flags = AI_PASSIVE;     // fill in my IP for me

getaddrinfo(NULL, "3490", &hints, &res);

// make a socket:

sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);

// bind it to the port we passed in to getaddrinfo():

bind(sockfd, res->ai_addr, res->ai_addrlen);
```

Another thing to watch out for when calling bind(): don’t go underboard with your port numbers.
All ports below 1024 are RESERVED (unless you’re the superuser)!
You can have any port number above that, right up to 65535 (provided they aren’t already being used by another program).

Sometimes, you might notice, you try to rerun a server and bind() fails, claiming “Address already in use.” What does that mean?
Well, a little bit of a socket that was connected is still hanging around in the kernel, and it’s hogging the port.
You can either wait for it to clear (a minute or so), or add code to your program allowing it to reuse the port, like this:

```c
int yes=1;
//char yes='1'; // Solaris people use this

// lose the pesky "Address already in use" error message
if (setsockopt(listener,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof yes) == -1) {
    perror("setsockopt");
    exit(1);
}
```

RETURN VALUE
On success, zero is returned.
On error, -1 is returned, and errno is set appropriately.


---

## send - send a message on a socket

```sh
man send
```

```c
#include <sys/types.h>
#include <sys/socket.h>

ssize_t send(int sockfd, const void *buf, size_t len, int flags);
```
It is possible that the data will not be sent all at once,
you can check for the return value to check for how many bitys was sent.
[Beej’s Guide to Network Programming](https://beej.us/guide/bgnet/html/#sendall)

RETURN VALUE
On success, these calls return the number of bytes sent.
On error, -1 is returned, and errno is set appropriately.

[[send-recv security risks]]

[Beej’s Guide to Network Programming](https://beej.us/guide/bgnet/html/#sendrecv)
---

##  recv - receive a message from a socket

```sh
man recv
```

```c
#include <sys/types.h>
#include <sys/socket.h>

ssize_t recv(int sockfd, void *buf, size_t len, int flags);
```

RETURN VALUE
Return the number of bytes received, or -1 if an error occurred.
In the event of an error, errno is set to indicate the error.

When a stream socket peer has performed an orderly shutdown, the return value will be 0 (the traditional "end-of-file" return).

Datagram  sockets  in various domains (e.g., the UNIX and Internet domains) permit zero-length datagrams.
When such a datagram is received, the return value is 0.

The value 0 may also be returned if the requested number of bytes to receive from a stream socket was 0.

---


## listen - listen for connections on a socket
[Beej’s Guide to Network Programming](https://beej.us/guide/bgnet/html/#listen)

```sh
man listen
```

```c
#include <sys/types.h>
#include <sys/socket.h>

int listen(int sockfd, int backlog);
```
**backlog**: Before calling the the next function (accept), the server can queue all the reciving connection up to INT connections.
             128/256: Common for web servers.
             Use SOMAXCONN for high-traffic servers (e.g., listen(sockfd, SOMAXCONN))
             How It Works:
                Client Connection Request:
                    - A client sends a SYN packet to the server.
                    - The server acknowledges with SYN-ACK and places the connection in a "SYN queue" (incomplete connections).
                Connection Completion:
                    - When the client responds with ACK, the connection moves to the "accept queue" (completed connections).
                Server Action:
                    - The server calls accept() to remove a connection from the accept queue and handle it.
             Key Behavior:
                - If the accept queue is full (i.e., backlog limit is reached), new completed connections are rejected.
                - If the SYN queue is full, new SYN packets may be ignored or trigger retries (OS-dependent).
             Why It Matters:
                - Too small: Clients see ECONNREFUSED under load.
                - Too large: Wastes kernel memory and may delay connection handling.
                - The backlog parameter ensures your server can handle bursts of incoming connections.



Enables TCP-specific connection acceptance. Used only by servers.
TCP clients: They use `connect()` to initiate connections, not `listen()`.
UDP sockets: UDP is connectionless and skips the `listen()/accept()` workflow entirely.



RETURN VALUE
On  success, zero is returned.
On error, -1 is returned, and errno is set appropriately.

---

## accept - accept a connection on a socket
[Beej’s Guide to Network Programming](https://beej.us/guide/bgnet/html/#acceptthank-you-for-calling-port-3490.)

```sh
man accept
```

```c
#include <sys/types.h>
#include <sys/socket.h>

int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
```

Creates a new socket for each client connection.
Returns the fd of the connecting client socket.
For each connected socket creates a fd on the server side.
After having both sockets connected in both sides, the functions **send** and **recv** can be used on both sides.

**addr**: Should be the addr filled by the client. Same for the addrlen;

All of this information can be gleaned from the results of the `getaddrinfo()` call, which rocks.


RETURN VALUE
On success, these system calls return a file descriptor  for  the  accepted  socket (a nonnegative integer).
On error, -1 is returned, errno is set appropriately, and addrlen is left unchanged.

> Common Confusions:
    ❌ "accept() returns the client’s socket."
    → No! The client’s socket exists on the client’s machine. accept() creates a server-side socket to communicate with the client.

---

## select - Allows a program to monitor multiple file descriptors
```sh
man select
```

[[select()]]

```c
#include <sys/select.h>

int select(int nfds, fd_set *readfds, fd_set *writefds,
           fd_set *exceptfds, struct timeval *timeout);
```

**nfds**: FD_SETSIZE -> the highest-numbered file descriptor in any of the three sets, plus 1.
**readfds**: -> Fds that are set for reading
**writefds**: -> Fds that are set for writing
**exceptfds**: -> Fds that are set for errors
**timeout**: -> If want to only wait a certen amont of time for changes

The four last argument all can be set to NULL;

select() is destructive, meaning it will change the set passed in.
So a temporary copy of the sets before calling select is needed.

select() is portable, while some of the replacements are not.

- fd_set -> A collection of file descriptors.

### macros for working with fd_set

void FD_CLR(int fd, fd_set *set); -> Remove a fd from a set.
int  FD_ISSET(int fd, fd_set *set); -> Check if the fd is in a set.
void FD_SET(int fd, fd_set *set); -> Add a socket to the  set.
void FD_ZERO(fd_set *set); -> Zero inicialice the a set.


RETURN VALUE
On  success, select() return the total number of bits that are set in readfds, writefds, exceptfds.
The return value may be zero if the timeout expired before  any  file  descriptors became ready.

On  error, -1 is returned, and errno is set to indicate the error; the
file descriptor sets are unmodified, and timeout becomes undefined.

---

## poll - wait for some event on a file descriptor

```sh
man poll
```

```c
#include <poll.h>

int poll(struct pollfd *fds, nfds_t nfds, int timeout);
```

poll() performs a similar task to select(2): it waits for one of a set
of file descriptors to become ready to perform  I/O.

The set of file descriptors to be monitored is specified  in  the  fds
argument, which is an array of structures of the following form:

```c
struct pollfd {
    int   fd;         /* file descriptor */
    short events;     /* requested events */
    short revents;    /* returned events */
};
```

**fd**: contains a file descriptor for an  open  file.
**events**: 

The  caller  should  specify  the  number of items in the fds array in
`nfds`.

RETURN VALUE
On success, poll() returns a nonnegative value.
A return value of  zero indicates  that  the system call timed out before any file descriptors became read.

On error, -1 is returned, and errno is set to indicate  the  cause  of
the error.

---

## epoll - I/O event notification facility
```bash
man epoll
```
```c
#include <sys/epoll.h>
```
The  epoll API performs a similar task to poll(2): monitoring multiple
file descriptors to see if I/O is possible on any of them.

The following system calls are provided to create and manage an  epoll
instance:

• `epoll_create(2)`  creates a new epoll instance and returns a file de‐
scriptor referring to that instance.

• Interest  in  particular  file  descriptors  is  then registered via
`epoll_ctl(2)`, which adds items to the interest list of the epoll instance.

• `epoll_wait(2)`  waits  for I/O events, blocking the calling thread if
no events are currently available.  (This system call can be thought
of as fetching items from the ready list of the epoll instance.)

