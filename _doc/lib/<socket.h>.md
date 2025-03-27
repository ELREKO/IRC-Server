### **Übersicht: `<sys/socket.h>` – Netzwerkprogrammierung in C/C++**  

[see Galileo Bock's](https://openbook.rheinwerk-verlag.de/linux_unix_programmierung/Kap11-005.htm)
[Macros](https://pubs.opengroup.org/onlinepubs/7908799/xns/syssocket.h.html)
---

## **1️⃣ Wichtige Funktionen**
| Funktion                 | Beschreibung |
|--------------------------|-------------|
| **`socket(domain, type, protocol)`** | Erstellt einen neuen Socket. |
| **`bind(sockfd, addr, addrlen)`** | Weist dem Socket eine Adresse (IP + Port) zu. |
| **`listen(sockfd, backlog)`** | Setzt den Socket in den Zuhörmodus (nur TCP). |
| **`accept(sockfd, addr, addrlen)`** | Akzeptiert eine eingehende Verbindung (nur TCP). |
| **`connect(sockfd, addr, addrlen)`** | Verbindet den Client mit einem Server. |
| **`send(sockfd, buf, len, flags)`** | Sendet Daten über einen TCP-Socket. |
| **`recv(sockfd, buf, len, flags)`** | Empfängt Daten über einen TCP-Socket. |
| **`sendto(sockfd, buf, len, flags, addr, addrlen)`** | Sendet Daten über einen UDP-Socket. |
| **`recvfrom(sockfd, buf, len, flags, addr, addrlen)`** | Empfängt Daten über einen UDP-Socket. |
| **`shutdown(sockfd, how)`** | Beendet eine Verbindung (`SHUT_RD`, `SHUT_WR`, `SHUT_RDWR`). |
| **`close(sockfd)`** | Schließt einen Socket. |

---

## **2️⃣ Wichtige Strukturen**
| Struktur               | Beschreibung |
|------------------------|-------------|
| **`struct sockaddr`** | Allgemeine Adresse für verschiedene Protokolle. |
| **`struct sockaddr_in`** | IPv4-spezifische Adresse (enthält IP + Port). |
| **`struct sockaddr_in6`** | IPv6-spezifische Adresse. |
| **`struct in_addr`** | Enthält eine IPv4-Adresse. |

**Beispiel `sockaddr_in`:**  
```cpp
struct sockaddr_in server;
server.sin_family = AF_INET;
server.sin_port = htons(8080);  // Port 8080
server.sin_addr.s_addr = INADDR_ANY;  // Alle verfügbaren Netzwerkschnittstellen
```

---

## **3️⃣ Server-Client-Kommunikation (TCP)**
### **🖥️ Server**
```cpp
int sockfd = socket(AF_INET, SOCK_STREAM, 0);
bind(sockfd, (struct sockaddr *)&server, sizeof(server));
listen(sockfd, 5);
int client_fd = accept(sockfd, nullptr, nullptr);
send(client_fd, "Hello!", 6, 0);
close(client_fd);
close(sockfd);
```

### **💻 Client**
```cpp
int sock = socket(AF_INET, SOCK_STREAM, 0);
connect(sock, (struct sockaddr *)&server, sizeof(server));
recv(sock, buffer, sizeof(buffer), 0);
close(sock);
```

---

## **4️⃣ Wichtige Konstanten**
| Konstante           | Beschreibung |
|---------------------|-------------|
| **`AF_INET`** | IPv4-Adresse |
| **`AF_INET6`** | IPv6-Adresse |
| **`SOCK_STREAM`** | TCP (verbindungsorientiert) |
| **`SOCK_DGRAM`** | UDP (verbindungslos) |
| **`INADDR_ANY`** | Beliebige lokale IP-Adresse |
| **`htons(port)`** | Konvertiert Portnummer ins Netzwerkformat |

---

## **5️⃣ Wichtige Systemaufrufe**
| Systemaufruf | Beschreibung |
|-------------|-------------|
| **`perror("Fehler")`** | Gibt eine Fehlermeldung aus. |
| **`fcntl(sockfd, F_SETFL, O_NONBLOCK)`** | Setzt den Socket in den nicht-blockierenden Modus. |
| **`getsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, &optlen)`** | Erlaubt das Wiederverwenden einer Adresse. |


