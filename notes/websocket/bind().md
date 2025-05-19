> [!CAUTION]
> DeepSeek generated answer

[[socket API#bind - bind a name to a socket]]

The `bind()` function in socket programming assigns a specific local protocol address (IP address and port number) to a socket.
It is typically used by servers to specify the port they will listen on, but can also be used by clients to pin a socket to a specific address.
Here's a detailed breakdown of the function and its `struct sockaddr *addr` parameter:

---

### **Function Signature**
```c
int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
```
- **Parameters**:
  - `sockfd`: The socket file descriptor returned by `socket()`.
  - `addr`: A pointer to a protocol-specific address structure (e.g., `sockaddr_in` for IPv4).
  - `addrlen`: The size of the address structure (e.g., `sizeof(struct sockaddr_in)`).
- **Return Value**:
  - `0` on success, `-1` on failure (sets `errno`).

---

### **Purpose of `bind()`**
- Associates a socket with a **local IP address** and **port number**.
- For servers: Binds to a well-known port (e.g., port 80 for HTTP).
- For clients: Optional (the OS assigns an ephemeral port automatically if not bound).

---

### **The `struct sockaddr *addr` Parameter**
This is a generic pointer to a structure holding the address information.
The actual structure type depends on the **address family** (e.g., IPv4, IPv6, Unix sockets).

#### **Common Address Structures**
1. **IPv4 (`AF_INET`)**:
   ```c
   struct sockaddr_in {
       sa_family_t    sin_family; // Address family (AF_INET)
       in_port_t      sin_port;   // Port (16-bit, network byte order)
       struct in_addr sin_addr;   // IPv4 address (32-bit)
       char           sin_zero[8]; // Padding (zeroed out)
   };

   struct in_addr {
       in_addr_t s_addr; // 32-bit IPv4 address (network byte order)
   };
   ```

2. **IPv6 (`AF_INET6`)**:
   ```c
   struct sockaddr_in6 {
       sa_family_t     sin6_family;   // AF_INET6
       in_port_t       sin6_port;     // Port (network byte order)
       uint32_t        sin6_flowinfo; // IPv6 flow information
       struct in6_addr sin6_addr;     // IPv6 address (128-bit)
       uint32_t        sin6_scope_id; // Scope ID
   };

   struct in6_addr {
       unsigned char s6_addr[16]; // 128-bit IPv6 address
   };
   ```

3. **Unix Domain Sockets (`AF_UNIX`)**:
   ```c
   struct sockaddr_un {
       sa_family_t sun_family; // AF_UNIX
       char        sun_path[108]; // Pathname (e.g., "/tmp/my_socket")
   };
   ```

---

### **Key Steps to Use `bind()`**
1. **Create the Address Structure**:
   - For IPv4:
     ```c
     struct sockaddr_in addr;
     memset(&addr, 0, sizeof(addr)); // Clear the structure
     addr.sin_family = AF_INET; // IPv4
     addr.sin_port = htons(8080); // Port 8080 (network byte order)
     addr.sin_addr.s_addr = htonl(INADDR_ANY); // Bind to all interfaces
     ```

2. **Call `bind()`**:
   ```c
   int result = bind(sockfd, (struct sockaddr *)&addr, sizeof(addr));
   if (result == -1) {
       perror("bind() failed");
       exit(EXIT_FAILURE);
   }
   ```

---

### **Important Details**
#### 1. **Byte Order Conversion**:
   - Use `htons()` (host to network short) for ports.
   - Use `htonl()` (host to network long) for IP addresses.
   - Example:
     ```c
     addr.sin_port = htons(8080); // Converts 8080 to network byte order
     ```

#### 2. **Special IP Addresses**:
   - `INADDR_ANY` (IPv4): Bind to all available interfaces.
   - `in6addr_any` (IPv6): Equivalent for IPv6.
   - Specific IP: Bind to a single interface (e.g., `inet_addr("192.168.1.100")`).

#### 3. **Port Rules**:
   - Ports 0-1023: Reserved (require root privileges).
   - Ports 1024-49151: Registered ports (use freely, but check IANA assignments).
   - Ports 49152-65535: Ephemeral (for client-side OS-assigned ports).

#### 4. **Common Errors**:
   - `EADDRINUSE`: The port is already in use.
   - `EACCES`: Permission denied (e.g., binding to a privileged port without root).
   - `EINVAL`: Socket already bound or invalid address.

---

### **Example: Binding a TCP Server to Port 8080**
```c
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("socket() failed");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8080); // Port 8080
    addr.sin_addr.s_addr = htonl(INADDR_ANY); // Bind to all interfaces

    if (bind(sockfd, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
        perror("bind() failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    printf("Socket bound to port 8080!\n");
    close(sockfd);
    return 0;
}
```

---

### **Summary**
- **`bind()`** assigns a local address (IP + port) to a socket.
    - Which network interface(s) the socket will listen on.
    - Which IP(s) the socket will accept connections from. 
- **`struct sockaddr *addr`** is a generic pointer to an address structure (IPv4/IPv6/Unix).
- Always use **byte order functions** (`htons`, `htonl`) for port and IP addresses.
- Use `INADDR_ANY` to bind to all network interfaces.
