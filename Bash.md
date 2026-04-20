# 🐚 **Bash Cheat Sheet**

_A quick reference for the most useful Bash features and idioms._

---

## 1️⃣ Shell Basics

| Topic                 | Command / Syntax  | Example                                      |
| --------------------- | ----------------- | -------------------------------------------- |
| **Run a command**     | `cmd [options]`   | `ls -la /tmp`                                |
| **Run in background** | `&`               | `sleep 60 &`                                 |
| **Pause for input**   | `read varname`    | `read name`                                  |
| **Exit with status**  | `exit [n]`        | `exit 1`                                     |
| **Print to stdout**   | `echo` / `printf` | `echo "Hello"` `<br>` `printf "%s\n" "$var"` |

---

## 2️⃣ Variables

| Type                           | Syntax                              | Notes                                                 |
| ------------------------------ | ----------------------------------- | ----------------------------------------------------- | --- |
| **Positional params**          | `$1`, `$2`, …, `$#`, `$@`, `$*`     | `$#` = number of args; `$@` = all quoted individually |     |
| **Environment variable**       | `export VAR="value"`                | Becomes available to child processes                  |
| **Local variable in function** | `local var=value`                   | Only visible inside the function                      |
| **Command substitution**       | `var=$(cmd)` or ``var=`cmd` ``      | Captures stdout of `cmd`                              |
| **Process ID**                 | `$!`                                | PID of last background job                            |
| **Shell options**              | `set -o option` / `shopt -s option` | e.g.,`set -e`, `shopt -s nullglob`                    |

---

## 3️⃣ Quoting & Expansion

| Rule                      | Effect                                        | Example                             |
| ------------------------- | --------------------------------------------- | ----------------------------------- | --- |
| **Single quotes `'...'`** | No expansion, literal text                    | `'${HOME}'` → `${HOME}`             |
| **Double quotes `"..."`** | Variable/command expansion, backslash escapes | `"$var"`                            |
| **No quotes**             | Word splitting & globbing                     | `echo *` expands to all files       |
| **Brace expansion**       | Generates list of strings                     | `{a,b,c}.txt` → `a.txt b.txt c.txt` |
| **Arithmetic expansion**  | `$(( expr ))`                                 | `echo $(( 5 + 3 ))` → `8`           |     |

---

## 4️⃣ Control Flow

### 4.1 Conditional Statements

```bash
if [[ condition ]]; then
    # true branch
elif [[ other_condition ]]; then
    # else-if branch
else
    # false branch
fi
```

| Condition               | Meaning                 |
| ----------------------- | ----------------------- |
| `[[ -f file ]]`         | Regular file exists     |
| `[[ -d dir ]]`          | Directory exists        |
| `[[ $var == pattern ]]` | Pattern match (`glob`)  |
| `[[ $var =~ regex ]]`   | Regex match (POSIX ERE) |

### 4.2 Loops

```bash
# For loop over words or numbers
for item in a b c; do
    echo "$item"
done

# While loop
while [[ condition ]]; do
    ...
done

# Until loop
until [[ condition ]]; do
    ...
done
```

### 4.3 Case Statement

```bash
case $var in
    pattern1) commands ;;
    pattern2|pattern3) commands ;;
    *) default_commands ;;
esac
```

---

## 5️⃣ Functions

```bash
my_func() {
    local arg1="$1"
    echo "Inside ${FUNCNAME[0]}"
}
```

- `return n` exits with status `n`.
- `$?` inside a function is the exit status of the last command executed.
- **Recursive calls**: just call by name.

---

## 6️⃣ File & Directory Operations

| Operation                  | Command                                       |
| -------------------------- | --------------------------------------------- |
| **Create file**            | `touch file.txt`                              |
| **Delete file**            | `rm file.txt`                                 |
| **Move/Rename**            | `mv old new`                                  |
| **Copy**                   | `cp -r src dest`                              |
| **Append to file**         | `>>`                                          |
| **Read file line by line** | `while IFS= read -r line; do ... done < file` |

---

## 6️⃣ Process Control

| Feature               | Syntax                     | Use‑case                       |
| --------------------- | -------------------------- | ------------------------------ |
| **List processes**    | `ps aux`                   | See all running jobs           |
| **Kill process**      | `kill PID` / `kill -9 PID` | Force kill                     |
| **Suspend/Resume**    | `Ctrl+Z`, `fg %jobid`      | Bring to foreground            |
| **Wait for job**      | `wait [PID]`               | Blocks until child exits       |
| **Job control flags** | `set -m` (monitor mode)    | Enables job control in scripts |

---

## 7️⃣ File Manipulation Helpers

```bash
# Remove all files matching pattern
rm /tmp/*.log

# Find files recursively
find . -type f -name "*.sh" -exec chmod +x {} \;

# Copy directory tree preserving attributes
cp -a src/ dest/

# Read entire file into variable (no newline)
file_contents=$(<filename)

# Append multiple lines safely
cat >>file <<'EOF'
line1
line2
EOF
```

---

## 8️⃣ Text Processing

| Tool     | Key options                                                                                          |
| -------- | ---------------------------------------------------------------------------------------------------- |
| **grep** | `-i` ignore case `<br>` `-v` invert match `<br>` `-E` extended regex `<br>` `--color=auto` highlight |
| **sed**  | `s/pattern/repl/` replace `<br>` `/path/to/file` editing in place with `-i`                          |
| **awk**  | `$0`, `$1`, … ; `{print $2}`                                                                         |
| **cut**  | `-d delimiter -f fields`                                                                             |

---

## 9️⃣ Input / Output Redirection

| Operator    | Meaning                       | Example                      |
| ----------- | ----------------------------- | ---------------------------- |
| `>`         | Overwrite file                | `echo "x" > out.txt`         |
| `>>`        | Append to file                | `echo "y" >> out.txt`        |
| `<`         | Read from file as stdin       | `cmd < input.txt`            |
| `2>`, `2>>` | Redirect stderr               | `ls nonexistent 2>/dev/null` |
| `&>`        | Redirect both stdout & stderr | `./run.sh &>log.txt`         |

---

## 🔗 Networking / Utilities

| Utility  | Key options                                             |
| -------- | ------------------------------------------------------- |
| **curl** | `-L` follow redirects `<br>` `-O` save with remote name |
| **wget** | `-q` quiet `<br>` `--recursive` recursive download      |
| **ssh**  | `ssh user@host 'cmd'`                                   |
| **scp**  | `scp localfile user@host:/path/`                        |

---

## 📐 Regular Expressions (POSIX ERE)

```bash
[[ $var =~ ^[0-9]+$ ]]   # numeric string
```

- Capture groups: `${BASH_REMATCH[1]}`, `${BASH_REMATCH[2]}` …
- `=` inside `[[` is pattern matching; use `=~` for regex.

---

## 🔧 Common Flags & Options

| Flag                | Effect                                   |
| ------------------- | ---------------------------------------- |
| `set -e`            | Exit on any command with non‑zero status |
| `set -u`            | Treat unset variables as error           |
| `set -x`            | Print commands before executing (debug)  |
| `shopt -s nullglob` | Empty glob patterns expand to nothing    |
| `shopt -s dotglob`  | Include hidden files in globs            |

---

## 📚 Quick Reference Commands

```bash
# File system
ls -alh /var/log
mkdir -p /tmp/foo/bar
rm -rf /tmp/old

# Text search & replace
grep -i "error" logfile.txt
sed -n '5,10p' file.txt
awk '/pattern/{print $1}' data.txt

# Process handling
ps aux | grep myscript.sh
kill $(pgrep -f myscript.sh)
wait $!

# Networking
curl -I https://example.com
wget --spider http://localhost:8080/health

# Arithmetic
sum=$((a + b))
factorial() { local n=$1; echo $((n * factorial(n-1))) }
```

---

### 📌 Tip Sheet Formatting

Use **bold** for commands, **italics** for options, and code blocks for multi‑line snippets. Keep this sheet handy—copy it to a `cheat.sh` file in your home directory and source it from any script:

```bash
source ~/cheat.sh
```

Happy scripting! 🚀
