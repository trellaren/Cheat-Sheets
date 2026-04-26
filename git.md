This Git cheat sheet is organized by workflow stages, from setting up a repository to collaborating with others and fixing mistakes.

---

### ⚙️ 1. Setup & Configuration
Configure your identity and initialize new repositories.

| Command | Description |
| :--- | :--- |
| `git init` | Initialize a new local Git repository in the current folder. |
| `git clone <url>` | Download an existing repository from a remote source (like GitHub). |
| `git config --global user.name "Your Name"` | Set the name that will be attached to your commits. |
| `git config --global user.email "you@example.com"` | Set the email attached to your commits. |
| `git config --list` | Show all current Git configurations. |

---

### 🔄 2. The Basic Workflow (Daily Use)
The cycle of making changes, staging them, and saving them.

| Command | Description |
| :--- | :--- |
| `git status` | See which files are modified, staged, or untracked. |
| `git add <file>` | Add a specific file to the "Staging Area." |
| `git add .` | Add **all** changed files to the staging area. |
| `git commit -m "message"` | Save your staged changes with a descriptive message. |
| `git commit --amend` | Edit the last commit (add forgotten files or change message). |
| `git diff` | Show the exact line changes in unstaged files. |

---

### 🌿 3. Branching & Merging
Managing different versions/features of your project.

| Command | Description |
| :--- | :--- |
| `git branch` | List all local branches in your repository. |
| `git branch <name>` | Create a new branch. |
| `git switch <name>` | Switch to a specific branch (Modern alternative to `checkout`). |
  *Note: Use `git switch -c <name>` to create and switch at once.* |
| `git merge <branch-name>` | Merge the specified branch into your *current* branch. |
| `git branch -d <name>` | Delete a branch that has been merged. |
| `git log --oneline` | Show a simplified, one-line history of commits. |

---

### ☁️ 4. Remote Repositories
Syncing your local work with a server (GitHub, GitLab, Bitbucket).

| Command | Description |
| :--- | :--- |
| `git remote add origin <url>` | Connect your local repo to a remote server. |
| `git push -u origin <branch>` | Push changes to remote and "set upstream" (tracks branch). |
| `git push` | Push your current branch to the remote repository. |
| `git pull` | Fetch changes from remote AND merge them into your current branch. |
| `git fetch` | Download history from remote **without** changing your local files. |
| `git remote -v` | List all remote connections and their URLs. |

---

### 🛠️ 5. Undoing Changes & Fixing Mistakes
Because everyone makes mistakes. **Use with caution!**

| Command | Description |
| :--- | :--- |
| `git checkout -- <file>` | Discard changes in a specific file (revert to last commit). |
| `git reset HEAD <file>` | Unstage a file but keep the actual changes in the file. |
| `git reset --soft HEAD~1` | Undo the last commit, but **keep** your changes staged. |
| `git reset --hard HEAD~1` | **DANGER:** Completely delete the last commit and all changes. |
| `git revert <commit-id>` | Create a *new* commit that does the exact opposite of a previous commit (safe). |
| `git stash` | Temporarily "hide" uncommitted changes to clean your workspace. |
| `git stash pop` | Bring back your hidden (stashed) changes. |

---

### 🔍 6. Inspection & Comparison
Looking into the history and differences.

| Command | Description |
| :--- | :--- |
| `git log` | Show the full commit history. |
| `git log --graph --oneline --all` | View a visual "tree" of all branches and commits. |
| `git diff <branch1> <branch2>` | Compare the differences between two different branches. |
| `git show <commit-id>` | Show the detailed changes made in a specific commit. |

---

### 💡 Pro Tips for Success
1.  **Commit Small, Commit Often:** Large commits are hard to debug and review.
2.  **Write Good Messages:** Use the imperative mood (e.g., *"Fix login bug"* instead of *"Fixed login bug"* or *"I fixed the bug"*).
3.  **Pull Before You Push:** Always run `git pull` before pushing to ensure you have the latest updates from your team and avoid merge conflicts.
4.  **The `.gitignore` File:** Create a file named `.gitignore` in your root folder to tell Git which files (like `node_modules`, `.env`, or logs) should **never** be tracked.
