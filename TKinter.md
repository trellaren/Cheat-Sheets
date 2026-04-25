This Tkinter Cheat Sheet is designed to be a quick reference for Python developers. It covers everything from basic setup to advanced event handling.

---

## 1. Basic Boilerplate
Every Tkinter application follows this fundamental structure.

```python
import tkinter as tk
from tkinter import ttk  # Themed widgets (modern look)

# 1. Create the main window
root = tk.Tk()
root.title("My App")
root.geometry("400x300") # Width x Height

# 2. Add widgets here...

# 3. Start the event loop
root.mainloop()
```

---

## 2. Geometry Managers (Layout)
Tkinter uses three different managers to position widgets. **Never mix `pack` and `grid` in the same parent container.**

### `pack()` (Simple Stacking)
Best for simple vertical or horizontal layouts.
* `side`: `tk.TOP`, `tk.BOTTOM`, `tk.LEFT`, `tk.RIGHT`
* `fill`: `tk.X` (horizontal), `tk.Y` (vertical), `tk.BOTH`
* `expand`: `True` or `False` (takes up extra space)
* `padx`, `pady`: External padding (space outside the widget)

### `grid()` (Table/Grid Layout)
The most powerful and common manager. Uses rows and columns.
* `row`, `column`: The position in the grid (starts at 0).
* `columnspan`: How many columns a widget should span.
* `rowspan`: How many rows a widget should span.
* `padx`, `pady`: Internal/External padding.

### `place()` (Absolute Positioning)
Uses exact coordinates. Hard to maintain for responsive design.
* `x`, `y`: Pixel coordinates.
* `relx`, `rely`: Relative position (0.0 to 1.0).

---

## 3. Common Widgets Reference

| Widget | Description | Key Arguments/Methods |
| :--- | :--- | :--- |
| **`Label`** | Displays text or images | `text="Hello"`, `fg="red"` (text color), `bg="black"` |
| **`Button`** | Clickable button | `command=function_name`, `state="disabled"` |
| **`Entry`** | Single-line text input | `.get()`, `.insert(index, string)`, `.delete(start, end)` |
| **`Text`** | Multi-line text area | `.get("1.0", "end")`, `.insert("end", "text")` |
| **`Checkbutton`** | Toggle checkbox | `variable=my_var` (linked to Tkinter variable) |
| **`Radiobutton`**| One of many options | `value=1`, `variable=my_var` |
| **`Frame`** | A container for widgets | Used to group widgets together |
| **`Canvas`** | Drawing area | `.create_line()`, `.create_rectangle()`, `.create_image()` |

---

## 4. Tkinter Variables (Tracing Data)
To dynamically update widgets, use Tkinter's special variable classes instead of standard Python strings/ints.

```python
# Variable types:
my_string = tk.StringVar()
my_int = tk.IntVar()
my_bool = tk.BooleanVar()

# Usage in a widget:
label = tk.Label(root, textvariable=my_string)

# Updating the variable (automatically updates the UI):
my_string.set("New Text!")

# Getting the value:
current_val = my_string.get()
```

---

## 5. Event Handling & Binding
How to make your app interactive beyond simple button clicks.

### Method A: The `command` parameter (Buttons only)
```python
def my_func():
    print("Button clicked!")

btn = tk.Button(root, text="Click Me", command=my_func)
```

### Method B: `.bind()` (For any widget/event)
Allows you to capture keyboard presses, mouse movements, etc.
```python
def handle_keypress(event):
    print(f"You pressed: {event.char}")

root.bind("<Key>", handle_keypress) # Any key press
root.bind("<Return>", lambda e: print("Enter pressed")) # Specific key
root.bind("<Button-1>", lambda e: print("Left click"))  # Mouse click
```

---

## 6. Popups & Messageboxes
Standard dialog boxes for alerts and confirmations.

```python
from tkinter import messagebox

messagebox.showinfo("Title", "Information message")
messagebox.showwarning("Warning", "This is a warning!")
messagebox.showerror("Error", "Something went wrong!")

# Confirmation Dialog
response = messagebox.askyesno("Question", "Do you want to continue?")
if response:  # True if user clicked 'Yes'
    print("Proceeding...")
```

---

## 7. Complete Quick Example
A tiny app that takes input and shows it in a label.

```python
import tkinter as tk

def greet():
    name = entry.get()
    result_label.config(text=f"Hello, {name}!")

root = tk.Tk()
root.title("Greeter App")
root.geometry("300x200")

# Widget 1: Label
tk.Label(root, text="Enter your name:").pack(pady=5)

# Widget 2: Entry
entry = tk.Entry(root)
entry.pack(pady=5)

# Widget 3: Button
btn = tk.Button(root, text="Greet Me", command=greet)
btn.pack(pady=10)

# Widget 4: Result Label
result_label = tk.Label(root, text="", font=("Arial", 12, "bold"))
result_label.pack(pady=5)

root.mainloop()
```
