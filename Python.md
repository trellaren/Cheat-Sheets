# Python Cheat Sheet

_(Python 3.x – 2026)_

> Quick reference for the most common syntax, functions and idioms you’ll use every day.

---

## Table of Contents

1. [Basics](#basics)
2. [Data Types &amp; Structures](#data-types-structures)
3. [Control Flow](#control-flow)
4. [Functions &amp; Lambdas](#functions-lambdas)
5. [Modules &amp; Packages](#modules-packages)
6. [Object‑Oriented Programming](#oop)
7. [File I/O](#file-io)
8. [List/Dict Comprehensions](#comprehensions)
9. [Functional Tools](#functional-tools)
10. [Regular Expressions](#regex)
11. [Date &amp; Time](#datetime)
12. [Error Handling](#error-handling)
13. [Miscellaneous Utilities](#misc-utilities)

---

## 1. Basics

| Item                 | Syntax                                | Example                                   |
| -------------------- | ------------------------------------- | ----------------------------------------- |
| Print                | `print()`                             | `print("Hello, world!")`                  |
| Comments             | `# single line`, `'''multi\nline'''`  | `# TODO: refactor`                        |
| Variables            | No declaration needed                 | `x = 42`                                  |
| String interpolation | f‑strings (`f"{}"`), `.format()`, `%` | `name="Alice"; print(f"{name} is {age}")` |
| Escape sequences     | `\n, \t, \\`                          | `"Line1\nLine2"`                          |

---

## 2. Data Types & Structures

```python
# Numbers
int_var   = 7          # int
float_var = 3.14       # float
complex_var = 2+3j     # complex

# Booleans
truthy = True
falsy  = False

# Strings (immutable)
s = "hello"

# Lists (mutable, ordered)
lst = [1, 2, 3]
lst.append(4)

# Tuples (immutable, ordered)
tpl = (1, 2, 3)

# Sets (unordered, unique)
st = {1, 2, 3}
st.add(4)

# Dictionaries (key‑value)
dct = {"a": 1, "b": 2}
dct["c"] = 3
```

---

## 3. Control Flow

| Construct                   | Syntax                                   | Example                             |
| --------------------------- | ---------------------------------------- | ----------------------------------- |
| `if`                        | `if cond: ... elif other: ... else: ...` | `if x > 0: print("positive")`       |
| `for`                       | `for item in iterable:`                  | `for i in range(5):`                |
| `while`                     | `while condition:`                       | `while count < 10:`                 |
| `break`, `continue`, `pass` | -                                        | `if stop: break`                    |
| List comprehension          | `[expr for var in iter if cond]`         | `[x*2 for x in range(5) if x%2==0]` |

---

## 4. Functions & Lambdas

```python
def greet(name, greeting="Hello"):
    """Return a greeting string."""
    return f"{greeting}, {name}!"

# Lambda (anonymous)
double = lambda x: x * 2

# Default arguments are evaluated once at definition time
def counter(start=0):
    count = start
    def inc():
        nonlocal count
        count += 1
        return count
    return inc
```

---

## 5. Modules & Packages

```python
import math          # whole module
from os import path  # specific names
import numpy as np   # alias

# Dynamic import
module_name = "json"
mod = __import__(module_name)

# In a package: `__init__.py` can expose symbols
```

---

## 6. Object‑Oriented Programming

```python
class Person:
    species = "Homo sapiens"   # class attribute

    def __init__(self, name, age):
        self.name = name      # instance attr
        self.age = age

    def greet(self):
        return f"Hi, I'm {self.name}."

    @classmethod
    def from_dict(cls, data):
        return cls(data["name"], data["age"])

    @staticmethod
    def species_name():
        return "Homo sapiens"
```

---

## 7. File I/O

```python
# Text file (context manager auto‑closes)
with open("data.txt", "r") as f:
    text = f.read()

# Write
with open("out.txt", "w") as f:
    f.write(text)

# CSV with csv module
import csv
with open("table.csv") as f:
    reader = csv.DictReader(f)
    for row in reader: ...

# JSON
import json
data = json.load(open("config.json"))
json.dump(data, open("out.json", "w"), indent=2)
```

---

## 8. List/Dict Comprehensions

```python
squares = [x*x for x in range(10)]
evens   = {i: i*i for i in range(10) if i%2==0}
```

---

## 9. Functional Tools

| Tool                   | Purpose                           | Example                                                       |
| ---------------------- | --------------------------------- | ------------------------------------------------------------- |
| `map()`                | Apply function to iterable        | `list(map(str, [1,2,3]))`                                     |
| `filter()`             | Keep items that satisfy predicate | `list(filter(lambda x: x>5, nums))`                           |
| `reduce()` (functools) | Reduce to single value            | `from functools import reduce; reduce(lambda a,b: a+b, nums)` |
| `zip()`                | Parallel iteration                | `for k,v in zip(keys, values): ...`                           |

---

## 10. Regular Expressions

```python
import re
pattern = r"^\w+@\w+\.\w{2,3}$"

if re.match(pattern, email):
    print("Valid")

# Search and replace
text = re.sub(r"\bfoo\b", "bar", text)
```

---

## 11. Date & Time

```python
from datetime import datetime, timedelta

now   = datetime.now()
yesterday = now - timedelta(days=1)

# Formatting
print(now.strftime("%Y-%m-%d %H:%M"))

# Parsing
dt = datetime.strptime("2024-06-19", "%Y-%m-%d")
```

---

## 12. Error Handling

```python
try:
    risky_operation()
except ValueError as e:
    handle_error(e)
else:
    # runs if no exception
finally:
    cleanup()   # always runs
```

Custom exception:

```python
class MyError(Exception): pass
raise MyError("Something went wrong")
```

---

## 13. Miscellaneous Utilities

| Utility                       | Usage                                                      |
| ----------------------------- | ---------------------------------------------------------- |
| `enumerate()`                 | Get index with value:`for i, v in enumerate(lst):`         |
| `zip_longest()` (itertools)   | Pad shorter iterables:`from itertools import zip_longest;` |
| `defaultdict()` (collections) | Auto‑create dict values:`d = defaultdict(list)`            |
| `Counter()` (collections)     | Count hashable items:`c = Counter([1,2,2])`                |
| `namedtuple()` (collections)  | Lightweight struct:`Point = namedtuple('Point', 'x y')`    |
| `dataclass` (dataclasses)     | Auto‑generated init, repr, etc.                            |
| `pathlib.Path`                | Modern file paths:`p = Path("file.txt"); p.read_text()`    |

---

### Quick One‑liner Cheats

```python
# Max/min with key
max_item = max(collection, key=lambda x: x.value)

# Set operations
intersection = set_a & set_b
difference   = set_a - set_b

# Chain iterables
from itertools import chain
all_items = list(chain(list1, list2))

# Shuffle a list
import random; random.shuffle(lst)
```

---

**Tip:**
Keep this sheet handy while you code. Over time, the patterns will become second nature!
