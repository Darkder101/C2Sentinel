# C2Sentinel

**C2Sentinel** is a minimal Command and Control (C2) framework built for **educational and research purposes only**.  
It demonstrates the core communication model between a C2 server and an agent, using Winsock for TCP connections.

---

## 📜 Features
- Basic C2 server-client communication
- Agent persistence via Windows Registry
- Configurable IP and Port via config files (`server_config.txt` / `agent_config.txt`)

---

## 📂 Project Structure
- Sentinel_server.cpp # C2 server source
- Sentinel_agent.cpp # C2 agent source
- server_config.txt # Server IP/Port config
- agent_config.txt # Agent IP/Port config

---
## ⚙️ Config File Format
Both config files follow this format:
- IP=127.0.0.1
- PORT=4444

- **Server** reads from `server_config.txt`
- **Agent** reads from `agent_config.txt`

---

## 🚀 How to Build & Run

1. Compile both `Sentinel_server.cpp` and `Sentinel_agent.cpp` with a C++ compiler supporting Winsock2 (e.g., MSVC).
2. Place `server_config.txt` in the same directory as the server executable.
3. Place `agent_config.txt` in the same directory as the agent executable.
4. Run the server first, then run the agent.

> 🔐 Ensure your environment is isolated and used strictly for learning or research.

---

## ⚠️ Disclaimer & Author Liability

### ❗ The author of this project is **NOT responsible** for:
- Any illegal or unauthorized use of this software
- Damage caused to systems, networks, or data
- Violations of laws, regulations, or policies resulting from misuse

This tool was created **exclusively for learning and academic discussion** about C2 infrastructure design and detection.

By downloading, using, or distributing this software, you agree that:
- You will **not use it for unauthorized penetration testing, hacking, or attacks**
- You assume **full responsibility** for any consequences of its use
- You comply with all applicable local, national, and international laws

The author **disclaims all liability** and **will not be held accountable** under any circumstances for misuse of this code.

---

## 🛑 Why Development Stopped

While developing additional features (encryption, file transfer, etc.), it became clear that even in an educational context, the potential for misuse is significant.  
To prevent any possible harm or legal risk, the project is now archived and **no further development will be made**.

