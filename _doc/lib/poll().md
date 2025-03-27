[main](/)

---

# **🔹 `poll()` – Overview**  
`poll()` is a function used to monitor multiple file descriptors (sockets, files, inputs) without blocking the program.

## **📌 Syntax**
```cpp
#include <poll.h>
int poll(struct pollfd fds[], nfds_t nfds, int timeout);
```
| Parameter  | Description |
|------------|-------------|
| `fds[]`    | Array of `pollfd` structures (contains the descriptors to be monitored). |
| `nfds`     | Number of descriptors in the `fds[]` array. |
| `timeout`  | Maximum wait time in milliseconds (`>0`: time limit, `0`: check immediately, `-1`: wait indefinitely). |

## **📌 Structure `pollfd`**
```cpp
struct pollfd {
    int fd;       // File descriptor (e.g., socket or keyboard)
    short events; // Expected events (e.g., POLLIN)
    short revents;// Actually occurred events
};# `poll()`  

```

## **📌 Important `events` and `revents` Flags**
| Flag       | Meaning |
|------------|----------|
| `POLLIN`   | Data can be read (e.g., keyboard, socket). |
| `POLLOUT`  | Writing is possible without blocking. |
| `POLLERR`  | Error condition detected. |
| `POLLHUP`  | Connection has been closed. |

---

## **🔹 Example: Check User Input Without Blocking**
```cpp
#include <iostream>
#include <poll.h>
#include <unistd.h> // For STDIN_FILENO

int main() {
    struct pollfd fd{};
    fd.fd = STDIN_FILENO; // Standard input (keyboard)
    fd.events = POLLIN;   // Check if input is available

    std::cout << "Press a key (5-second timeout)...\n";

    int ret = poll(&fd, 1, 5000); // Timeout: 5000 ms = 5 seconds

    if (ret > 0 && (fd.revents & POLLIN)) {
        std::string input;
        std::getline(std::cin, input);
        std::cout << "Input received: " << input << std::endl;
    } else if (ret == 0) {
        std::cout << "Timeout: No input detected.\n";
    } else {
        std::cerr << "Error in poll()!\n";
    }

    return 0;
}
```