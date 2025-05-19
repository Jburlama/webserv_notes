[[socket()#**Parameters Explained**]]

> [!CAUTION]
> DeepSeek generated answer

The **`protocol`** parameter in the `socket()` function specifies the **specific protocol** to be used with the socket, within the constraints of the chosen **domain** (address family) and **type** (communication semantics). Let’s break this down in detail.

---

### **Purpose of the `protocol` Parameter**
While the `domain` (e.g., `AF_INET` for IPv4) and `type` (e.g., `SOCK_STREAM` for TCP) narrow down the socket’s behavior, the `protocol` parameter allows you to:
- Explicitly select a protocol when multiple protocols exist for the same `domain` and `type`.
- Use non-default or specialized protocols (e.g., raw packet manipulation).

In most cases, you’ll set `protocol` to **`0`**, letting the OS choose the **default protocol** for the given `domain` and `type`. However, advanced use cases (e.g., raw sockets) require explicitly specifying a protocol.

---

### **Common Values for `protocol`**

#### 1. **`0` (Default Protocol)**
- The OS selects the standard protocol for the given `domain` and `type`.
- Example:
  - `AF_INET` + `SOCK_STREAM` → **TCP** (default for stream sockets).
  - `AF_INET` + `SOCK_DGRAM` → **UDP** (default for datagram sockets).

#### 2. **Explicit Protocols**
- Use these when you need fine-grained control:
  - **`IPPROTO_TCP`**: Explicitly select TCP (redundant if `type` is `SOCK_STREAM`).
  - **`IPPROTO_UDP`**: Explicitly select UDP (redundant if `type` is `SOCK_DGRAM`).
  - **`IPPROTO_ICMP`**: For raw sockets handling ICMP packets (e.g., `ping`).
  - **`IPPROTO_RAW`**: For crafting custom IP headers (requires `SOCK_RAW`).
  - **`IPPROTO_SCTP`**: Stream Control Transmission Protocol (a less common alternative to TCP).

#### 3. **Protocol Numbers**
- You can use **IANA-assigned protocol numbers** (listed in `/etc/protocols` on Unix-like systems).  
  Example:
  ```c
  // ICMP protocol number is 1
  int sock = socket(AF_INET, SOCK_RAW, 1); // Equivalent to IPPROTO_ICMP
  ```

---

### **How It Works**
The OS validates the triplet:  
`(domain, type, protocol)` → Must be a valid combination.  

#### **Example Scenarios**
1. **Standard TCP Socket**:
   ```c
   int tcp_sock = socket(AF_INET, SOCK_STREAM, 0); // OS picks TCP
   ```
   - Equivalent to:
     ```c
     int tcp_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
     ```

2. **Raw ICMP Socket**:
   ```c
   int raw_sock = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
   ```
   - Allows sending/receiving ICMP packets (requires root privileges).

3. **Invalid Combination**:
   ```c
   // Error: SOCK_STREAM (TCP) is incompatible with UDP protocol
   int bad_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_UDP);
   ```
   - Returns `-1` and sets `errno` to `EPROTONOSUPPORT`.

---

### **Key Use Cases for Explicit `protocol`**

#### 1. **Raw Sockets**
- Used for low-level network programming (e.g., crafting custom packets).
- Example: Creating a raw socket to send ICMP echo requests (ping):
  ```c
  int sock = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
  ```

#### 2. **Non-Default Protocols**
- Using protocols like **SCTP** (Stream Control Transmission Protocol):
  ```c
  int sctp_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_SCTP);
  ```

#### 3. **Protocol Selection in Overlapping Cases**
- When a `domain`/`type` pair supports multiple protocols (rare).

---

### **How to Find Protocol Numbers**
- Check `/etc/protocols` on Unix-like systems:
  ```bash
  cat /etc/protocols
  # Example output:
  # icmp    1   ICMP
  # tcp     6   TCP
  # udp     17  UDP
  ```
- Use the `getprotobyname()` function in C:
  ```c
  #include <netdb.h>
  struct protoent *proto = getprotobyname("icmp");
  if (proto) {
    printf("ICMP protocol number: %d\n", proto->p_proto); // Output: 1
  }
  ```

---

### **Common Mistakes**
1. **Mismatched `type` and `protocol`**:
   ```c
   // UDP (SOCK_DGRAM) cannot be used with IPPROTO_TCP
   int sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_TCP); // Fails!
   ```

2. **Assuming `0` Always Works**:
   - For raw sockets (`SOCK_RAW`), you **must** specify a protocol (e.g., `IPPROTO_ICMP`).

3. **Ignoring Privilege Requirements**:
   - Raw sockets often require root access (`sudo`/`CAP_NET_RAW` capability).

---

### **Summary**
- **`protocol`**: Specifies the exact protocol to use, given a `domain` and `type`.
- **`0`**: Lets the OS choose the default (recommended for most use cases).
- **Explicit values**: Needed for raw sockets or non-standard protocols.
- Always validate combinations of `domain`, `type`, and `protocol` to avoid errors.
