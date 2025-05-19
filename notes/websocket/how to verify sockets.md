Use tools like **`netstat`**, **`ss`**, or **`lsof`** to check for open sockets:

```bash
# List all TCP/UDP sockets (Linux/macOS)
netstat -tuln      # Traditional
ss -tuln           # Faster/more modern

# Find sockets tied to a specific port (e.g., port 8080)
lsof -i :8080
```
If your program is terminated, you **won’t see its sockets** in these lists (unless in `TIME_WAIT`).
