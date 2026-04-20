# ⚡️ Unreal C++ Cheat Sheet

_(Unreal Engine 5.x – the most recent long‑term release as of 2026)_

> **Quick note:** Unreal’s C++ is a _sub‑set_ of modern C++. It introduces its own macros, reflection system, and coding conventions that are tightly coupled with the engine. This sheet covers the essentials you’ll need when writing gameplay code, editor extensions, or low‑level systems.

---

## 1️⃣ Language & Build Settings

| Setting               | What it means                              |
| --------------------- | ------------------------------------------ |
| **C++ Standard**      | `-std=c++20` (UE5 uses C++20)              |
| **Platform SDK**      | `-platform=Win64`, `-platform=Linux`, etc. |
| **Compiler warnings** | `-Werror -Wall -Wextra -pedantic`          |

```bash
# Example compile command (for a single module)
c++ -std=c++20 -Wall -Wextra -pedantic \
    $(UnrealBuildTool -projectfiles) \
    MyModule.cpp -o MyModule.dll
```

---

## 2️⃣ Unreal Macros & Boilerplate

| Macro                                               | Purpose                                                                            | Typical usage                                                                   |
| --------------------------------------------------- | ---------------------------------------------------------------------------------- | ------------------------------------------------------------------------------- |
| `UCLASS()`                                          | Declares a reflected class (must inherit from `UObject` or one of its subclasses). | `UCLASS(Blueprintable) class AMyActor : public AActor { … };`                   |
| `UPROPERTY()`                                       | Exposes member variables to the editor / replication.                              | `UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 Health;`                     |
| `UFUNCTION()`                                       | Declares a reflected function (BlueprintCallable, Server, etc.).                   | `UFUNCTION(BlueprintImplementableEvent) void OnHit();`                          |
| `GENERATED_BODY()`                                  | Required UE macro to generate boilerplate.                                         | `GENERATED_BODY()` placed right after class declaration.                        |
| `DECLARE_LOG_CATEGORY_EXTERN / DEFINE_LOG_CATEGORY` | Logging categories.                                                                | `DEFINE_LOG_CATEGORY(LogMyGame);` then `UE_LOG(LogMyGame, Warning, TEXT("…"));` |

---

## 3️⃣ Object System

| Concept                                   | Notes                                                                                                |
| ----------------------------------------- | ---------------------------------------------------------------------------------------------------- |
| **UObject lifecycle**                     | Objects are created via the engine (e.g.,`NewObject<UActorComponent>()`). They’re garbage‑collected. |
| **Smart pointers**                        | Use `TSharedPtr`, `TWeakPtr`, or `TUniquePtr` for non‑UObject ownership.                             |
| **Reflection** (`UProperty`, `UFunction`) | Allows dynamic access, serialization, networking.                                                    |

---

## 4️⃣ Templating & Metaprogramming

Unreal uses its own lightweight templates (mostly in the _Core_ module).Key patterns:

- `TOptional<T>` – optional value
- `TArray<T>` – dynamic array (resembles `std::vector`)
- `TMap<Key, Value>` – hash map
- `TSet<Value>` – unordered set

---

## 5️⃣ Common Containers & Types

| Type                  | Header / Namespace                    | Typical use                            |
| --------------------- | ------------------------------------- | -------------------------------------- |
| `FString`             | `<CoreTypes.h>`                       | Human‑readable string.                 |
| `FName`               | `<CoreTypes.h>`                       | Interned, case‑insensitive identifier. |
| `TArray<T>`           | `<Containers/Array.h>`                | Dynamic array; use `Add`, `Reserve`.   |
| `TMap<Key, Value>`    | `<Containers/Map.h>`                  | Dictionary / hash map.                 |
| `FVector`, `FRotator` | `<Math/Vector.h>`, `<Math/Rotator.h>` | 3‑D math types.                        |

---

## 6️⃣ Memory Management

| Feature                                  | Why it matters in UE                           | Example                                        |
| ---------------------------------------- | ---------------------------------------------- | ---------------------------------------------- |
| **Garbage Collection**                   | Non‑UObject pointers must be managed manually. | Use `TWeakObjectPtr`, `TStrongObjectPtr`.      |
| **Arena allocation** (`FMemory::Malloc`) | Faster bulk allocations for temporary data.    | `void* ptr = FMemory::Malloc(1024);`           |
| **Fast math macros**                     | Disable bounds checks, enable SIMD.            | `#define USE_FAST_MATH 1` in project settings. |

---

## 7️⃣ Networking & Replication

| Feature                   | How to use                                                 |
| ------------------------- | ---------------------------------------------------------- |
| **Replicated properties** | `UPROPERTY(Replicated)` + `GetLifetimeReplicatedProps()`   |
| **RPCs**                  | `UFUNCTION(Server, Reliable)` `<br>` `void DoSomething();` |
| **NetMulticast**          | `UFUNCTION(NetMulticast, Reliable)`                        |
| **Authority checks**      | `if (HasAuthority()) { … }`                                |

---

## 8️⃣ Blueprints & Scripting

- **BlueprintImplementableEvent / BlueprintNativeEvent** – expose C++ hooks to Blueprint.
- **Kismet Functions** (`UKismetSystemLibrary`, etc.) – static helpers for common tasks.
- **Gameplay Tags** (`FGameplayTag`) – efficient tag system.

---

## 9️⃣ Logging & Debugging

| Macro                                        | What it prints                                                          |
| -------------------------------------------- | ----------------------------------------------------------------------- |
| `UE_LOG(LogTemp, Warning, TEXT("msg"));`     | General log.                                                            |
| `UE_LOG(LogGameInstance, Error, TEXT("…"));` | Custom categories.                                                      |
| **Debug break**                              | `UKismetSystemLibrary::BreakPoint(this);`                               |
| **Print to screen**                          | `GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Hit!"));` |

---

## 🔧 Quick Build / Test Commands

```bash
# Compile a single module (UE4 uses UE4BuildTool, UE5 uses UnrealBuildTool)
UnrealBuildTool -projectfiles -project=MyProject.uproject -game -engine -progress
make MyModuleEditor

# Run automated tests
AutomationRunTests.bat
```

---

## 📚 Further Resources

- **Official Documentation** – [docs.unrealengine.com](https://docs.unrealengine.com/)
- _Unreal Engine 5 Programming Guide_ (latest PDF)
- _Game Programming Patterns_ – Robert Nystrom (for design patterns in UE)
- Community Discord / forums for UE devs

---

### Quick Code Snippet: Replicated Health Component

```cpp
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class UHealthComponent : public USceneComponent
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Health")
    float MaxHealth{100.f};

    UFUNCTION(BlueprintCallable, Category="Health")
    void TakeDamage(float Damage)
    {
        if (!HasAuthority()) return;
        CurrentHealth = FMath::Clamp(CurrentHealth - Damage, 0.f, MaxHealth);
        OnHealthChanged.Broadcast();
    }

protected:
    virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override
    {
        Super::GetLifetimeReplicatedProps(OutLifetimeProps);
        DOREPLIFETIME(UHealthComponent, CurrentHealth);
    }

private:
    UPROPERTY(Replicated)
    float CurrentHealth{100.f};

public:
    DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnHealthChanged);
    FOnHealthChanged OnHealthChanged;
};
```

Happy coding in Unreal! 🎮
