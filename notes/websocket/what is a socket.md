
A **socket** is a end point for aplications to connect computers.
If two devises want to comunicate with each other, they each one must be connected to a socket.

You can have two types of connections:
    - client
    - server
The main diference on how the interatc with the socket API is the cliente uses the ***connect*** syscall and the server the ***accept***.
Sockets are the backbone of modern network communication, enabling protocols like HTTP, FTP, SSH, and more.

[[socket API]]

Every time you call socket(), the kernel creates a brand-new socket and returns a file descriptor that points to it.


---


> [!CAUTION]
> DeepSeek generated answer

A **socket** is a fundamental abstraction in computer networking that allows two programs (often running on different machines) to communicate over a network or locally.
It acts as an **endpoint** for sending and receiving data, similar to how a phone is an endpoint for a conversation.
Sockets are the backbone of modern network communication, enabling protocols like HTTP, FTP, SSH, and more.

---

### **What is a Socket?**
- **Software Interface**: A socket is a software construct provided by the operating system (OS) that abstracts the complexity of network communication.
- **File Descriptor**: In Unix-like systems, a socket is represented as a **file descriptor** (an integer), allowing you to use standard I/O operations (`read`, `write`, `close`) to interact with it.
- **End-to-End Communication**: Sockets define the "ends" of a connection, including:
  - IP address (to identify the machine).
  - Port number (to identify the application/service on the machine).
  - Protocol (e.g., TCP/UDP).

---

### **How Sockets Work: A Step-by-Step Overview**

#### **1. Socket Creation**
```c
int sockfd = socket(domain, type, protocol);
```
- The `socket()` system call creates a socket in the kernel and returns a file descriptor.
- Example:
  - `domain`: `AF_INET` (IPv4) or `AF_INET6` (IPv6).
  - `type`: `SOCK_STREAM` (TCP) or `SOCK_DGRAM` (UDP).
  - `protocol`: Typically `0` (let OS choose the default for the type).

#### **2. Binding (Server-Side)**
```c
bind(sockfd, (struct sockaddr*)&addr, sizeof(addr));
```
- Assigns the socket to a specific **IP address** and **port**.
- Servers use this to "listen" on a known port (e.g., port 80 for HTTP).
- Example:
  ```c
  struct sockaddr_in addr = {
    .sin_family = AF_INET,
    .sin_port = htons(8080),    // Port 8080
    .sin_addr.s_addr = INADDR_ANY // Accept connections from any IP
  };
  ```

#### **3. Listening (TCP Server)**
```c
listen(sockfd, backlog);
```
- For TCP servers: Marks the socket as passive, ready to accept incoming connections.
- `backlog`: Maximum number of pending connections in the queue.

#### **4. Accepting Connections (TCP Server)**
```c
int client_sock = accept(sockfd, (struct sockaddr*)&client_addr, &addr_len);
```
- Blocks until a client connects.
- Returns a **new socket** (`client_sock`) dedicated to that client.

#### **5. Connecting (Client-Side)**
```c
connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr));
```
- For TCP clients: Initiates a connection to a server’s IP/port.
- For UDP: Optionally sets the default destination address.

#### **6. Data Transfer**
- **TCP** (reliable, stream-based):
  ```c
  send(sockfd, buffer, buffer_size, 0);  // Send data
  recv(sockfd, buffer, buffer_size, 0);  // Receive data
  ```
- **UDP** (unreliable, message-based):
  ```c
  sendto(sockfd, buffer, size, 0, (struct sockaddr*)&dest_addr, addr_len);
  recvfrom(sockfd, buffer, size, 0, (struct sockaddr*)&src_addr, &addr_len);
  ```

#### **7. Closing the Socket**
```c
close(sockfd);
```
- Releases OS resources associated with the socket.
- For TCP: Sends a `FIN` packet to gracefully terminate the connection.

---

### **Key Concepts**

#### **Socket Types**
1. **Stream Sockets (`SOCK_STREAM`)**:
   - Uses **TCP** (reliable, connection-oriented).
   - Guarantees in-order delivery and error correction.
   - Example: Web servers (HTTP/HTTPS).

2. **Datagram Sockets (`SOCK_DGRAM`)**:
   - Uses **UDP** (unreliable, connectionless).
   - No guarantees on delivery or order.
   - Example: Video streaming, DNS queries.

3. **Raw Sockets (`SOCK_RAW`)**:
   - Direct access to network-layer protocols (e.g., ICMP, custom IP packets).
   - Requires superuser privileges.

#### **Socket Address**
- A combination of:
  - **IP Address**: Identifies the machine (e.g., `192.168.1.1`).
  - **Port**: Identifies the application/service (e.g., port `443` for HTTPS).
  - **Protocol**: Rules for communication (e.g., TCP/UDP).

#### **Socket States (TCP)**
- `LISTEN`: Server is waiting for connections.
- `ESTABLISHED`: Connection is active.
- `TIME_WAIT`: Waiting to ensure all packets are flushed (post-close).

---

### **How Data Flows Through Sockets**
1. **Application Layer**: Your code writes data to the socket (e.g., `send()`).
2. **Transport Layer**: The OS breaks data into segments (TCP) or datagrams (UDP).
3. **Network Layer**: Adds IP headers (source/destination IP).
4. **Link Layer**: Frames the data for the physical network (e.g., Ethernet).
5. **Receiving End**: The reverse process happens to reconstruct the data.

---

### **Example: TCP Client-Server Workflow**

#### **Server**
1. Create socket → `socket()`
2. Bind to IP/port → `bind()`
3. Listen → `listen()`
4. Accept connection → `accept()`
5. Read/write data → `recv()`/`send()`
6. Close → `close()`

#### **Client**
1. Create socket → `socket()`
2. Connect to server → `connect()`
3. Read/write data → `send()`/`recv()`
4. Close → `close()`

---

### **Socket Internals (Simplified)**
- **Kernel Buffers**: The OS manages send/receive buffers for each socket.
- **Blocking vs. Non-Blocking**:
  - Blocking sockets wait until data is sent/received.
  - Non-blocking sockets return immediately (use `fcntl()` or `SOCK_NONBLOCK`).
- **Multiplexing**: Tools like `select()`, `poll()`, or `epoll()` allow monitoring multiple sockets simultaneously.

---

### **Common Use Cases**
- Web servers/clients (HTTP/HTTPS).
- Email (SMTP/IMAP).
- File transfer (FTP).
- Real-time communication (WebSockets, VoIP).

---

### **Summary**
A socket is a **communication endpoint** that lets applications exchange data over a network or locally. It abstracts low-level networking details, allowing developers to focus on application logic. Whether you’re streaming video (UDP) or loading a webpage (TCP), sockets are the invisible glue making it all possible.
