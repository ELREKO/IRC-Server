[main](/)

---

# Network Programming in C/C++
## Overview: `<sys/socket.h>` 




### **1️⃣ Important Functions**
| Function                  | Description |
|---------------------------|-------------|
| **`socket(domain, type, protocol)`** | Creates a new socket. |
| **`bind(sockfd, addr, addrlen)`** | Assigns an address (IP + Port) to the socket. |
| **`listen(sockfd, backlog)`** | Puts the socket into listening mode (TCP only). |
| **`accept(sockfd, addr, addrlen)`** | Accepts an incoming connection (TCP only). |
| **`connect(sockfd, addr, addrlen)`** | Connects the client to a server. |
| **`send(sockfd, buf, len, flags)`** | Sends data over a TCP socket. |
| **`recv(sockfd, buf, len, flags)`** | Receives data over a TCP socket. |
| **`sendto(sockfd, buf, len, flags, addr, addrlen)`** | Sends data over a UDP socket. |
| **`recvfrom(sockfd, buf, len, flags, addr, addrlen)`** | Receives data over a UDP socket. |
| **`shutdown(sockfd, how)`** | Closes a connection (`SHUT_RD`, `SHUT_WR`, `SHUT_RDWR`). |
| **`close(sockfd)`** | Closes a socket. |

---

### **2️⃣ Important Structures**
| Structure               | Description |
|-------------------------|-------------|
| **`struct sockaddr`** | General address for various protocols. |
| **`struct sockaddr_in`** | IPv4-specific address (contains IP + Port). |
| **`struct sockaddr_in6`** | IPv6-specific address. |
| **`struct in_addr`** | Contains an IPv4 address. |

**Example `sockaddr_in`:**  
```cpp
struct sockaddr_in server;
server.sin_family = AF_INET;
server.sin_port = htons(8080);  // Port 8080
server.sin_addr.s_addr = INADDR_ANY;  // All available network interfaces
```

---

### **3️⃣ Server-Client Communication (TCP)**
#### **🖥️ Server**
```cpp
int sockfd = socket(AF_INET, SOCK_STREAM, 0);
bind(sockfd, (struct sockaddr *)&server, sizeof(server));
listen(sockfd, 5);
int client_fd = accept(sockfd, nullptr, nullptr);
send(client_fd, "Hello!", 6, 0);
close(client_fd);
close(sockfd);
```

#### **💻 Client**
```cpp
int sock = socket(AF_INET, SOCK_STREAM, 0);
connect(sock, (struct sockaddr *)&server, sizeof(server));
recv(sock, buffer, sizeof(buffer), 0);
close(sock);
```

---

### **4️⃣ Important Constants**
| Constant            | Description |
|--------------------|-------------|
| **`AF_INET`** | IPv4 address |
| **`AF_INET6`** | IPv6 address |
| **`SOCK_STREAM`** | TCP (connection-oriented) |
| **`SOCK_DGRAM`** | UDP (connectionless) |
| **`INADDR_ANY`** | Any available local IP address |
| **`htons(port)`** | Converts port number to network format |

---

### **5️⃣ Important System Calls**
| System Call | Description |
|------------|-------------|
| **`perror("Error")`** | Prints an error message. |
| **`fcntl(sockfd, F_SETFL, O_NONBLOCK)`** | Sets the socket to non-blocking mode. |
| **`getsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, &optlen)`** | Allows reusing an address. |

### Links

- [Galileo Bock's](https://openbook.rheinwerk-verlag.de/linux_unix_programmierung/Kap11-005.htm)  
- [man page](https://pubs.opengroup.org/onlinepubs/7908799/xns/syssocket.h.html)
 
#### Other used sockets
- [fd socket](fd_socket.md)