## 🚀 NPM Cheat Sheet

> “The ultimate quick‑reference guide for everything you’ll do with npm.”
> (Assumes `node` & `npm` are already installed.)

| **Category**        | **Command**                     | **What it does**                                      | **Typical usage / flags**                             |
| ------------------- | ------------------------------- | ----------------------------------------------------- | ----------------------------------------------------- |
| **Project setup**   | `npm init`                      | Create a new `package.json` interactively.            | `npm init -y` → use defaults (no prompts)             |
|                     | `npm init <template>`           | Scaffold a project from an npm‑scaffolded template.   | `npm init @vitejs/app my-app -- --template react`     |
| **Installation**    | `npm install <pkg>`             | Install package locally, add to `dependencies`.       | `npm i lodash`                                        |
|                     | `-g / -g`                       | Global installation (for CLI tools).                  | `npm i -g typescript`                                 |
|                     | `--save-dev`                    | Add to `devDependencies`.                             | `npm i --save-dev jest`                               |
|                     | `--no-save`                     | Install without adding to `package.json`.             | `npm i lodash --no-save`                              |
|                     | `--force / -f`                  | Ignore warnings (e.g., peer‑dependency conflicts).    | `npm i express -f`                                    |
|                     | `-D / -S`                       | Shorthand for `--save-dev` / `--save`.                | `npm i -D eslint`                                     |
|                     | `--legacy-peer-deps`            | Install with old peer‑dependency algorithm (v7+).     | `npm i --legacy-peer-deps`                            |
| **Uninstalling**    | `npm uninstall <pkg>`           | Remove package locally.                               | `npm un lodash`                                       |
|                     | `-g`                            | Uninstall globally.                                   | `npm un -g typescript`                                |
| **Updating**        | `npm update [<pkg>]`            | Update packages to the latest compatible version.     | `npm upd`                                             |
|                     | `npm install <pkg>@latest`      | Force install the newest release of a package.        | `npm i react@latest`                                  |
| **Running scripts** | `npm run <script>`              | Execute a script defined in `package.json`.           | `npm run build`                                       |
|                     | `npm test`                      | Shortcut for `npm run test`.                          | `npm test`                                            |
|                     | `npm start`                     | Shortcut for `npm run start`.                         | `npm start`                                           |
| **Listing & info**  | `npm list [<pkg>]`              | Show dependency tree.                                 | `npm ls react`                                        |
|                     | `-g`                            | List globally installed packages.                     | `npm ls -g --depth=0`                                 |
|                     | `npm outdated`                  | Show which packages are out of date.                  | `npm outdated`                                        |
|                     | `npm view <pkg> [<field>]`      | View package metadata.                                | `npm view react version`                              |
| **Publishing**      | `npm publish`                   | Publish current project to npm registry.              | `npm publish --access public`                         |
|                     | `--tag <name>`                  | Tag a release (e.g., beta).                           | `npm publish --tag beta`                              |
| **Configuration**   | `npm config set <key> <value>`  | Set global/local npm config.                          | `npm config set cache /tmp/npm-cache`                 |
|                     | `npm config get <key>`          | Get config value.                                     | `npm config get registry`                             |
|                     | `npm config list`               | List all configs.                                     | `npm config list`                                     |
| **Registry & auth** | `npm whoami`                    | Show logged‑in user.                                  | `npm whoami`                                          |
|                     | `npm login / logout`            | Authenticate with npm registry.                       | `npm login`                                           |
|                     | `npm set registry <url>`        | Switch registry (e.g., for private repos).            | `npm config set registry https://registry.npmjs.org/` |
| **Workspaces**      | `npm install --workspace <pkg>` | Install a dependency in a specific workspace.         | `npm i -w core util`                                  |
|                     | `npm run <script> --workspaces` | Run script across all workspaces.                     | `npm run build --workspaces`                          |
| **Miscellaneous**   | `npx <cmd>`                     | Execute package binaries without installing globally. | `npx create-react-app myapp`                          |
|                     | `npm audit`                     | Scan for vulnerabilities.                             | `npm audit`                                           |
|                     | `npm ci`                        | Clean install (used in CI).                           | `npm ci --prefer-offline`                             |
|                     | `npm dedupe`                    | Deduplicate nested dependencies.                      | `npm dedupe`                                          |

---

### Quick‑Start Flow

1. **Create a project**

   ```bash
   mkdir myapp && cd myapp
   npm init -y
   ```

2. **Add runtime deps**

   ```bash
   npm i express mongoose
   ```

3. **Add dev‑tools**

   ```bash
   npm i -D nodemon eslint
   ```

4. **Define scripts** (in `package.json`)

   ```json
   "scripts": {
     "dev": "nodemon src/index.js",
     "lint": "eslint .",
     "build": "echo 'Build step'"
   }
   ```

5. **Run**

   ```bash
   npm run dev
   ```

6. **Check for updates / audit**

   ```bash
   npm outdated
   npm audit
   ```

7. **Publish** (after bumping version in `package.json`)

   ```bash
   npm publish --access public
   ```

---

### Common Gotchas

| Problem                                                                 | Fix                                                          |
| ----------------------------------------------------------------------- | ------------------------------------------------------------ |
| “EBADENGINE” error when installing a package that requires a newer Node | Update Node or use `--engine-strict=false` (not recommended) |
| Peer dependency conflicts in v7+                                        | Use `--legacy-peer-deps` or update dependencies              |
| Global installs not found in PATH                                       | Add npm’s global bin dir (`npm bin -g`) to your `$PATH`      |

---

### Handy Aliases (add to shell profile)

```bash
alias npmi='npm i'
alias npl='npm ls --depth=0'
alias nup='npm update'
alias nrc='npm run compile'
```

Enjoy a smoother npm workflow! 🎉
