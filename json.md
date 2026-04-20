This cheat sheet is designed as a highly structured reference guide, using markdown for readability while strictly adhering to all valid **JSON (JavaScript Object Notation)** rules.

---

# 📘 JSON Cheat Sheet Reference Guide

## 📜 Overview & Philosophy

**What is JSON?**
A lightweight data-interchange format used for transmitting structured data between web applications and servers. It is based on JavaScript object syntax but is language-agnostic.

**Core Rule:** Data must be represented as **key-value pairs**.

- `"Key"`: Must always be a string (enclosed in double quotes).
- `: `: Separates the key from its value.
- `,` : Separates one pair/element from the next.
- **Crucial Note:** There should **never** be a comma after the last element of an object or array.

---

## 🧱 I. The Building Blocks (Data Types)

JSON supports six basic data types. All strings _must_ use double quotes (`"`).

| Data Type   | Representation                | Syntax Example         | Notes                                                                                                            |
| :---------- | :---------------------------- | :--------------------- | :--------------------------------------------------------------------------------------------------------------- |
| **String**  | Textual data                  | `"Hello World"`        | Must be enclosed in double quotes. Handles Unicode and escaped characters (e.g.,`\n`, `\\`).                     |
| **Number**  | Integer or Float              | `42` or `3.14159`      | No quotes allowed. JSON does not support specific data types like dates; they are treated as strings or numbers. |
| **Boolean** | Truth value                   | `true` / `false`       | Must be lowercase. No quotes allowed.                                                                            |
| **Null**    | Absence of Value              | `null`                 | Must be lowercase. No quotes allowed. Represents an empty or unknown value.                                      |
| **Object**  | Collection of key/value pairs | `{ "key": "value" }`   | Used for structured data (like a dictionary or record). Defined by curly braces `{}`.                            |
| **Array**   | Ordered list of values        | `[ "item1", 2, true ]` | Used for lists of items. Defined by square brackets `[]`.                                                        |

---

## 📦 II. Structural Elements (Containers)

### 🔑 A. JSON Object (`{}`)

Objects are unordered collections of key-value pairs. Every piece of data inside an object must follow the structure: `"string_key": value`.

**Syntax Rules:**

1. Must start and end with `{}`.
2. Keys **must** be strings (in quotes).
3. Key/Value pairs are separated by colons (`:`).
4. Pairs are separated by commas (`,`).

#### 💡 Object Example: User Profile

```json
{
  "userId": 101, // Key is string "userId", value is number
  "username": "DataAnalyst", // Both key and value are strings
  "isActive": true, // Key is string "isActive", value is boolean
  "lastLogin": null, // Represents missing or empty data
  "attributes": {
    // Value can be another nested object
    "city": "Seattle",
    "zipCode": 98101
  }
}
```

### 🧺 B. JSON Array (`[]`)

Arrays are ordered lists of values. The items in the array can be any valid JSON data type, and they do not need to share a common type (heterogeneous).

**Syntax Rules:**

1. Must start and end with `[]`.
2. Items are separated by commas (`,`).
3. The elements themselves do **not** need quotes unless they are strings.

#### 💡 Array Example: Product Catalog

```json
[
  {
    // Object element 1
    "id": "A45",
    "name": "Laptop Charger"
  },
  "Mouse Pad", // String element 2 (simple value)
  3.99, // Number element 3 (simple value)
  ["USB-C", "Bluetooth"] // Array element 4 (nested array)
]
```

---

## ⚙️ III. Quick Reference Guide Table

| Scenario             | Structure Used         | Example JSON                       | Explanation                                                                |
| :------------------- | :--------------------- | :--------------------------------- | :------------------------------------------------------------------------- |
| **Simple Object**    | Key/Value Pairs (`{}`) | `{"key": "value", "numKey": 123}`  | Basic structure. Keys must be strings.                                     |
| **Simple Array**     | Ordered List (`[]`)    | `["apple", "banana", true]`        | Simple list of heterogeneous data types.                                   |
| **Nested Object**    | `{}` inside a value    | `{"user": {"age": 30}}`            | Used when one key's value is an entire object.                             |
| **Array of Objects** | `[]` containing `{}`   | `[ {"item": "A"}, {"item": "B"} ]` | Most common structure: a list where each element is its own record/object. |
| **Complex Example**  | Combination            | (See full example below)           | Demonstrates combining all types and structures.                           |

### 🎯 Full Complex Example (The Best Practice):

This represents an array of book records, demonstrating deep nesting and various data types:

```json
[
  {
    "isbn": "978-0321765723",
    "title": "JSON Fundamentals",
    "author": "John Doe",
    "publishedYear": 2020,
    "isAvailable": true,
    "genres": ["Technology", "Coding"],
    "reviewRating": null
  },
  {
    "isbn": "978-1234567890",
    "title": "The Great Novel",
    "author": "Jane Smith",
    "publishedYear": 1995,
    "isAvailable": false,
    "genres": ["Fiction"],
    "reviewRating": 4.5
  }
]
```

---

## ⚠️ IV. Common Pitfalls & Errors (DO NOT DO)

| Error                   | Incorrect JSON Syntax            | Why it is wrong                                                | Correct Syntax                  |
| :---------------------- | :------------------------------- | :------------------------------------------------------------- | :------------------------------ |
| **Missing Quotes**      | `{ key: "value" }`               | Keys and strings MUST be double-quoted.                        | `{"key": "value"}`              |
| **Trailing Comma**      | `"key1": "val", "key2": "val2",` | No comma allowed after the last element in an object or array. | `"key1": "val", "key2": "val2"` |
| **Unquoted Keys**       | `{ key: 5 }`                     | All keys must be strings (double-quoted).                      | `{"key": 5}`                    |
| **Using Single Quotes** | `{'key': 'value'}`               | JSON standard only supports double quotes (`"`) for strings.   | `{"key": "value"}`              |
