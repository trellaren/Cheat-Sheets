## Docker Cheat Sheet (Linux/Windows/macOS)

> **Tip:** Use `docker --help` or `docker COMMAND --help` for full documentation.

---

### 1️⃣ Quick Commands

| Purpose                                    | Command                               | Example                                 |
| ------------------------------------------ | ------------------------------------- | --------------------------------------- |
| **List all containers**                    | `docker ps -a`                        | Show running & stopped                  |
| **Run a container (detached)**             | `docker run -d --name <name> <image>` | `docker run -d --name web nginx:latest` |
| **Stop a container**                       | `docker stop <id                      | name>`                                  |
| **Remove a container**                     | `docker rm <id                        | name>`                                  |
| **View logs**                              | `docker logs <id                      | name>`                                  |
| **Execute shell inside running container** | `docker exec -it <id                  | name> bash`                             |
| **Pull an image**                          | `docker pull <image>`                 | `docker pull postgres:13-alpine`        |
| **Build an image from Dockerfile**         | `docker build -t <tag> .`             | `docker build -t myapp:v1 .`            |
| **List images**                            | `docker images`                       |                                         |
| **Remove image**                           | `docker rmi <image>`                  | `docker rmi nginx:latest`               |

---

### 2️⃣ Common Flags

| Flag                            | Meaning                               | Example                           |
| ------------------------------- | ------------------------------------- | --------------------------------- |
| `-d`                            | Detached mode (run in background)     | `docker run -d nginx`             |
| `--name`                        | Assign a name to the container        | `docker run --name mydb postgres` |
| `-p <host>:<container>`         | Port mapping                          | `-p 8080:80`                      |
| `-v /host/path:/container/path` | Volume bind‑mount                     | `-v $(pwd)/data:/app/data`        |
| `--rm`                          | Auto‑remove container when it exits   | `docker run --rm alpine echo hi`  |
| `-e VAR=value`                  | Environment variable                  | `-e POSTGRES_PASSWORD=secret`     |
| `-t`                            | Allocate a pseudo‑TTY (use with `-i`) | `docker exec -it <id> bash`       |

---

### 3️⃣ Dockerfile Basics

```Dockerfile
# Base image
FROM node:18-alpine

# Working directory inside container
WORKDIR /app

# Copy package files & install deps
COPY package*.json ./
RUN npm ci --only=production

# Copy rest of app code
COPY . .

# Expose port (optional)
EXPOSE 3000

# Default command
CMD ["node", "index.js"]
```

**Key instructions**

| Instruction          | Purpose                             |
| -------------------- | ----------------------------------- |
| `FROM`               | Base image                          |
| `WORKDIR`            | Set working dir                     |
| `COPY` / `ADD`       | Copy files into image               |
| `RUN`                | Execute shell commands during build |
| `ENV`                | Set environment variables           |
| `EXPOSE`             | Document port usage (no mapping)    |
| `CMD` / `ENTRYPOINT` | Default container command           |

---

### 4️⃣ Docker Compose (YAML)

```yaml
version: "3.9"
services:
  web:
    build: .
    ports:
      - "3000:3000"
    volumes:
      - .:/app
    environment:
      NODE_ENV: production

  db:
    image: postgres:15-alpine
    restart: always
    environment:
      POSTGRES_USER: user
      POSTGRES_PASSWORD: pass
      POSTGRES_DB: mydb
    volumes:
      - db-data:/var/lib/postgresql/data

volumes:
  db-data:
```

**Commands**

| Command                        | What it does                                |
| ------------------------------ | ------------------------------------------- |
| `docker compose up`            | Start services (detached if `-d`)           |
| `docker compose down`          | Stop & remove containers, networks, volumes |
| `docker compose logs -f`       | Tail logs                                   |
| `docker compose exec web bash` | Exec into a running service                 |

---

### 5️⃣ Networking

| Command                                       | Purpose                     |
| --------------------------------------------- | --------------------------- |
| `docker network ls`                           | List networks               |
| `docker network inspect <name>`               | Show details                |
| `docker network create --driver bridge mynet` | Create custom network       |
| `--network mynet` (in run/compose)            | Attach container to network |

**Default networks**

- `bridge`: Default for standalone containers
- `host`: No isolation (use with caution)
- `none`: Disable networking

---

### 6️⃣ Volumes & Persistence

| Command                           | Purpose             |
| --------------------------------- | ------------------- |
| `docker volume ls`                | List volumes        |
| `docker volume inspect <name>`    | Show details        |
| `docker volume create myvol`      | Create a new volume |
| `-v myvol:/data` (in run/compose) | Mount named volume  |

**Best practice:** Use Docker Compose for local dev; keep data in named volumes.

---

### 7️⃣ Image & Container Management

```bash
# Show detailed image info
docker image inspect <image>

# List all images with size
docker images --format "table {{.Repository}}\t{{.Tag}}\t{{.Size}}"

# Clean up dangling (unused) objects
docker system prune -a

# Export an image to tarball
docker save -o myapp.tar myapp:latest

# Import from tarball
docker load -i myapp.tar
```

---

### 8️⃣ Security & Best Practices

| Tip                              | Explanation                       |
| -------------------------------- | --------------------------------- |
| **Use multi‑stage builds**       | Keep runtime images lean.         |
| **Specify `USER` in Dockerfile** | Avoid running as root.            |
| **Limit container resources**    | `--memory`, `--cpus`.             |
| **Remove build caches**          | `docker builder prune`.           |
| **Pin image tags**               | Don’t use `latest` in production. |

---

### 9️⃣ Troubleshooting

| Problem                     | Command / Fix                                                                                           |
| --------------------------- | ------------------------------------------------------------------------------------------------------- |
| Container exits immediately | Check logs (`docker logs`) or inspect exit code (`docker inspect --format='{{.State.ExitCode}}' <id>`). |
| Port conflict               | Find process on host port:`sudo lsof -i :<port>`. Kill or change mapping.                               |
| Image not found             | Ensure correct registry and tag; login if private (`docker login`).                                     |
| Slow build                  | Use `.dockerignore` to exclude unnecessary files.                                                       |

---

### 10️⃣ Handy Aliases (add to ~/.bashrc or ~/.zshrc)

```bash
alias dps='docker ps'
alias dpsa='docker ps -a'
alias drm='docker rm $(docker ps -aq)'
alias dri='docker rmi $(docker images -q)'
alias drmvol='docker volume prune -f'
```

---

### 11️⃣ Quick Reference Cheat Sheet (Printed)

| Section         | Key Commands                                               |
| --------------- | ---------------------------------------------------------- |
| **Run**         | `docker run -d --name <n> -p 80:80 -v .:/app <img>`        |
| **Build**       | `docker build -t myapp:v1 .`                               |
| **Compose**     | `docker compose up -d && docker compose logs -f`           |
| **Logs**        | `docker logs -f <id>`                                      |
| **Exec**        | `docker exec -it <id> bash`                                |
| **Stop/Remove** | `docker stop <id>; docker rm <id>`                         |
| **Pull / Push** | `docker pull repo/image:tag`; `docker push repo/image:tag` |

Print this table, keep it handy while you work!
