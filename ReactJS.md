# React JS Cheat Sheet

_A quick‑reference guide for building modern web apps with React._

---

## 1️⃣ Quick Start

| Command                      | Description                         |
| ---------------------------- | ----------------------------------- |
| `npx create-react-app myapp` | Scaffold a new project (React 18+). |
| `cd myapp && npm start`      | Run dev server.                     |
| `npm run build`              | Production bundle.                  |
| `npm test`                   | Jest + React Testing Library tests. |

> **Tip:** Use Vite (`npm create vite@latest`) for faster builds if you don’t need CRA’s built‑in features.

---

## 2️⃣ Project Structure (CRA)

```
myapp/
├─ public/          # static assets
└─ src/
   ├─ assets/       # images, fonts, etc.
   ├─ components/   # UI widgets
   ├─ pages/        # route views
   ├─ hooks/        # custom hooks
   ├─ context/      # Context providers
   ├─ redux/        # Redux slices & store (if used)
   ├─ utils/        # helpers, constants
   ├─ App.jsx
   └─ index.js
```

---

## 3️⃣ Core Concepts

| Concept       | What it is                                                 | Key Syntax                                                                    |
| ------------- | ---------------------------------------------------------- | ----------------------------------------------------------------------------- |
| **JSX**       | XML‑like syntax for describing UI.                         | `<Button onClick={handle}>Save</Button>`                                      |
| **Component** | Reusable piece of UI.                                      | `function MyComp(props) { ... }` or `class MyComp extends React.Component {}` |
| **Props**     | Read‑only data passed to components.                       | `<MyComp title="Hello" />`                                                    |
| **State**     | Mutable data local to a component (functional:`useState`). | `const [count, setCount] = useState(0);`                                      |
| **Effect**    | Side‑effects, lifecycle hooks (`useEffect`).               | `useEffect(() => { fetch(); }, []);`                                          |
| **Context**   | Global state without prop drilling.                        | `React.createContext(defaultValue)`                                           |

---

## 4️⃣ Functional Component Boilerplate

```tsx
import React, { useState, useEffect } from "react";

export const MyComponent = ({ title }) => {
  const [data, setData] = useState(null);

  useEffect(() => {
    fetch("/api")
      .then((r) => r.json())
      .then(setData);
  }, []); // run once

  if (!data) return <p>Loading...</p>;

  return (
    <div>
      <h1>{title}</h1>
      {/* render data */}
    </div>
  );
};
```

---

## 5️⃣ Common Hooks

| Hook          | Purpose                         | Example                                                     |
| ------------- | ------------------------------- | ----------------------------------------------------------- |
| `useState`    | Local state                     | `const [value, setValue] = useState(0);`                    |
| `useEffect`   | Lifecycle side‑effects          | `useEffect(() => { /* run */ }, deps);`                     |
| `useContext`  | Consume Context                 | `const user = useContext(UserContext);`                     |
| `useReducer`  | Complex state logic             | `const [state, dispatch] = useReducer(reducer, initState);` |
| `useCallback` | Memoize callbacks               | `const memoFn = useCallback(() => { ... }, deps);`          |
| `useMemo`     | Memoize values                  | `const total = useMemo(() => compute(), deps);`             |
| `useRef`      | Persist mutable value / DOM ref | `const inputEl = useRef(null);`                             |

---

## 6️⃣ Routing (react‑router-dom v6)

```tsx
import { BrowserRouter, Routes, Route, Link } from "react-router-dom";

<BrowserRouter>
  <nav>
    <Link to="/">Home</Link>
    <Link to="/about">About</Link>
  </nav>

  <Routes>
    <Route path="/" element={<Home />} />
    <Route path="about" element={<About />} />
    <Route path="*" element={<NotFound />} />
  </Routes>
</BrowserRouter>;
```

- `useNavigate()` – programmatic navigation.
- `useParams()` – read URL params.

---

## 7️⃣ Context API

```tsx
// UserContext.js
export const UserContext = React.createContext(null);

export const UserProvider = ({ children }) => {
  const [user, setUser] = useState(null);
  return (
    <UserContext.Provider value={{ user, setUser }}>
      {children}
    </UserContext.Provider>
  );
};

// In app:
<UserProvider>
  <App />
</UserProvider>;
```

Consume:

```tsx
const { user } = useContext(UserContext);
```

---

## 8️⃣ State Management Alternatives

| Library             | When to Use                              |
| ------------------- | ---------------------------------------- |
| **Redux Toolkit**   | Large, global state; complex actions.    |
| **Zustand / Jotai** | Simpler stores without boilerplate.      |
| **Recoil**          | Atom‑based state with derived selectors. |

> **Tip:** Start with Context + useReducer; migrate to Redux only if you hit scalability issues.

---

## 9️⃣ Testing

### Jest (default)

```bash
npm test   # runs all *.test.js files
```

### React Testing Library

```tsx
import { render, screen, fireEvent } from "@testing-library/react";
import MyButton from "./MyButton";

test("calls onClick", () => {
  const handle = jest.fn();
  render(<MyButton onClick={handle}>Press</MyButton>);
  fireEvent.click(screen.getByText(/press/i));
  expect(handle).toHaveBeenCalledTimes(1);
});
```

### Snapshot Testing

```tsx
import renderer from "react-test-renderer";

const tree = renderer.create(<MyComponent />).toJSON();
expect(tree).toMatchSnapshot();
```

---

## 🔟 Styling Options

| Approach              | Syntax                                          |
| --------------------- | ----------------------------------------------- |
| **CSS Modules**       | `styles.module.css` + `className={styles.btn}`  |
| **Styled‑Components** | ``const Button = styled.button\`color:red;\`;`` |
| **Tailwind CSS**      | Utility classes in JSX (needs PostCSS config).  |
| **Emotion / JSS**     | Inline styles with theming.                     |

---

## 1️⃣1️⃣ Performance Tips

- Use `React.memo` for pure components.
- Memoize expensive calculations (`useMemo`).
- Avoid anonymous functions inside render unless necessary.
- Split bundles with React.lazy + Suspense.

```tsx
const LazyComp = React.lazy(() => import("./LazyComp"));

<Suspense fallback={<Spinner />}>
  <LazyComp />
</Suspense>;
```

---

## 1️⃣2️⃣ Error Handling

| Technique                    | Usage                                               |
| ---------------------------- | --------------------------------------------------- |
| **Error Boundaries** (class) | `class ErrorBoundary extends React.Component { … }` |
| **React Query / SWR**        | Handles async data + caching.                       |
| **Try/Catch in `useEffect`** | For non‑reactive errors.                            |

---

## 1️⃣3️⃣ Deployment

- **Vercel**, **Netlify**, **GitHub Pages** – all support CRA out of the box.
- Build first: `npm run build`.
- Deploy `build/` folder.

---

## 1️⃣4️⃣ Useful Libraries & Tools

| Tool                             | Purpose                                |
| -------------------------------- | -------------------------------------- |
| **React Query / TanStack Query** | Data fetching, caching.                |
| **react-hook-form**              | Form handling with minimal re-renders. |
| **Zustand / Jotai**              | Tiny state stores.                     |
| **Framer Motion**                | Animations.                            |
| **ESLint + Prettier**            | Code quality.                          |
| **Storybook**                    | Component library docs.                |

---

## 1️⃣5️⃣ Common Gotchas

- **State updates are async** – don’t rely on the new value immediately.
- **Keys in lists** – use stable, unique IDs.
- **Avoid mutating state directly** – always copy (`{...obj}` or `[...arr]`).
- **Dependencies array in `useEffect`** – include everything used inside.
- **Context re-renders** – provide only what consumers need; memoize values.

---

## 1️⃣6️⃣ Sample Project Skeleton

```tsx
// src/App.jsx
import { BrowserRouter, Routes, Route } from "react-router-dom";
import Home from "./pages/Home";
import About from "./pages/About";

export default function App() {
  return (
    <BrowserRouter>
      <Routes>
        <Route path="/" element={<Home />} />
        <Route path="/about" element={<About />} />
      </Routes>
    </BrowserRouter>
  );
}
```

```tsx
// src/pages/Home.jsx
import { useEffect, useState } from "react";
export default function Home() {
  const [posts, setPosts] = useState([]);
  useEffect(
    () =>
      fetch("/api/posts")
        .then((r) => r.json())
        .then(setPosts),
    [],
  );
  return (
    <ul>
      {posts.map((p) => (
        <li key={p.id}>{p.title}</li>
      ))}
    </ul>
  );
}
```

---

### 🎉 Final Thought

React’s ecosystem evolves fast. Keep an eye on the official docs, and remember: _Start simple, add complexity only when needed._ Happy coding!
