# 🏎️ C Cheat Sheet – “The One‑Page Reference”

> **Quick‑look guide**
> for the most frequently used syntax and idioms in modern C (C11/C18).
> (Feel free to copy‑paste into a sticky note, PDF or your editor.)

---

## 1️⃣ Preprocessor

| #define | #include | #ifdef / #else / #endif |
| ------- | -------- | ----------------------- |

```c
#define PI   3.14159        // macro
#define MAX(a,b) ((a) > (b) ? (a) : (b))  // function‑like macro

#include <stdio.h>         // standard header
#include "myheader.h"      // local file

#ifdef DEBUG
    printf("debug\n");
#else
    printf("release\n");
#endif
```

---

## 2️⃣ Data Types & Qualifiers

| Type          | Size (Typical) | Range                  |
| ------------- | -------------- | ---------------------- |
| `char`        | 1 byte         | -128…127 / 0…255       |
| `short`       | 2 bytes        | -32 768…32 767         |
| `int`         | 4 bytes        | -2³¹…2³¹‑1             |
| `long`        | 8 bytes (C99)  | -9×10¹⁸…9×10¹⁸         |
| `float`       | 4 bytes        | ~±3.4e‑38 … ±3.4e+38   |
| `double`      | 8 bytes        | ~±1.7e‑308 … ±1.7e+308 |
| `long double` | ≥8 bytes       | platform‑specific      |

**Qualifiers**

```c
const   // read‑only
volatile // may change unexpectedly (hardware)
static  // internal linkage / block scope
extern   // external linkage
```

---

## 3️⃣ Variables & Constants

```c
int a = 5;          // mutable variable
const double PI = 3.14159; // compile‑time constant
volatile int sensor;       // hardware register
static int counter;        // file‑scope persistent value
extern int global_var;     // defined elsewhere
```

---

## 4️⃣ Control Flow

| Construct                 | Syntax |
| ------------------------- | ------ |
| `if` / `else if` / `else` | ```c   |
| if (cond) { ... }         |        |
| else if (cond2) { ... }   |        |
| else { ... }```           |        |
| `switch`                  | ```c   |
| switch(expr){             |        |
| case 1: ...               |        |
| default: ...              |        |
| }                         |        |

````|
| Loops | ```c
for(init; cond; incr){...}
while(cond){...}
do{...} while(cond);
````

|
| `break`, `continue`, `goto` | ```c
break; // exit loop or switch
continue; // next iteration
goto label; // jump (use sparingly)

````|

---

## 5️⃣  Functions

```c
// prototype
int add(int a, int b);

// definition
int add(int a, int b) {
    return a + b;
}

// variadic function
#include <stdarg.h>
void logf(const char *fmt, ...) {
    va_list ap; va_start(ap, fmt);
    vprintf(fmt, ap);  // prints using printf‑style format
    va_end(ap);
}
````

**Calling convention**

```c
int (*func_ptr)(int) = &add;
int x = func_ptr(3);   // same as add(3)
```

---

## 6️⃣ Pointers

| Concept             | Example                 |
| ------------------- | ----------------------- |
| Declaration         | `int *p;`               |
| Address of          | `p = &x;`               |
| Dereference         | `int y = *p;`           |
| Pointer to function | `int (*fp)(int) = add;` |
| Array pointer       | `char *s = "hello";`    |
| NULL pointer        | `ptr == NULL`           |

**Common pitfalls**

```c
int *p = malloc(sizeof(int));  // remember free(p);
if (p != NULL) { ... }

int arr[5];
int *q = arr;   // points to first element
```

---

## 7️⃣ Structs & Unions

```c
typedef struct {
    int id;
    char name[32];
} Person;

Person p = {.id=1, .name="Alice"};

union Data {
    int i;
    float f;
};

union Data d;
d.i = 42;   // d.f now contains same bits interpreted as float
```

**Bit‑fields**

```c
struct Flags {
    unsigned is_admin : 1;
    unsigned is_active : 1;
    unsigned reserved : 6;
};
```

---

## 8️⃣ Memory Management

| Function                 | Purpose                               |
| ------------------------ | ------------------------------------- |
| `malloc(size)`           | Allocate `size` bytes (uninitialized) |
| `calloc(n, size)`        | Allocate and zero‑initialize          |
| `realloc(ptr, new_size)` | Resize existing block                 |
| `free(ptr)`              | Release memory                        |

```c
int *arr = malloc(10 * sizeof(int));
if (!arr) { perror("malloc"); exit(EXIT_FAILURE); }
// use arr
free(arr);
```

---

## 9️⃣ I/O Basics

### Standard Streams

```c
printf("Hello %s\n", "world");
scanf("%d", &num);
```

### File Operations

```c
FILE *fp = fopen("data.txt","r"); // mode: r,w,rb,wb,a+, etc.
if (!fp) { perror("fopen"); exit(EXIT_FAILURE); }

char buf[256];
while (fgets(buf, sizeof buf, fp)) {
    printf("%s", buf);
}
fclose(fp);
```

---

## 🔟 String Handling

```c
#include <string.h>

char src[] = "abc";
char dst[10];

strcpy(dst, src);          // copy
strncpy(dst, src, 4);      // copy up to n chars (no null if truncated)

size_t len = strlen(src);

int cmp = strcmp("foo", "bar"); // <0, 0, >0

// tokenization
char *tok = strtok(str, ",");
while(tok){
    puts(tok);
    tok = strtok(NULL, ",");
}
```

---

## 🏁 Common Standard Library Headers

| Header        | Typical use                             |
| ------------- | --------------------------------------- |
| `<stdio.h>`   | I/O (`printf`, `scanf`, file ops)       |
| `<stdlib.h>`  | Memory (`malloc`), conversions (`atoi`) |
| `<string.h>`  | String & memory functions               |
| `<stdbool.h>` | `bool`, `true`, `false`                 |
| `<stddef.h>`  | `size_t`, `ptrdiff_t`, `NULL`           |
| `<stdint.h>`  | Fixed‑width integers (`int32_t`)        |
| `<limits.h>`  | Integer limits (`INT_MAX`)              |
| `<ctype.h>`   | Character classification (`isdigit`)    |
| `<math.h>`    | Math functions (`sin`, `pow`)           |
| `<assert.h>`  | Debug assertions                        |

---

## 📚 Quick Reference – Common Macros

```c
#define ARRAY_SIZE(arr)   (sizeof(arr)/sizeof((arr)[0]))

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
```

---

## ⚠️ Safety Tips

- **Always check return values** (`malloc`, `fopen`, `scanf`).
- Use **`const`** for read‑only data.
- Prefer **`size_t`** for sizes and indexes.
- Avoid magic numbers; define meaningful constants.
- When using pointers, **initialize** them to `NULL`.
- Remember **`free()`** for every successful allocation.

---

## 🎯 One‑liner Cheat Sheet

```c
// Declarations
int a = 5;
const char *msg = "Hello";

// Functions
int sum(int x,int y){return x+y;}

// Pointers
int *p=&a;

// Control flow
if(a>0) puts(msg);
else printf("%d\n",a);

// Loops
for(int i=0;i<10;i++) printf("%d ",i);

// IO
printf("Sum: %d\n", sum(3,4));
```

---

### Final Thought

Keep this sheet handy while you write C code.
As you grow more comfortable, you’ll internalize most of these patterns – and your own personal “cheat sheet” will evolve! 🚀
