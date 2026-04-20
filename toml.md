# 📜 TOML Cheat Sheet: Tom's Obvious, Minimal Language

TOML is a configuration file format designed to be easy for humans to read while remaining simple enough for machines to parse reliably. It is widely used for application configurations (e.g., `pyproject.toml`, `Cargo.toml`).

---

## ✨ 💡 Key Concepts & Syntax

| Feature               | Syntax                      | Description                                                                                          |
| :-------------------- | :-------------------------- | :--------------------------------------------------------------------------------------------------- |
| **Key-Value Pair**    | `key = value`               | The basic building block of TOML.                                                                    |
| **Comments**          | `# This is a comment`       | Any line starting with `#` is ignored.                                                               |
| **Tables (Sections)** | `[table_name]`              | Groups related key-value pairs, like a dictionary or object in other languages.                      |
| **Nested Tables**     | `[parent.child]`            | Used to create hierarchical sections (e.g.,`[database.sql]`).                                        |
| **Arrays**            | `key = [item1, item2, ...]` | A list of values for a single key.                                                                   |
| **Array of Tables**   | `[[array_name]]`            | Used when you have multiple instances of the same structured object (e.g., multiple users, servers). |

---

## 💾 Data Types Reference

TOML supports several native data types:

### 1. Strings (Text)

- **Basic:** Use double quotes (`""`).
  ```toml
  title = "My Application Title"
  ```
- **Multi-line:** Use triple quotes (`"""`). This allows the string to span multiple lines without escaping characters.
  ```toml
  description = """
  A complex description
  that spans multiple lines.
  """
  ```

### 2. Numbers (Integers & Floats)

- **Integer:** Whole numbers. No quotes needed.
  ```toml
  max_users = 100
  ```
- **Float:** Decimal numbers. No quotes needed.
  ```toml
  pi_value = 3.14159
  ```

### 3. Booleans (Truth Values)

- Use lowercase `true` or `false`.
  ```toml
  is_active = true
  logging_enabled = false
  ```

### 4. Datetime

- TOML uses the standard [RFC 3339](https://datatracker.ietf.org/doc/html/rfc3339) format: `YYYY-MM-DDTHH:MM:SSZ`.
  ```toml
  start_date = 2024-10-27T10:00:00Z  # Date and Time (UTC recommended)
  ```

---

## 🧩 Complex Structures Examples

### 📂 Tables & Sections

Grouping related settings.

```toml
[database]
host = "localhost"
port = 5432
user = "admin_user"

[server]
ip = "192.168.1.1"
timeout_ms = 5000
```

### 🔗 Nested Tables (Sub-Sections)

Used for highly structured data, like database connection details.

```toml
# This table holds configuration specifically for SQL databases
[database.sql]
driver = "sqlite"
schema_version = 2

# A sub-section within the 'server' group
[server.cache]
enabled = true
ttl = 3600 # Time to live in seconds
```

### 🔢 Arrays (Lists of Values)

A single key with multiple values, separated by commas and enclosed in brackets.

```toml
# List of allowed IP addresses
allowed_ips = [
    "192.168.1.1",
    "10.0.0.5"
]

# A list of numeric settings
port_ranges = [78, 443, 8080]
```

### 🚀 Array of Tables (The Advanced Feature)

When you need multiple instances of the _same_ structured object (e.g., multiple users, multiple endpoints). **You must use double brackets `[[ ]]`**.

```toml
# Defining two different user accounts
[[users]]
username = "alice"
role = "administrator"
email = "alice@example.com"

[[users]]
username = "bob"
role = "guest"
email = "bob@example.com"


# Example of an array of servers (each server has its own sub-settings)
[[servers]]
name = "main_api"
ip = "127.0.0.1"

[[servers]]
name = "backup_storage"
ip = "192.168.2.254"
```

---

## 🛠️ Quick Reference Summary

| Goal                    | TOML Code                                                            | Interpretation                                 |
| :---------------------- | :------------------------------------------------------------------- | :--------------------------------------------- |
| **Simple Key**          | `setting = value`                                                    | Sets a basic configuration option.             |
| **Group Settings**      | `[section]` `<br>` `key = value`                                     | Groups related settings (e.g.,`[network]`).    |
| **Hierarchy**           | `[group.sub_group]` `<br>` `key = value`                             | Creates nested settings (e.g.,`[server.ssl]`). |
| **List of Items**       | `list = [a, b, c]`                                                   | Assigns multiple values to one key.            |
| **Multiple Structures** | `[[array]]` `<br>` ... keys... `<br>` `[[array]]` `<br>` ... keys... | Defines repeating objects (e.g., two servers). |
