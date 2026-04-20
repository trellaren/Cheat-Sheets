# 🚀 Node.js Cheat Sheet

> Quick reference for the most used concepts, APIs and tools in a Node.js project.

---

## 1️⃣ Project Setup

| Task                              | Command                                                            |
| --------------------------------- | ------------------------------------------------------------------ |
| Create package.json (interactive) | `npm init`                                                         |
| Install local dev dependency      | `npm install <pkg> --save-dev`                                     |
| Install global binary             | `npm i -g <pkg>`                                                   |
| Add scripts                       | `"scripts": { "start":"node index.js", "dev":"nodemon index.js" }` |
| Run script                        | `npm run <script-name>`                                            |
| Check Node & npm versions         | `node -v && npm -v`                                                |

---

## 2️⃣ Common Modules

| Module               | Purpose                                | Quick Usage                                            |
| -------------------- | -------------------------------------- | ------------------------------------------------------ |
| **fs** (File System) | Read/write files, streams              | `const fs = require('fs');<br>``fs.readFileSync(path)` |
| **path**             | Path utilities                         | `path.join(__dirname, 'file.txt')`                     |
| **http / https**     | Low‑level server/client                | `const http = require('http');`                        |
| **url**              | URL parsing                            | `new URL(req.url, base).pathname`                      |
| **events**           | EventEmitter                           | `class MyEmitter extends EventEmitter {}`              |
| **util**             | Misc helpers (`promisify`, `inherits`) | `const { promisify } = require('util');`               |
| **child_process**    | Run shell commands                     | `execSync('ls -l')`                                    |
| **os**               | OS info (platform, cpus)               | `os.platform()`                                        |
| **crypto**           | Cryptography                           | `crypto.randomBytes(16).toString('hex')`               |

---

## 3️⃣ File System – async vs sync

```js
// Async (callback)
fs.readFile(file, "utf8", (err, data) => {
  /* ... */
});

// Async (Promise)
const read = util.promisify(fs.readFile);
await read(file, "utf8");

// Sync
const data = fs.readFileSync(file, "utf8");
```

_Prefer async in production; sync is handy for scripts._

---

## 4️⃣ HTTP Server

```js
const http = require("http");

const server = http.createServer((req, res) => {
  const { method, url } = req;
  if (url === "/" && method === "GET") {
    res.writeHead(200, { "Content-Type": "text/plain" });
    res.end("Hello World");
  }
});

server.listen(process.env.PORT || 3000, () =>
  console.log(`Listening on port ${process.env.PORT || 3000}`),
);
```

---

## 5️⃣ Event Loop & Async Patterns

| Pattern          | Description       | Example                                          |
| ---------------- | ----------------- | ------------------------------------------------ |
| **Callbacks**    | Traditional async | `fs.readFile(..., (err, data) => {})`            |
| **Promises**     | Chainable async   | `fetch(url).then(r=>r.json()).catch(e=>{})`      |
| **async/await**  | Linear syntax     | `const data = await fetch(url)`                  |
| **EventEmitter** | Pub/Sub           | `emitter.on('data', handler)`                    |
| **Streams**      | Chunked IO        | `fs.createReadStream(file).pipe(process.stdout)` |

---

## 6️⃣ Environment Variables

```js
process.env.PORT || 3000; // default fallback
require("dotenv").config(); // load .env file
```

`.env` example:

```
PORT=4000
DB_URL=mongodb://localhost:27017/db
```

---

## 7️⃣ Common CLI Commands

| Tool                     | Use                          |
| ------------------------ | ---------------------------- |
| `node`                   | Run JS files                 |
| `npm run dev`            | Start nodemon (auto‑restart) |
| `npx nodemon index.js`   | One‑off nodemon              |
| `npx eslint .`           | Lint                         |
| `npx prettier --write .` | Format                       |
| `npx jest`               | Run tests                    |
| `npx tsc`                | Compile TypeScript           |

---

## 8️⃣ Debugging

- **Node inspector**: `node --inspect-brk index.js`
- **Chrome DevTools**: open `chrome://inspect/#devices`
- **Logging**: `console.log`, `util.inspect(obj, {depth:null})`
- **Process warnings**: `process.on('warning', w => console.warn(w.stack))`

---

## 9️⃣ Package Management

| Command             | What it does                  |
| ------------------- | ----------------------------- |
| `npm install <pkg>` | Install & add to dependencies |
| `npm i -D <pkg>`    | Add as devDependency          |
| `npm update`        | Update all packages           |
| `npm audit`         | Security scan                 |
| `npm dedupe`        | Remove duplicate packages     |

---

## 🔧 Miscellaneous Tips

- **Use ES modules** (`import`, `.mjs`) if you need top‑level await.
- **Keep your event loop free** – avoid CPU‑heavy sync code in request handlers.
- **Memory leaks**: watch for unbounded timers, listeners or large buffers.
- **Performance**: use `async_hooks` to trace async resources (advanced).

---

Happy coding! 🎉
_(Feel free to print this cheat sheet and keep it next to your terminal.)_
