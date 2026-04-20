# TypeScript Cheat Sheet – Quick Reference Guide

_(for developers who want to keep the most common syntax and concepts handy)_

---

## 1️⃣ Basics

| Feature             | Syntax / Example                                                       | Notes                                                      |
| ------------------- | ---------------------------------------------------------------------- | ---------------------------------------------------------- |
| **Variables**       | `let x: number = 5;` `<br>` `const PI: number = 3.14159;`              | Use explicit types or let TypeScript infer (`let y = 10`). |
| **Primitive Types** | `string`, `number`, `boolean`, `bigint`, `symbol`, `null`, `undefined` | Prefer `number` over `any`.                                |
| **Arrays**          | `const arr: number[] = [1,2,3];` `<br>` or `Array<number>`             |                                                            |
| **Tuples**          | `let tuple: [string, number] = ['hello', 42];`                         | Fixed length & types.                                      |
| **Enums**           | `enum Color {Red, Green=5, Blue}`                                      | Auto‑increment unless set; can be numeric or string enums. |
| **Functions**       | `function add(a:number,b:number):number{ return a+b }`                 | Return type optional if inferred.                          |

---

## 2️⃣ Advanced Types

| Feature                | Syntax / Example                                                 | Notes                                      |
| ---------------------- | ---------------------------------------------------------------- | ------------------------------------------ |
| **Interfaces**         | `interface Person { name: string; age?: number; greet(): void }` | Describes object shape; can extend others. |
| **Type Aliases**       | `type ID = string                                                | number;`                                   |
| **Union Types**        | `let value: string                                               | null = null;`                              |
| **Intersection Types** | `type Admin = User & { permissions: string[] }`                  | Combines multiple types into one.          |
| **Literal Types**      | `let status: 'open'                                              | 'closed' = 'open';`                        |
| **Mapped Types**       | `type Readonly<T> = { readonly [P in keyof T]: T[P] }`           | Generic utilities.                         |
| **Conditional Types**  | `type IsString<T> = T extends string ? true : false;`            | Compile‑time logic.                        |

---

## 3️⃣ Generics

```ts
function identity<T>(arg: T): T {
  return arg;
}
const num = identity<number>(5);
```

_Use generics for reusable components that preserve type information._
`<T extends SomeInterface>` limits the generic to a subset of types.

---

## 4️⃣ Classes & Inheritance

| Feature               | Syntax / Example                                                                                                                                                                | Notes                        |
| --------------------- | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- | ---------------------------- |
| **Basic Class**       | `class Person { constructor(public name: string) {} }`                                                                                                                          | `public` creates a property. |
| **Private/Protected** | `private age: number; protected salary: number;`                                                                                                                                | Encapsulation.               |
| **Getters / Setters** | `get fullName(){ return this.name + ' Smith'; } set fullName(v){ this.name = v.split(' ')[0]; }`                                                                                |                              |
| **Inheritance**       | `class Employee extends Person { constructor(name:string, public id:number){ super(name); } }`                                                                                  | `super()` must be called.    |
| **Abstract Classes**  | `abstract class Shape{ abstract area(): number; }`                                                                                                                              | Cannot instantiate directly. |
| **Mixins**            | `type Constructor<T = {}> = new (...args: any[]) => T;` `<br>function Timestamped<TBase extends Constructor>(Base:TBase){ return class extends Base{ timestamp=Date.now(); };}` |                              |

---

## 5️⃣ Modules & Namespaces

| Feature                           | Syntax / Example                                                         | Notes                                  |
| --------------------------------- | ------------------------------------------------------------------------ | -------------------------------------- |
| **Export**                        | `export const PI = 3.14;` `<br>` `export default function foo(){}`       | Default export gets no name.           |
| **Import**                        | `import {PI} from './math';` `<br>` `import foo from './foo';`           | Use relative or absolute paths.        |
| **Re‑exports**                    | `export * from './utils';`                                               |                                        |
| **Namespaces (internal modules)** | `namespace Utils{ export function log(msg:string){ console.log(msg); }}` | Mostly legacy; use ES modules instead. |

---

## 6️⃣ Working with External Code

| Feature                  | Syntax / Example                                                | Notes                              |
| ------------------------ | --------------------------------------------------------------- | ---------------------------------- |
| **DefinitelyTyped**      | `npm i @types/lodash`                                           | Adds type definitions for JS libs. |
| **Ambient Declarations** | `declare var $: any;`                                           | For global vars without types.     |
| **Module Augmentation**  | `declare module 'express' { interface Request { user?: User }}` | Extend existing types.             |

---

## 7️⃣ Type Guards

| Guard              | Syntax / Example                                                       | What it does                        |
| ------------------ | ---------------------------------------------------------------------- | ----------------------------------- |
| `typeof`           | `if (typeof x === 'string') { ... }`                                   | Narrow primitive types.             |
| `instanceof`       | `if (x instanceof Date) { ... }`                                       | Narrow class instances.             |
| User‑defined guard | `function isString(x:any): x is string{ return typeof x==='string'; }` | Custom predicate that narrows type. |

---

## 8️⃣ Optional Chaining & Nullish Coalescing

```ts
const name = user?.profile?.name ?? "Anonymous";
```

- `?.` safely accesses nested properties.
- `??` returns the right side only if left is `null` or `undefined`.

---

## 9️⃣ Utility Types (in `lib.es5.d.ts`)

| Utility                          | Usage                         | Example                               |
| -------------------------------- | ----------------------------- | ------------------------------------- |
| `Partial<T>`                     | Makes all properties optional | `type MaybePerson = Partial<Person>;` |
| `Required<T>`                    | Opposite of `Partial`         |                                       |
| `Readonly<T>`                    | Makes all props readonly      |                                       |
| `Record<K extends keyof any, T>` | Creates a map type            | `Record<string, number>`              |
| `Pick<T, K extends keyof T>`     | Selects subset of keys        |                                       |
| `Omit<T, K extends keyof any>`   | Excludes keys                 |                                       |

---

## 🔧 Common CLI Commands

```bash
# Install TS compiler globally
npm i -g typescript

# Compile a single file
tsc file.ts

# Watch mode
tsc --watch

# Create tsconfig.json
tsc --init

# Lint (if using ESLint)
npx eslint . --ext .ts,.tsx

# Run with ts-node (for quick scripts)
npx ts-node script.ts
```

---

## 📚 Quick Reference – TypeScript Playground

Visit: [https://www.typescriptlang.org/play](https://www.typescriptlang.org/play) to experiment interactively.

---

Use this sheet as a pocket reference while coding. Happy typing!
