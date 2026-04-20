## 🎓 C# Cheat Sheet

_A quick‑reference guide for the most common concepts, syntax, and patterns in C# 12 (latest stable release)._

---

### Table of Contents

1. **Setup & Project Structure**
2. **Syntax Basics**
3. **Data Types & Variables**
4. **Control Flow**
5. **Functions & Delegates**
6. **Object‑Oriented Programming**
7. **Collections & LINQ**
8. **Exception Handling**
9. **Asynchronous Programming**
10. **File I/O & Serialization**
11. **Common Patterns & Best Practices**
12. **Useful Extensions & APIs**

---

## 1️⃣ Setup & Project Structure

| Item                   | Command / Example                  |
| ---------------------- | ---------------------------------- |
| Create new console app | `dotnet new console -n MyApp`      |
| Build                  | `dotnet build`                     |
| Run                    | `dotnet run`                       |
| Add package            | `dotnet add package <PackageName>` |
| Global using (C# 10+)  | `global using System;`             |

---

## 2️⃣ Syntax Basics

```csharp
// Comments
// Single line: // …
/* Multi‑line:
   ... */

// Namespaces
namespace MyApp { … }

// Entry point
static void Main(string[] args) => Console.WriteLine("Hello");

// Using alias
using Project = MyCompany.Project;

// Top‑level statements (C# 9+)
Console.WriteLine("Top‑level");
```

---

## 3️⃣ Data Types & Variables

| Type      | Size      | Default |
| --------- | --------- | ------- |
| `bool`    | 1 byte    | `false` |
| `byte`    | 1 byte    | `0`     |
| `sbyte`   | 1 byte    | `0`     |
| `short`   | 2 bytes   | `0`     |
| `ushort`  | 2 bytes   | `0`     |
| `int`     | 4 bytes   | `0`     |
| `uint`    | 4 bytes   | `0`     |
| `long`    | 8 bytes   | `0L`    |
| `ulong`   | 8 bytes   | `0UL`   |
| `float`   | 4 bytes   | `0F`    |
| `double`  | 8 bytes   | `0D`    |
| `decimal` | 16 bytes  | `0M`    |
| `char`    | 2 bytes   | `'\0'`  |
| `string`  | reference | `null`  |
| `object`  | reference | `null`  |

```csharp
int x = 5;
var y = "hello";          // implicit typing
const double PI = 3.14159; // compile‑time constant
```

---

## 4️⃣ Control Flow

### Conditional Statements

```csharp
if (x > 0) { … }
else if (x < 0) { … }
else { … }

switch (day)
{
    case Day.Monday: …
    default: …
}
```

### Loops

```csharp
for (int i = 0; i < 10; i++) { … }
foreach (var item in collection) { … }
while (condition) { … }
do { … } while (condition);
```

### Break / Continue / Return

```csharp
break;   // exit loop or switch
continue; // skip to next iteration
return;  // return from method
```

---

## 5️⃣ Functions & Delegates

| Concept            | Syntax                                                                                 |
| ------------------ | -------------------------------------------------------------------------------------- |
| Method             | `public int Add(int a, int b) => a + b;`                                               |
| Parameter defaults | `void Log(string msg = "No message") { … }`                                            |
| Params array       | `void Print(params string[] items) { … }`                                              |
| Out parameters     | `bool TryParse(string s, out int result)`                                              |
| Delegate           | `public delegate void Notify(string message);`                                         |
| Anonymous lambda   | `Notify n = msg => Console.WriteLine(msg);`                                            |
| Func / Action      | `Func<int,int> add = (a,b)=>a+b;<br>``Action<string> print = s=>Console.WriteLine(s);` |

```csharp
public static void Main()
{
    int sum = Add(3, 4);
}
```

---

## 6️⃣ Object‑Oriented Programming

### Class & Inheritance

```csharp
class Animal { public virtual void Speak() => Console.WriteLine("..."); }
class Dog : Animal { public override void Speak() => Console.WriteLine("Bark!"); }
```

### Properties (auto, computed, init‑only)

```csharp
public string Name { get; set; }          // auto
public int Age { get; private set; }      // private setter
public int Id { get; init; }              // init‑only (C# 9+)

public double Volume => Length * Width * Height; // computed
```

### Indexers & Default Interface Methods

```csharp
class Bag<T> : IEnumerable<T>
{
    public T this[int index] { … }
}
```

### Access Modifiers

`public`, `private`, `protected`, `internal`, `protected internal`, `private protected`.

---

## 6️⃣ Collections & LINQ

| Collection                | Notes         |
| ------------------------- | ------------- |
| `List<T>`                 | dynamic array |
| `Dictionary<TKey,TValue>` | hash map      |
| `HashSet<T>`              | unique set    |
| `Queue<T> / Stack<T>`     | FIFO/LIFO     |

### LINQ (Language Integrated Query)

```csharp
var numbers = Enumerable.Range(1, 10);

var evens = numbers.Where(n => n % 2 == 0);
var squares = numbers.Select(n => n * n);
var firstFive = numbers.Take(5);
var sumAll = numbers.Sum();

var grouped =
    numbers.GroupBy(n => n % 3)
           .Select(g => new { Mod = g.Key, Count = g.Count() });

var dict = numbers.ToDictionary(x => x, x => x * 2);

var joined = people.Join(
    orders,
    p => p.Id,
    o => o.CustomerId,
    (p,o)=>new{p.Name, o.Total});

```

---

## 6️⃣ Exception Handling

```csharp
try
{
    // risky code
}
catch (ArgumentException ex) { … }
catch (Exception ex) when (ex.Message.Contains("timeout")) { … }   // filter
finally { … }   // always runs

// Custom exception
public class ValidationException : Exception
{
    public ValidationException(string msg) : base(msg) { }
}
```

---

## 7️⃣ Asynchronous Programming

| Feature               | Syntax                                    |
| --------------------- | ----------------------------------------- |
| `async` method        | `async Task<int> ComputeAsync() { … }`    |
| Await                 | `var result = await ComputeAsync();`      |
| CancellationToken     | `await Task.Delay(1000, token);`          |
| ConfigureAwait(false) | `await SomeCall().ConfigureAwait(false);` |

### Async Main (C# 7.1+)

```csharp
static async Task Main(string[] args)
{
    await DoWorkAsync();
}
```

---

## 8️⃣ File I/O & Serialization

| Operation               | API                                                        |
| ----------------------- | ---------------------------------------------------------- |
| Read all text           | `string txt = File.ReadAllText(path);`                     |
| Write all text          | `File.WriteAllText(path, txt);`                            |
| Binary read/write       | `BinaryReader br = new(File.OpenRead(path));`              |
| XML serialization       | `var xml = XmlSerializer(typeof(MyClass)).Serialize(obj);` |
| JSON (System.Text.Json) |                                                            |

```csharp
string json = JsonSerializer.Serialize(obj);
var obj2 = JsonSerializer.Deserialize<MyClass>(json);
```

| JSON (Newtonsoft) | `JsonConvert.SerializeObject(obj)` / `DeserializeObject<T>()` |

---

## 9️⃣ Common Patterns & Best Practices

| Pattern       | Usage                             |
| ------------- | --------------------------------- |
| **Singleton** | Lazy initialization, thread‑safe: |

```csharp
public sealed class Logger
{
    private static readonly Lazy<Logger> _lazy = new(() => new Logger());
    public static Logger Instance => _lazy.Value;
}
```

|
| **Factory** | Create objects without exposing concrete types. |
| **Repository** | Abstract data access, useful with EF Core. |
| **Dependency Injection** | Use `Microsoft.Extensions.DependencyInjection`. Register services:
`services.AddSingleton<IMyService, MyService>();` |
| **Using statements** | Ensure disposal: `using var stream = File.OpenRead(path);` |
| **Record types** (C# 9+) | Immutable data with value semantics:

```csharp
public record Person(string FirstName, string LastName);
```

|

---

## 🔧 Useful Extensions & APIs

| Feature                       | Example                  |
| ----------------------------- | ------------------------ |
| `Span<T>` / `ReadOnlySpan<T>` | Zero‑copy memory slices. |
| `StringBuilder`               | Efficient concatenation: |

```csharp
var sb = new StringBuilder();
sb.Append("Hello");
```

|
| `Path`, `Directory`, `FileInfo` | Path manipulation, file info. |
| `DateTimeOffset` | Time‑zone aware timestamps. |
| `Random.Shared` | Thread‑safe random instance (C# 6+). |

---

### Quick Reference Snippets

```csharp
// Ternary
var status = isActive ? "Active" : "Inactive";

// Null‑conditional
string length = person?.Name?.Length.ToString() ?? "unknown";

// Null-coalescing assignment
person ??= new Person();

// Range & Index (C# 8+)
var lastThree = arr[^3..];   // from third‑last to end

// Pattern matching in switch
switch (obj)
{
    case string s when s.Length > 5: …
}
```

---

### 📚 Further Reading

- **.NET Docs** – [docs.microsoft.com/dotnet](https://docs.microsoft.com/en-us/dotnet/)
- **C# Language Spec** – official spec for deep dives.
- **Effective C# (Bill Wagner)** – best‑practice guide.

Happy coding! 🚀
