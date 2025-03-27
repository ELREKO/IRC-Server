[<back](<\<socket.h\>.md>) | [main](/)

---

## **Bedeutung der File-Deskriptoren**
| **fd**  | **Bedeutung**                          | **Woher kommt es?**               | **Verhalten in `poll()`** |
|---------|----------------------------------------|------------------------------------|---------------------------|
| **0**   | Standard-Eingabe (`stdin`)            | Wird vom Betriebssystem bereitgestellt | Erkennt Benutzereingaben in der Konsole |
| **1**   | Standard-Ausgabe (`stdout`)           | Wird vom Betriebssystem bereitgestellt | Nicht in `poll()` aufgenommen |
| **2**   | Standard-Fehlerausgabe (`stderr`)     | Wird vom Betriebssystem bereitgestellt | Nicht in `poll()` aufgenommen |
| **3**   | Server-Socket                         | Erstellt mit `socket(AF_INET, SOCK_STREAM, 0)` | Lauscht auf eingehende Verbindungen |
| **4**   | Client-Socket                         | Erstellt durch `accept(server_fdsocket, ...)` | Verwaltet die Verbindung zum Client |

