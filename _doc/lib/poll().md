**Übersicht zu `poll()`**:  

---

## **🔹 `poll()` – Übersicht**  
`poll()` ist eine Funktion zur Überwachung mehrerer Dateideskriptoren (Sockets, Dateien, Eingaben), ohne das Programm zu blockieren.  

### **📌 Syntax**
```cpp
#include <poll.h>
int poll(struct pollfd fds[], nfds_t nfds, int timeout);
```
| Parameter | Beschreibung |
|-----------|-------------|
| `fds[]`  | Array von `pollfd`-Strukturen (enthält die zu überwachenden Deskriptoren). |
| `nfds`   | Anzahl der Deskriptoren im `fds[]`-Array. |
| `timeout` | Maximale Wartezeit in Millisekunden (`>0`: Zeitlimit, `0`: sofort prüfen, `-1`: unendlich warten). |

### **📌 Struktur `pollfd`**
```cpp
struct pollfd {
    int fd;       // Dateideskriptor (z. B. Socket oder Tastatur)
    short events; // Erwartete Ereignisse (z. B. POLLIN)
    short revents;// Tatsächlich eingetretene Ereignisse
};
```

### **📌 Wichtige `events`- und `revents`-Flags**
| Flag        | Bedeutung |
|-------------|----------|
| `POLLIN`    | Daten können gelesen werden (z. B. Tastatur, Socket). |
| `POLLOUT`   | Schreiben ist möglich, ohne zu blockieren. |
| `POLLERR`   | Fehlerzustand erkannt. |
| `POLLHUP`   | Verbindung wurde getrennt. |

---

## **🔹 Beispiel: Nutzereingabe prüfen, ohne zu blockieren**
```cpp
#include <iostream>
#include <poll.h>
#include <unistd.h> // Für STDIN_FILENO

int main() {
    struct pollfd fd{};
    fd.fd = STDIN_FILENO; // Standard-Eingabe (Tastatur)
    fd.events = POLLIN;   // Prüfe, ob Eingaben verfügbar sind

    std::cout << "Drücke eine Taste (5 Sekunden Wartezeit)...\n";

    int ret = poll(&fd, 1, 5000); // Timeout: 5000 ms = 5 Sekunden

    if (ret > 0 && (fd.revents & POLLIN)) {
        std::string input;
        std::getline(std::cin, input);
        std::cout << "Eingabe erhalten: " << input << std::endl;
    } else if (ret == 0) {
        std::cout << "Timeout: Keine Eingabe erkannt.\n";
    } else {
        std::cerr << "Fehler bei poll()!\n";
    }

    return 0;
}
```
