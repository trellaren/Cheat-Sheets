# yaml cheat sheet 📜

YAML (YAML Ain't Markup Language) is a human-friendly data serialization standard used to store configuration and structure data. Its primary feature is its extreme readability, making it ideal for configuration files (`.yml` or `.yaml`).

---

## 💡 Core Principles

### 1. Indentation is EVERYTHING

YAML uses **spaces** (not tabs!) to denote hierarchy. Consistency in indentation is mandatory.

- ❌ **Incorrect:** `key: value\n  nested_key:` (Mixing spaces and tabs)
- ✅ **Correct:** Use only two or four spaces consistently for every level of nesting.

### 2. Key-Value Pairs

The fundamental unit of YAML is a mapping (dictionary/object):
`key: value`

---

## 📚 Data Structures & Syntax

### 🏷️ Mappings (Objects / Dictionaries)

A set of unique key-value pairs. Represented by colons (`:`).

**Example:**

```yaml
user:
  name: John Doe # Key 'user' maps to this block
  age: 30 # Nested key under 'user'
  is_admin: true # Another nested key
```

### 🧺 Sequences (Arrays / Lists)

An ordered list of items. Represented by hyphens (`-`).

**Example:**

```yaml
books: # Key 'books' maps to this list
  - The Hobbit # Item 1
  - Foundation # Item 2
  - Dune # Item 3
```

### 🔄 Combining Maps and Sequences (Complex Data)

You can have lists of objects, or objects that contain lists. This is the most common advanced use case.

**Example: A list of users (each user is a dictionary)**

```yaml
users: # 'users' is a sequence/list
  - name: Alice # The first item in the list
    id: 101
    roles: [read, write] # List within an object
  - name: Bob # The second item in the list
    id: 202
    roles: [admin]
```

---

## 🔡 Data Types

YAML supports common programming data types. **No explicit type casting is usually needed.**

| Type        | Example Syntax                     | Description                                                    |
| :---------- | :--------------------------------- | :------------------------------------------------------------- |
| **String**  | `message: Hello World`             | Always strings unless quoted for special characters.           |
| **Integer** | `count: 123`                       | Whole numbers.                                                 |
| **Float**   | `pi: 3.14159`                      | Numbers with decimal points.                                   |
| **Boolean** | `active: true` / `inactive: false` | Accepts `true`, `false`, `yes`, `no`, etc. (Case insensitive). |
| **Null**    | `owner: null`                      | Represents no value (`None` in Python, `nil` in Ruby).         |

### 💬 Handling Strings

- **Plain:** Standard text (`key: plain string`)
- **Single Quotes:** Useful for strings that contain special characters (e.g., `'it\'s time'`).
- **Double Quotes:** Used when escaping is required, or to force specific interpretation (`"C:\\path\\to\\file"`).
- **Multi-line Block Scalar (Best for large blocks of text):** Uses `|` (literal block) or `>` (folded block).

```yaml
# 📘 Literal Block (|): Preserves newlines and indentation exactly.
description: |
  This is line one.
  This is line two, indented perfectly.

# 📄 Folded Block (>): Replaces newlines with spaces for cleaner reading.
email_body: >
  Dear user, this message wraps across multiple lines
  and will be treated as a single string chunk by the parser.
```

---

## ✨ Advanced Syntax & Tips

### 1. Anchors & Aliases (References)

Use these when you need to reuse a block of data structure multiple times, preventing repetition and keeping configurations DRY (Don't Repeat Yourself).

- **`&anchorName`**: Defines the reusable piece of data.
- **`*anchorName`**: Inserts the definition at that point.

```yaml
# Define a common set of default credentials
defaults: &credentials
  api_key: "DEFAULT_KEY"
  timeout: 30

database_a:
  <<: *credentials # Use the defined block here
  host: db-a.com

database_b:
  <<: *credentials # And use it again here
  host: db-b.com
```

### 2. Comments

Use `#` to add comments. They are completely ignored by the parser.

```yaml
# This is a comment explaining the following section.
server:
  port: 8080 # Comment on the same line
```

---

## 🚀 Quick Reference Summary Table

| Feature                         | YAML Syntax                 | Example                      | Resulting Structure (Conceptual) |
| :------------------------------ | :-------------------------- | :--------------------------- | :------------------------------- |
| **Key-Value**                   | `key: value`                | `name: WidgetA`              | `{ "name": "WidgetA" }`          |
| **List Item**                   | `- item_value`              | `- Apple - Banana`           | `[ "Apple", "Banana" ]`          |
| **Nested Object**               | Indentation +`:`            | `user: name: John`           | `{ "user": { "name": "John" } }` |
| **List of Objects**             | `-` followed by indentation | `- id: 1\n  name: A`         | `[ { "id": 1, "name": "A" } ]`   |
| **Multi-Line String (Literal)** | `key:                       | \n line 2`                   | `text:                           |
| **Reference/Reuse**             | `<<: *anchorName`           | `service: { <<: *defaults }` | (Replaces the whole block)       |
