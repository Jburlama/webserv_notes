# Resorces

[Links](https://web.notaduo.com/notes/29srs/edit)
[Ninx](https://nginx.org/en/)
[Beej's Guide to Network Programming](https://beej.us/guide/bgnet/html/)

---

## links

[[HTTP protocol]]
[[what is a socket]]
[[socket API]]
[[HTTP protocol]]
[[socket()]]
[[bind() vs connect()]]
[[how to verify sockets]]


---

# What is a Web server

Is a program who servs content on the web.

---

# Functions to learn
[[socket API]]

[X] gai_strerror - Returns a error message giving a errcode
[ ] socketpair
[X] htons - host to network short - for ports
[X] htonl - host to network long - for IPs
[ ] ntohs
[ ] ntohl
[X] select - monitor multiple file descriptors, allowing for multiple clients to connect. [[select()]]
[ ] poll
[ ] epoll (epoll_create, epoll_ctl, epoll_wait)
[ ] kqueue (kqueue, kevent)
[X] socket - Return a fd to socket [[socket()]]
[X] accept - Returns the fd of the client socket
[X] listen - Listen for connections on a socket
[X] send - Send message on a socket
[X] recv - recive message on a socket
[X] bind - Used by the server to bind to a socket [[bind() vs connect()]]
[X] connect - Used by the client to connect to a socket [[bind() vs connect()]]
[X] getaddrinfo - DNS and service name lookups
[X] freeaddrinfo - free the `res` link-list, passed to getaddrinfo
[X] setsockopt - allow the program to reuse the port
[ ] getsockname
[ ] getprotobyname
[ ] fcntl
[ ] stat

---


## socket
[[socket()]]
[[socket API]]
[[what is a socket]]
[Beej’s Guide to Network Programming](https://beej.us/guide/bgnet/html/#socket)

What we want to do is use the values from the results of the call to `getaddrinfo()`, and feed them into `socket()` directly like this:

```c
int s;
struct addrinfo hints, *res;

// do the lookup
// [pretend we already filled out the "hints" struct]
getaddrinfo("www.example.com", "http", &hints, &res);

// again, you should do error-checking on getaddrinfo(), and walk
// the "res" linked list looking for valid entries instead of just
// assuming the first one is good (like many of these examples do).
// See the section on client/server for real examples.

s = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
```

Create a socket, connected to a local IP bind, and wait for a connection

***socket() -> bind() -> listen() -> accept() → read()/write()***.

- socket() -> Create;
- bind() -> bind to IP and Port;
- listen() -> Prepares the socket to accept incoming connections (non-blocking) (TCP-specific).
- accept() -> Blocks and waits for a client to connect, then creates a new socket for that connection.
- send -> Send message.
- recv -> Recieve message.

> "Why not combine listen() and accept()?"
    Because listen() sets up the queue once, while accept() is called repeatedly.
    Combining them would force re-initializing the queue for every client (inefficient).


> "What if I call accept() without listen()?"
   It fails! listen() must be called first to mark the socket as a passive listener.

> Why listen() can’t be part of bind():
    bind() is used in scenarios where no listening is required (e.g., UDP servers, TCP clients).
    Combining them would force all bound sockets into a listening state, breaking non-TCP use cases and clients.


### select(), poll(), epoll()
[[socket API#select - Allows a program to monitor multiple file descriptors]]
[[select()]]

Allow the server to handle multiple connections without the need for threads.
Select is destructive, meaning it will change the fds.

pooll() is very much like select()
[[socket API#poll - wait for some event on a file descriptor]]


---

## HTTP

[[HTTP protocol]]

### Requests

- Url
- Method Type (You need at least the GET, POST, and DELETE methods).
- Headers
- Body


### Resposnse

- Status Code
- Headers
- Body

### GET vs POST

[[GET vs POST]]

> [!NOTE]
> The requests methods funtionallty, is only a convention, and is what people are expection.
> In reality the funtionally of these methods are completly in the hands of the backend developers.

| property                | GET            | POST           |
| -----------------       | -------------- | -------------- |
| Body                    | Dont have      | Has            |
| Data Request Limit      | Yes(2048)      | No limit       |
| Data Type               | Ascii only     | Any Data       |
| safe                    | Yes            | No             | -> makes chages to the backend
| Idempotent              | Yes            | No             | -> The ability to make multiple requests to the same resorce without changing the request
| Caching and Prefetching | Yes            | No             | 
| Bookmarkable            | Yes            | No             | 
| Security?               | No             | Yes            | 

If The method is *read only*, most of the time you want to use GET.

---

## CGI

[[CGI]]

