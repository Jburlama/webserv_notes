[[what is a socket]]
[[socket API]]
[Beej’s Guide to Network Programming](https://beej.us/guide/bgnet/html/#socket)

> [!CAUTION]
> DeepSeek generated answer

The `socket()` function in C is used to create a new communication endpoint (a socket) for network interactions.
It is part of the [Berkeley Sockets API](https://en.wikipedia.org/wiki/Berkeley_sockets) and is foundational for network programming. Below is a detailed breakdown of its parameters, functionality, and usage:

---

### **Function Signature**
```c
int socket(int domain, int type, int protocol);
```
- **Return Value**: 
  - On success: A **file descriptor** (a non-negative integer) representing the socket.
  - On failure: Returns `-1`, and `errno` is set to indicate the error (e.g., `EAFNOSUPPORT`, `EPROTONOSUPPORT`, `EMFILE`).

---

### **Parameters Explained**

#### 1. **`domain` (Address Family)**
Specifies the communication domain (address family) for the socket. Common values include:
- **`AF_INET`**: IPv4 Internet protocols (e.g., `192.168.1.1`).
- **`AF_INET6`**: IPv6 Internet protocols (e.g., `2001:db8::1`).
- **`AF_UNIX`** (or `AF_LOCAL`): Communication between processes on the same host (Unix domain sockets).
- **`AF_PACKET`**: Low-level packet interfaces (e.g., raw sockets for link-layer headers).
- **`AF_BLUETOOTH`**: Bluetooth communication.

#### 2. **`type` (Socket Type)**
Defines the semantics of communication. Common types:
- **`SOCK_STREAM`**: 
  - Connection-oriented, reliable, bidirectional byte streams (e.g., **TCP**).
  - Guarantees in-order delivery and error checking.
- **`SOCK_DGRAM`**:
  - Connectionless, unreliable messages (e.g., **UDP**).
  - No guarantees on order or delivery.
- **`SOCK_RAW`**:
  - Provides raw network protocol access (e.g., custom ICMP packets).
  - Requires superuser privileges on many systems.

**Optional Flags** (may be combined with `type` using `|`):
- **`SOCK_NONBLOCK`**: Sets the socket to non-blocking mode (Linux-specific).
- **`SOCK_CLOEXEC`**: Closes the socket on `exec()` calls (to prevent inheritance by child processes).

#### 3. **`protocol`**
Specifies the protocol to use within the selected `domain` and `type`. Common values:
- **`0`**: Let the system choose the default protocol for the given `type` (e.g., `IPPROTO_TCP` for `SOCK_STREAM`, `IPPROTO_UDP` for `SOCK_DGRAM`).
- **`IPPROTO_ICMP`**: For raw ICMP packets.
- **`IPPROTO_RAW`**: For manually crafting IP headers.
- Other protocols like `IPPROTO_SCTP` or `IPPROTO_IGMP` (less common).

[[more indepth explanation of protocol]]

---

### **How It Works**
1. **Creates a Socket**: The kernel allocates resources for the socket and returns a file descriptor.
2. **Configures Communication**: Based on `domain`, `type`, and `protocol`, the socket is set up for:
   - IP-based communication (IPv4/IPv6).
   - Stream (TCP) or datagram (UDP) semantics.
   - Raw socket access (if applicable).
3. **Returns a File Descriptor**: Used in subsequent calls like `bind()`, `listen()`, `connect()`, `send()`, or `recv()`.

---

### **Common Use Cases**
1. **TCP Socket (IPv4)**:
   ```c
   int tcp_socket = socket(AF_INET, SOCK_STREAM, 0);
   ```
2. **UDP Socket (IPv6)**:
   ```c
   int udp_socket = socket(AF_INET6, SOCK_DGRAM, 0);
   ```
3. **Raw ICMP Socket**:
   ```c
   int raw_socket = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
   ```

---

### **Error Handling**
Check for errors after calling `socket()`:
```c
if (tcp_socket == -1) {
    perror("socket() failed");
    exit(EXIT_FAILURE);
}
```

---

### **Notes**
- **Non-Blocking Sockets**: Use `fcntl()` or `SOCK_NONBLOCK` (Linux) to set non-blocking mode.
- **Portability**: The `domain`, `type`, and `protocol` values may vary slightly across systems.
- **Windows**: The function is part of Winsock (`winsock2.h`), and usage requires initializing the Winsock library.

By understanding these parameters, you can create sockets tailored to specific networking needs, whether for reliable TCP communication, lightweight UDP messaging, or low-level protocol experimentation.


---

#### **What Happens When You Call `socket()`?**
   - **Kernel-Side**:  
     The kernel allocates **new resources** for a fresh socket:
     - Creates a **socket object** in kernel memory (e.g., structs for buffers, protocol-specific metadata, etc.).
     - Assigns a unique **file descriptor** (integer) to reference this socket in subsequent operations.
   - **User-Side**:  
     You receive the file descriptor (e.g., `3`, `4`, etc.), which acts as a "handle" to interact with the newly created socket.

   Example:  
   ```c
   int sock1 = socket(AF_INET, SOCK_STREAM, 0); // Creates socket A (fd=3)
   int sock2 = socket(AF_INET, SOCK_DGRAM, 0);  // Creates socket B (fd=4)
   ```
   Here, `sock1` and `sock2` are two **independent sockets** with their own kernel resources.

---

### **Common Pitfall**
Forgetting to close sockets can lead to resource exhaustion:
```c
// BAD: Leaks sockets if called repeatedly!
int sock = socket(...);
// Do something...
// Forgot close(sock)!
```

The socket exists in the kernel until you call close(fd), which triggers the kernel to clean up its resources.
If your program terminates (normally or abnormally), **the operating system (OS) automatically closes all sockets** and releases their resources. You don’t need to manually close them afterward. However, sockets in certain states (e.g., `TIME_WAIT`) might persist briefly in the OS network stack, but they are **not tied to your program**.

---


