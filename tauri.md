This cheat sheet covers the essential concepts, commands, and code patterns for **Tauri** (focused on the stable v1/v2 paradigms).

---



## 🚀 Core Architecture
*   **Frontend:** Any Web Framework (React, Vue, Svelte, Vanilla JS) running in a native WebView.
*   **Backend:** Rust. Handles system access, file I/O, and heavy computation.
*   **IPC (Inter-Process Communication):** The bridge that allows the Frontend to call Rust functions (`invoke`) and both sides to send messages (`events`).

---

## 🛠️ Getting Started
### Installation & Setup
```bash
# Create a new project (Interactive wizard)
npm create tauri-app@latest

# Navigate to project
cd my-tauri-app

# Install dependencies
npm install

# Run in development mode
npm run tauri dev
```

### Useful CLI Commands
| Command | Description |
| :--- | :--- |
| `npm run tauri dev` | Starts the app in dev mode with hot reload. |
| `npm run tauri build` | Compiles and bundles the app for production. |
| `npm run tauri info` | Prints system/environment information (great for debugging). |

---

## 🔄 Communication (The Bridge)

### 1. Commands (Frontend $\to$ Backend)
Use this when you want to trigger a specific function in Rust and get a result back (like an API call).

**Rust (`src-tauri/src/main.rs`):**
```rust
#[tauri::command]
fn greet(name: &str) -> String {
    format!("Hello, {}! You've been greeted from Rust!", name)
}

fn main() {
    tauri::Builder::default()
        .invoke_handler(tauri::generate_handler![greet]) // Register command
        .run(tauri::generate_context!())
        .expect("error while running tauri application");
}
```

**JavaScript/TypeScript:**
```javascript
import { invoke } from '@tauri-apps/api/tauri';

// Calling the command
const response = await invoke('greet', { name: 'World' });
console.log(response); // "Hello, World!..."
```

### 2. Events (Async Broadcast)
Use this for one-way notifications or streaming data (e.g., progress bars, hardware sensors).

**Rust $\to$ Frontend (Emit):**
```rust
use tauri::Manager;

#[tauri::command]
fn start_long_process(window: tauri::Window) {
    // Emit an event to the frontend
    window.emit("download-progress", 42).unwrap();
}
```

**Frontend $\to$ Rust (Listen):**
```javascript
import { listen } from '@tauri-apps/api/event';

const unlisten = await listen('download-progress', (event) => {
  console.log(`Progress: ${event.payload}%`);
});

// Call unlisten() when component unmounts to prevent memory leaks
```

---

## ⚙️ Configuration (`tauri.conf.json`)
The heart of your app's settings.

| Key | Purpose |
| :--- | :--- |
| `identifier` | Unique ID for your app (e.g., `com.myapp.dev`). **Must be unique.** |
| `window` | Configures window size, title, and visibility. |
| `allowlist` | **Crucial Security.** Controls which Rust APIs are exposed to JS (Filesystem, Shell, etc.). |
| `bundle` | Settings for installers (`.deb`, `.msi`, `.app`). |
| `security` $\to$ `csp` | Content Security Policy to prevent XSS attacks. |

**Example Allowlist Snippet:**
```json
"allowlist": {
  "fs": {
    "all": true,
    "scope": ["$APPCONFIG/*"]
  },
  "shell": {
    "open": true
  }
}
```

---

## 🔒 Security Best Practices
1.  **Principle of Least Privilege:** Only enable the `allowlist` features you absolutely need.
2.  **Scoped Filesystem Access:** Never use `"all": true` for filesystems in production. Use specific paths (e.g., `$APPDATA`).
3.  **Validate Input:** Always treat data coming from the Frontend (`invoke` arguments) as untrusted/malicious in your Rust code.
4.  **CSP:** Implement a strict `Content-Security-Policy` to prevent unauthorized scripts from running in your WebView.

---

## 📦 Common Plugins & APIs
*   **`@tauri-apps/api/fs`**: File system operations (Read/Write files).
*   **`@tauri-apps/api/shell`**: Execute system commands or open URLs.
*   **`@tauri-apps/api/dialog`**: Native Open/Save file dialogs.
*   **`@tauri-apps/api/http`**: Make network requests (bypassing CORS issues).
*   **`@tauri-apps/api/window`**: Programmatically resize or move windows.

---

## 🛠️ Debugging Tips
*   **Inspect Frontend:** Right-click in the app window $\to$ **Inspect Element** (just like Chrome).
*   **Check Rust Errors:** Look at your terminal where `npm run tauri dev` is running.
*   **`tauri info`**: Run this if you have issues with dependencies, compilers, or path configurations.
*   **Panic!**: If the Rust backend crashes, it will "panic." Check the terminal for the stack trace.
