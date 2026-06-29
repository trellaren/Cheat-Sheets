// ============================================================
// C Language Examples
// ============================================================
// Compile:  gcc -std=c11 -Wall -Wextra -pedantic -o main main.c -lm
// Run:      ./main
//
// This file is a single-file, self-contained walkthrough of the
// major components of the C programming language.
// Each section header marks a topic; read top-to-bottom or jump
// to the section you need.
// ============================================================

// ============================================================
// SECTION 1: PREPROCESSOR DIRECTIVES
// ============================================================
// The preprocessor runs before the compiler.  It performs textual
// substitution, conditional compilation, and file inclusion.

// Standard-library headers use angle brackets.
#include <stdio.h>    // printf, scanf, fopen, fclose, fgets, FILE …
#include <stdlib.h>   // malloc, calloc, realloc, free, exit, atoi …
#include <string.h>   // strcpy, strncpy, strcmp, strlen, memset, memcpy …
#include <stdint.h>   // uint8_t, int32_t, uint64_t … fixed-width integers
#include <stdbool.h>  // bool, true, false  (C99)
#include <stddef.h>   // size_t, ptrdiff_t, NULL, offsetof
#include <limits.h>   // INT_MAX, INT_MIN, UINT_MAX …
#include <ctype.h>    // isdigit, isalpha, toupper, tolower …
#include <errno.h>    // errno, EINVAL, ENOMEM …
#include <assert.h>   // assert() — disabled in release (-DNDEBUG)
#include <math.h>     // sqrt, pow, sin, cos …  (link with -lm)
#include <time.h>     // time, clock, struct tm …

// Object-like macro: a constant with no type – the preprocessor
// replaces every use of PI with the literal before compilation.
#define PI 3.14159265358979

// Function-like macro: parenthesise every argument to avoid operator
// precedence surprises.
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define ARRAY_LEN(arr) (sizeof(arr) / sizeof((arr)[0]))

// Conditional compilation – define DEBUG on the command line with -DDEBUG.
#ifdef DEBUG
#  define LOG(fmt, ...) fprintf(stderr, "[DEBUG] " fmt "\n", ##__VA_ARGS__)
#else
#  define LOG(fmt, ...) /* nothing in release builds */
#endif

// ============================================================
// SECTION 2: DATA TYPES
// ============================================================
// C provides several fundamental types.  Their exact size can vary by
// platform; use <stdint.h> fixed-width types for portable code.

void data_types_examples(void) {
    printf("\n== Section 2: Data Types ==\n");

    // ---- Integer types ----
    char    c    = 'A';         // 1 byte; may be signed or unsigned
    short   s    = -32000;      // at least 16 bits
    int     i    = 2147483647;  // at least 16 bits, usually 32
    long    l    = 123456789L;  // at least 32 bits
    long long ll = 9000000000LL;// at least 64 bits

    // unsigned variants (no negative values; doubles the positive range)
    unsigned int  ui = 4294967295U;
    unsigned char uc = 255;

    // Fixed-width types from <stdint.h> – use these for portable code.
    int8_t   i8  = -128;
    uint8_t  u8  = 255;
    int16_t  i16 = -32768;
    uint16_t u16 = 65535;
    int32_t  i32 = -2147483648;
    uint32_t u32 = 4294967295U;
    int64_t  i64 = -9223372036854775807LL - 1;
    uint64_t u64 = 18446744073709551615ULL;

    // ---- Floating-point types ----
    float  f  = 3.14f;       // ~7 significant decimal digits
    double d  = 3.14159265;  // ~15 significant decimal digits
    long double ld = 3.141592653589793238L;

    // ---- Boolean (C99) ----
    bool flag = true;        // 0 or 1; from <stdbool.h>

    // ---- size_t: the result type of sizeof, used for sizes/counts ----
    size_t len = sizeof(int);

    printf("char='%c' short=%hd int=%d long=%ld llong=%lld\n",
           c, s, i, l, ll);
    printf("uint=%u uchar=%u\n", ui, uc);
    printf("int8=%d uint8=%u int16=%hd uint16=%hu\n", i8, u8, i16, u16);
    printf("int32=%d uint32=%u int64=%lld uint64=%llu\n",
           i32, u32, (long long)i64, (unsigned long long)u64);
    printf("float=%.2f double=%.8f ldouble=%.18Lf\n", f, d, ld);
    printf("bool=%s size_t(sizeof int)=%zu\n", flag ? "true" : "false", len);

    // ---- Type conversions ----
    // Implicit widening conversion (safe): int → double
    double promoted = i;
    // Explicit cast (may truncate): double → int
    int truncated = (int)d;
    printf("promoted=%f truncated=%d\n", promoted, truncated);
}

// ============================================================
// SECTION 3: VARIABLES, CONSTANTS & QUALIFIERS
// ============================================================
// Storage-class and type qualifiers change how a variable is stored
// or whether it can be modified.

// extern declares a variable that is *defined* in another translation unit.
extern int global_counter;   // definition lives elsewhere; just a declaration here.

// static at file scope: internal linkage – invisible outside this .c file.
static int file_private_var = 0;

// const: the value cannot be changed after initialisation.
static const double GRAVITY = 9.80665;

void variables_examples(void) {
    printf("\n== Section 3: Variables, Constants & Qualifiers ==\n");

    // auto (default for locals): automatic storage – lives on the stack.
    int x = 10;
    const int LIMIT = 100;  // local constant

    // static local: initialised once, retains value between calls.
    static int call_count = 0;
    call_count++;

    // volatile: tells the compiler not to optimise accesses away.
    // Used for memory-mapped hardware registers and signal-modified variables.
    volatile int sensor_reading = 42;

    // register: hint to the compiler to keep in a CPU register (ignored today).
    register int fast = 5;

    printf("x=%d LIMIT=%d call_count=%d sensor=%d fast=%d\n",
           x, LIMIT, call_count, sensor_reading, fast);
    printf("GRAVITY=%.5f file_private=%d\n", GRAVITY, file_private_var);

    // sizeof gives the byte size of a type or variable.
    printf("sizeof(int)=%zu  sizeof(double)=%zu  sizeof(char)=%zu\n",
           sizeof(int), sizeof(double), sizeof(char));
}

// Definition of the external variable declared above.
int global_counter = 0;

// ============================================================
// SECTION 4: CONTROL FLOW
// ============================================================

void control_flow_examples(void) {
    printf("\n== Section 4: Control Flow ==\n");

    // ---- if / else if / else ----
    int score = 75;
    if (score >= 90) {
        printf("Grade: A\n");
    } else if (score >= 75) {
        printf("Grade: B\n");
    } else {
        printf("Grade: C or below\n");
    }

    // ---- Ternary operator ----
    const char *result = (score >= 60) ? "pass" : "fail";
    printf("Result: %s\n", result);

    // ---- switch / case ----
    // Each case falls through to the next unless there is a break.
    int day = 3;
    switch (day) {
        case 1:  printf("Monday\n");    break;
        case 2:  printf("Tuesday\n");   break;
        case 3:  printf("Wednesday\n"); break;
        case 4:  printf("Thursday\n");  break;
        case 5:  printf("Friday\n");    break;
        default: printf("Weekend\n");   break;
    }

    // ---- for loop ----
    printf("squares:");
    for (int i = 1; i <= 5; i++) {
        printf(" %d", i * i);
    }
    printf("\n");

    // ---- while loop ----
    int n = 1;
    while (n < 32) {
        n *= 2;
    }
    printf("first power of 2 >= 32: %d\n", n);

    // ---- do-while loop (executes body at least once) ----
    int attempts = 0;
    do {
        attempts++;
    } while (attempts < 3);
    printf("attempts: %d\n", attempts);

    // ---- break / continue ----
    printf("odd numbers up to 10:");
    for (int i = 1; i <= 10; i++) {
        if (i % 2 == 0) continue;   // skip even
        if (i > 9)      break;      // stop early
        printf(" %d", i);
    }
    printf("\n");

    // ---- goto (avoid in general; useful for cleanup in C) ----
    int error_code = 0;
    if (error_code != 0) goto cleanup;
    printf("no error to handle\n");
cleanup:
    ; // semicolon needed before end of block when label is last statement
}

// ============================================================
// SECTION 5: FUNCTIONS
// ============================================================
// Functions must be declared (prototype) before they are called.
// A definition includes the body.

// Forward declaration (prototype).
int add(int a, int b);
int factorial(int n);

// Function definition.
int add(int a, int b) {
    return a + b;
}

// Recursive function.
int factorial(int n) {
    if (n <= 1) return 1;
    return n * factorial(n - 1);
}

// Pass-by-pointer to allow the function to modify the caller's variable.
void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

// Returns multiple values via output parameters.
void min_max(const int *arr, size_t len, int *out_min, int *out_max) {
    assert(arr != NULL && len > 0);
    *out_min = *out_max = arr[0];
    for (size_t i = 1; i < len; i++) {
        if (arr[i] < *out_min) *out_min = arr[i];
        if (arr[i] > *out_max) *out_max = arr[i];
    }
}

// Variadic function: accepts a variable number of arguments.
#include <stdarg.h>
int sum_variadic(int count, ...) {
    va_list args;
    va_start(args, count);
    int total = 0;
    for (int i = 0; i < count; i++) {
        total += va_arg(args, int);
    }
    va_end(args);
    return total;
}

// Function pointer type alias for readability.
typedef int (*BinaryOpFn)(int, int);

// Higher-order function: accepts a function pointer as a parameter.
int apply(BinaryOpFn op, int x, int y) {
    return op(x, y);
}

static int multiply(int a, int b) { return a * b; }

void functions_examples(void) {
    printf("\n== Section 5: Functions ==\n");

    printf("add(3, 4)=%d\n", add(3, 4));
    printf("factorial(6)=%d\n", factorial(6));

    int a = 10, b = 20;
    swap(&a, &b);
    printf("after swap: a=%d b=%d\n", a, b);

    int nums[] = {5, 2, 8, 1, 9, 3};
    int lo, hi;
    min_max(nums, ARRAY_LEN(nums), &lo, &hi);
    printf("min=%d max=%d\n", lo, hi);

    printf("sum_variadic(4, 1,2,3,4)=%d\n", sum_variadic(4, 1, 2, 3, 4));

    // Use a function pointer.
    BinaryOpFn op = multiply;
    printf("apply(multiply, 6, 7)=%d\n", apply(op, 6, 7));
}

// ============================================================
// SECTION 6: ARRAYS & STRINGS
// ============================================================

void arrays_strings_examples(void) {
    printf("\n== Section 6: Arrays & Strings ==\n");

    // ---- 1-D array ----
    int scores[5] = {90, 85, 72, 88, 95};
    int total = 0;
    for (size_t i = 0; i < ARRAY_LEN(scores); i++) {
        total += scores[i];
    }
    printf("average score: %.1f\n", (double)total / ARRAY_LEN(scores));

    // ---- 2-D array ----
    int matrix[2][3] = {{1, 2, 3}, {4, 5, 6}};
    printf("matrix[1][2]=%d\n", matrix[1][2]);

    // ---- C strings ----
    // A C string is a null-terminated array of char.
    char greeting[32] = "Hello";          // mutable, stack-allocated
    const char *language = "C";           // string literal (read-only)

    // strcat appends src to dst; ensure dst is large enough.
    strncat(greeting, ", ", sizeof(greeting) - strlen(greeting) - 1);
    strncat(greeting, language, sizeof(greeting) - strlen(greeting) - 1);
    strncat(greeting, "!", sizeof(greeting) - strlen(greeting) - 1);

    printf("greeting='%s' length=%zu\n", greeting, strlen(greeting));
    printf("strcmp(\"abc\",\"abc\")=%d\n", strcmp("abc", "abc"));
    printf("strcmp(\"abc\",\"abd\")=%d\n", strcmp("abc", "abd"));

    // Character classification and conversion.
    char sample[] = "Hello World";
    for (size_t i = 0; sample[i]; i++) {
        sample[i] = (char)toupper((unsigned char)sample[i]);
    }
    printf("uppercase: %s\n", sample);

    // Formatted into a buffer with snprintf (safe – respects buffer size).
    char buf[64];
    snprintf(buf, sizeof(buf), "Score: %d / %zu", 88, ARRAY_LEN(scores));
    printf("%s\n", buf);
}

// ============================================================
// SECTION 7: POINTERS
// ============================================================
// A pointer holds the memory address of another variable.

void pointers_examples(void) {
    printf("\n== Section 7: Pointers ==\n");

    int value = 42;
    int *ptr = &value;      // ptr holds the address of value
    *ptr = 100;             // dereference: modify value through the pointer
    printf("value=%d *ptr=%d\n", value, *ptr);

    // Pointer arithmetic: moves by sizeof(*ptr) bytes per step.
    int arr[] = {10, 20, 30, 40, 50};
    int *p = arr;           // array name decays to pointer to first element
    printf("arr[2] via pointer: %d\n", *(p + 2));

    // Incrementing a pointer steps to the next element.
    for (int *q = arr; q < arr + ARRAY_LEN(arr); q++) {
        printf("%d ", *q);
    }
    printf("\n");

    // Pointer to pointer (double pointer): used for dynamic 2-D arrays, etc.
    int x = 7;
    int *ptrx = &x;
    int **ptrptr = &ptrx;
    printf("**ptrptr=%d\n", **ptrptr);

    // const pointer semantics:
    const int *cptr = &value;     // can't modify value through cptr
    int * const cptr2 = &value;   // can't change where cptr2 points
    (void)cptr; (void)cptr2;      // suppress unused-variable warnings

    // NULL pointer: represents "no object".
    int *null_ptr = NULL;
    if (null_ptr == NULL) {
        printf("null_ptr is NULL – safe to check before dereferencing\n");
    }

    // void pointer: a generic pointer, must be cast before dereferencing.
    void *generic = ptr;
    int *typed = (int *)generic;
    printf("via void*: %d\n", *typed);
}

// ============================================================
// SECTION 8: STRUCTS, UNIONS & ENUMS
// ============================================================

// Struct: a named collection of fields of potentially different types.
typedef struct {
    char  name[64];
    int   age;
    float gpa;
} Student;

// Struct with a pointer for linked-list style.
typedef struct Node {
    int          value;
    struct Node *next;  // self-referential – can't use the typedef name here
} Node;

// Enum: a set of named integer constants.
typedef enum {
    COLOUR_RED   = 0,
    COLOUR_GREEN = 1,
    COLOUR_BLUE  = 2
} Colour;

// Union: all members share the same memory location.
// The size of the union equals the size of the largest member.
typedef union {
    uint32_t raw;
    struct {
        uint8_t b;    // byte 0
        uint8_t g;    // byte 1
        uint8_t r;    // byte 2
        uint8_t a;    // byte 3
    } channels;
} Pixel;

void structs_examples(void) {
    printf("\n== Section 8: Structs, Unions & Enums ==\n");

    // Initialise with a compound literal (designated initialisers – C99).
    Student s1 = {.name = "Alice", .age = 20, .gpa = 3.8f};
    printf("student: %s age=%d gpa=%.1f\n", s1.name, s1.age, s1.gpa);

    // Accessing struct members via pointer uses the arrow (->) operator.
    Student *sp = &s1;
    sp->age = 21;
    printf("updated age: %d\n", sp->age);

    // Enum usage.
    Colour c = COLOUR_GREEN;
    printf("colour value: %d\n", (int)c);

    // Union: writing raw sets all channels at once.
    Pixel px;
    px.raw = 0xFF8040C0U;  // BGRA packed 32-bit colour
    printf("pixel raw=0x%08X r=%u g=%u b=%u a=%u\n",
           px.raw, px.channels.r, px.channels.g,
           px.channels.b, px.channels.a);

    // offsetof: byte offset of a field within a struct.
    printf("offsetof(Student, gpa)=%zu\n", offsetof(Student, gpa));
}

// ============================================================
// SECTION 9: MEMORY MANAGEMENT
// ============================================================
// C manages memory manually.  Every malloc/calloc must be paired with free.

void memory_management_examples(void) {
    printf("\n== Section 9: Memory Management ==\n");

    // malloc: allocate raw bytes (uninitialised).
    int *arr = malloc(5 * sizeof(int));
    if (arr == NULL) {
        perror("malloc");
        return;
    }
    for (int i = 0; i < 5; i++) arr[i] = i * i;

    // calloc: allocate and zero-initialise.
    int *zeroed = calloc(5, sizeof(int));
    if (zeroed == NULL) {
        perror("calloc");
        free(arr);
        return;
    }

    // realloc: resize an existing allocation (may move the pointer).
    int *bigger = realloc(arr, 10 * sizeof(int));
    if (bigger == NULL) {
        perror("realloc");
        free(zeroed);
        free(arr);
        return;
    }
    arr = bigger;  // arr is now invalid; use bigger (saved back to arr)
    for (int i = 5; i < 10; i++) arr[i] = i * i;

    printf("dynamic array:");
    for (int i = 0; i < 10; i++) printf(" %d", arr[i]);
    printf("\n");
    printf("zeroed[0]=%d\n", zeroed[0]);

    free(arr);
    free(zeroed);
    // Good practice: set freed pointers to NULL to avoid use-after-free.
    arr    = NULL;
    zeroed = NULL;

    // Dynamic struct allocation.
    Student *student = malloc(sizeof(Student));
    if (student == NULL) { perror("malloc"); return; }
    snprintf(student->name, sizeof(student->name), "Bob");
    student->age = 22;
    student->gpa = 3.5f;
    printf("heap student: %s age=%d\n", student->name, student->age);
    free(student);
}

// ============================================================
// SECTION 10: FILE I/O
// ============================================================

void file_io_examples(void) {
    printf("\n== Section 10: File I/O ==\n");

    // fopen modes: "r" read, "w" write (truncate), "a" append,
    //              "rb"/"wb" binary, "r+" read+write
    const char *path = "/tmp/c_example_demo.txt";

    // ---- Write ----
    FILE *fp = fopen(path, "w");
    if (fp == NULL) { perror("fopen write"); return; }
    fprintf(fp, "Line 1: C file I/O\n");
    fprintf(fp, "Line 2: fgets reads one line at a time\n");
    fprintf(fp, "Line 3: fclose flushes and releases the handle\n");
    fclose(fp);

    // ---- Read ----
    fp = fopen(path, "r");
    if (fp == NULL) { perror("fopen read"); return; }

    char line[128];
    int  line_num = 0;
    while (fgets(line, sizeof(line), fp) != NULL) {
        line_num++;
        // fgets preserves the trailing '\n'; strip it for clean output.
        line[strcspn(line, "\n")] = '\0';
        printf("  [%d] %s\n", line_num, line);
    }
    fclose(fp);

    // ---- Binary I/O ----
    const char *bin_path = "/tmp/c_example_demo.bin";
    int data[4] = {0xDEAD, 0xBEEF, 0xCAFE, 0xBABE};
    fp = fopen(bin_path, "wb");
    if (fp == NULL) { perror("fopen binary write"); return; }
    fwrite(data, sizeof(int), ARRAY_LEN(data), fp);
    fclose(fp);

    int read_back[4] = {0};
    fp = fopen(bin_path, "rb");
    if (fp == NULL) { perror("fopen binary read"); return; }
    fread(read_back, sizeof(int), ARRAY_LEN(read_back), fp);
    fclose(fp);
    printf("binary round-trip: 0x%X 0x%X\n", read_back[0], read_back[1]);

    // Clean up temp files.
    remove(path);
    remove(bin_path);
}

// ============================================================
// SECTION 11: ERROR HANDLING
// ============================================================
// C does not have exceptions.  Errors are communicated via:
//   • return codes (negative values or NULL)
//   • the global errno variable (set by system calls / library functions)
//   • perror() / strerror() to turn errno into a human-readable message

void error_handling_examples(void) {
    printf("\n== Section 11: Error Handling ==\n");

    // errno is set by many library functions on failure.
    errno = 0;
    FILE *fp = fopen("/tmp/nonexistent_path_xyz/file.txt", "r");
    if (fp == NULL) {
        // perror prefixes the message with the given string.
        perror("fopen demo");
        // strerror converts errno to a string directly.
        printf("strerror(%d): %s\n", errno, strerror(errno));
    }

    // assert aborts the program if the condition is false.
    // Compiled out when NDEBUG is defined (release builds).
    int value = 42;
    assert(value == 42);  // passes – no output

    // Propagating errors with return codes.
    char *end;
    errno = 0;
    long parsed = strtol("12345", &end, 10);
    if (errno != 0) {
        perror("strtol");
    } else if (*end != '\0') {
        fprintf(stderr, "non-numeric characters remain: '%s'\n", end);
    } else {
        printf("strtol parsed: %ld\n", parsed);
    }
}

// ============================================================
// SECTION 12: STANDARD LIBRARY HIGHLIGHTS
// ============================================================

void stdlib_examples(void) {
    printf("\n== Section 12: Standard Library Highlights ==\n");

    // ---- <math.h> ----
    printf("sqrt(2)=%.6f  pow(2,10)=%.0f  sin(PI/2)=%.6f\n",
           sqrt(2.0), pow(2.0, 10.0), sin(PI / 2.0));

    // ---- <stdlib.h> conversions ----
    int    i  = atoi("42");
    double d  = atof("3.14");
    printf("atoi=\"%d\"  atof=\"%.2f\"\n", i, (float)d);

    // ---- <stdlib.h> qsort ----
    // qsort needs a comparator: returns <0, 0, or >0.
    int numbers[] = {5, 2, 8, 1, 9, 3};
    // Use a compatible function pointer directly.
    int cmp(const void *a, const void *b);
    qsort(numbers, ARRAY_LEN(numbers), sizeof(int), cmp);
    printf("sorted:");
    for (size_t i2 = 0; i2 < ARRAY_LEN(numbers); i2++) {
        printf(" %d", numbers[i2]);
    }
    printf("\n");

    // ---- <stdlib.h> bsearch ----
    int target = 5;
    int *found = bsearch(&target, numbers, ARRAY_LEN(numbers),
                         sizeof(int), cmp);
    printf("bsearch(5): %s\n", found ? "found" : "not found");

    // ---- <time.h> ----
    time_t now = time(NULL);
    struct tm *gmt = gmtime(&now);
    char ts[32];
    strftime(ts, sizeof(ts), "%Y-%m-%dT%H:%M:%SZ", gmt);
    printf("current UTC: %s\n", ts);

    // ---- <string.h> memory functions ----
    char src[] = "Hello, C!";
    char dst[32];
    memcpy(dst, src, sizeof(src));  // copy sizeof(src) bytes
    printf("memcpy: %s\n", dst);
    memset(dst, 0, sizeof(dst));    // zero out the buffer
    printf("memset: all zeros, dst[0]=%d\n", dst[0]);
}

// Comparator for qsort / bsearch (must have this signature).
int cmp(const void *a, const void *b) {
    int ia = *(const int *)a;
    int ib = *(const int *)b;
    return (ia > ib) - (ia < ib);  // branchless; avoids subtraction overflow
}

// ============================================================
// SECTION 13: LINKED LIST (putting it all together)
// ============================================================
// A singly-linked list demonstrates: structs, pointers, dynamic memory,
// and the common cleanup pattern.

// Prepend a new node.  Returns the new head.
Node *list_prepend(Node *head, int value) {
    Node *node = malloc(sizeof(Node));
    if (node == NULL) { perror("malloc"); return head; }
    node->value = value;
    node->next  = head;
    return node;
}

// Free all nodes in the list.
Node *list_free(Node *head) {
    while (head != NULL) {
        Node *next = head->next;
        free(head);
        head = next;
    }
    return NULL;
}

void linked_list_examples(void) {
    printf("\n== Section 13: Linked List ==\n");

    Node *list = NULL;
    for (int i = 1; i <= 5; i++) {
        list = list_prepend(list, i * 10);
    }

    printf("list (reversed):");
    for (Node *n = list; n != NULL; n = n->next) {
        printf(" %d", n->value);
    }
    printf("\n");

    list = list_free(list);
    printf("list freed: %s\n", list == NULL ? "NULL" : "not NULL");
}

// ============================================================
// MAIN
// ============================================================

int main(void) {
    printf("=== C Language Walkthrough ===\n");

    data_types_examples();
    variables_examples();
    control_flow_examples();
    functions_examples();
    arrays_strings_examples();
    pointers_examples();
    structs_examples();
    memory_management_examples();
    file_io_examples();
    error_handling_examples();
    stdlib_examples();
    linked_list_examples();

    printf("\n=== All sections complete ===\n");
    return 0;
}
