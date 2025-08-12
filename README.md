# 🎯 Custom C2 Framework (C++, Educational & Legal)

> **DISCLAIMER**: This project is for **educational purposes only**. Do **not** use it outside of isolated, legal lab environments you own or control. Any misuse is **strictly prohibited**.

---

## ✅ Development Roadmap (Stage-wise)

This project builds a minimal yet extensible Command & Control (C2) framework in **C++**, following red-team practices and staying within legal boundaries. We develop one phase at a time — clean, modular, and educational.

---

## 🔹 Phase 1: Reverse Shell Core (MVP)

### 🎯 Goal:
Establish a working reverse TCP connection with basic command execution.

### 🖥️ Agent:

- Connects back to C2 server (reverse TCP)
- Receives commands (e.g., `whoami`, `dir`, `ipconfig`)
- Executes them and sends output back

### 📡 Server:

- Listens for incoming agent connection
- Sends commands and receives results

### 💻 C++ Focus:

- WinSock (network sockets)
- `CreateProcess`, `ReadFile`, `WriteFile` for command execution

### ✅ Deliverable:

- A functioning reverse shell (PoC)
- The beginning of a custom C2 channel

---

## 🔹 Phase 2: Modular Command System

### 🎯 Goal:
Create a scalable architecture for handling commands.

### 🧩 Features:

- Support command formats like:
  - `exec:<cmd>`
  - `screenshot`
  - `keylog:on`
- Dispatch commands to modular handlers

### 💻 C++ Focus:

- Enums or strings for command parsing
- Handler functions (or class-based design)

### ✅ Result:

- Easily add new capabilities (keylogger, file ops, etc.)

---

## 🔹 Phase 3: Encrypted Communication

### 🎯 Goal:
Secure traffic with symmetric encryption.

### 🔐 Agent & Server:

- Use **AES-128** or **AES-256**
- Pre-shared symmetric key

### 💻 C++ Focus:

- OpenSSL or Windows CryptoAPI
- Encrypt/decrypt all messages

### ✅ Result:

- Encrypted command and response channel
- Reduced detection risk

---

## 🔹 Phase 4: Additional Command Capabilities

### 🛠️ Add Commands:

- `upload <file>`
- `download <file>`
- `screenshot` (GDI+ or Win32 API)
- `keylogger start/stop`
- `exit` or `self-delete`

### 💻 C++ Focus:

- Implement features one-by-one
- Modular design for rapid feature growth

---

## 🔹 Phase 5: Persistence

### 🎯 Goal:
Allow agent to survive reboot (for lab testing).

### 🔁 Techniques:

- **Registry**: Add `Run` key
- **Scheduled Tasks**: via `schtasks.exe` or COM
- **(Optional)**: WMI Event Subscriptions

### 💻 C++ Focus:

- Windows Registry APIs
- Scheduled task creation via shell or COM

### ✅ Note:

- Only run persistence in **your VM labs**!

---

## 🔹 Phase 6: Evasion Techniques

### 🎯 Goal:
Reduce AV/EDR detection in a controlled lab environment.

### 🕵 Techniques:

- Dynamic API resolution (`LoadLibrary`, `GetProcAddress`)
- String obfuscation (e.g., XOR encoding)
- Compile as **shellcode** (optional)
- Pack using **UPX** or custom loader

---

## 🔹 Phase 7: C2 Server Enhancements

### 🎯 Goal:
Make server usable for realistic red teaming.

### 📋 Features:

- Log command sessions
- Support multiple agents
  - (via `select()` or threads)
- Agent identification system
- Interactive CLI interface

---

## ⚖️ Legal & Ethical Use

This C2 framework is strictly for:

- Educational cybersecurity labs
- Malware analysis training
- Red team practice in controlled environments

Do **not** deploy this on systems you do not own or have explicit permission to test.

---
| Phase | Feature                                   | Code File                                    | Description                                   |
| ----- | ----------------------------------------- | -------------------------------------------- | --------------------------------------------- |
| ✅ 1   | Reverse shell (basic C2 channel)          | `Sentinel_agent.cpp` + `Sentinel_server.cpp` | You’ve already done this                      |
| ✅ 2  | Add persistence                           | `Sentinel_agent.cpp`                         | Registry run key, Task Scheduler              |
| 🔜 3  | Encrypt communication                     | Both                                         | XOR / AES over socket                         |
| 🔜 4  | Modular command system (plugins/commands) | Agent + Server                               | Structured command handling                   |
| 🔜 5  | File upload/download                      | Agent                                        | Exfiltration support                          |
| 🔜 6  | Screenshot / webcam capture               | Agent (Windows API)                          | Real capabilities                             |
| 🔜 7  | AV Evasion techniques                     | Agent                                        | Obfuscation, API unhooking, string encryption |
| 🔜 8  | GUI or CLI C2 Client                      | Optional                                     | Better usability for operator                 |
