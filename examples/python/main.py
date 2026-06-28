from __future__ import annotations

from collections import Counter
from dataclasses import dataclass, field
from pathlib import Path
from tempfile import NamedTemporaryFile
from typing import Iterable


# ============================================================
# PAGE 1: BASICS, TYPES, AND CONTROL FLOW
# ============================================================
APP_NAME = "Python Example Tour"
VERSION = "1.0"


def basics_page() -> None:
    print("== PAGE 1: Basics ==")
    print(f"{APP_NAME} v{VERSION}")

    # Python supports dynamic typing with optional type hints.
    explicit: int = 7
    inferred = 5
    total = explicit + inferred
    print("explicit + inferred =", total)

    # if/elif/else + for-loop are core control-flow tools.
    for number in range(1, 4):
        if number % 2 == 0:
            print("even:", number)
        else:
            print("odd:", number)


# ============================================================
# PAGE 2: CLASSES, DATACLASSES, COMPOSITION, METHODS
# ============================================================
@dataclass(slots=True)
class User:
    name: str
    age: int
    tags: list[str] = field(default_factory=list)

    def greet(self) -> str:
        return f"Hi, I'm {self.name} and I'm {self.age} years old."

    def add_tag(self, tag: str) -> None:
        self.tags.append(tag)


def classes_page() -> None:
    print("\n== PAGE 2: Classes ==")
    user = User("Ada", 27)
    user.add_tag("engineer")
    user.add_tag("mentor")
    print(user.greet())
    print("tags:", user.tags)


# ============================================================
# PAGE 3: COLLECTIONS, COMPREHENSIONS, GENERATORS
# ============================================================
def squares(values: Iterable[int]) -> list[int]:
    # List comprehension is concise and readable.
    return [value * value for value in values]


def countdown(start: int) -> Iterable[int]:
    # Yield turns this function into a generator.
    while start > 0:
        yield start
        start -= 1


def collections_page() -> None:
    print("\n== PAGE 3: Collections ==")
    values = [1, 2, 3, 4, 5]
    print("squares:", squares(values))

    words = ["Go", "go", "Python", "python", "python"]
    # Counter is a dict subclass for frequency counting.
    frequencies = Counter(word.lower() for word in words)
    print("word frequencies:", dict(frequencies))

    for number in countdown(3):
        print("count:", number)


# ============================================================
# PAGE 4: EXCEPTIONS, FILE I/O, STANDARD LIBRARY
# ============================================================
def safe_divide(a: float, b: float) -> float:
    if b == 0:
        raise ValueError("division by zero")
    return a / b


def io_and_errors_page() -> None:
    print("\n== PAGE 4: Exceptions + File I/O ==")

    try:
        print("10 / 2 =", safe_divide(10, 2))
        safe_divide(10, 0)
    except ValueError as error:
        print("caught expected error:", error)

    # NamedTemporaryFile gives a safe temporary file path.
    with NamedTemporaryFile("w+", delete=False, suffix=".txt") as handle:
        handle.write("temporary python example line\n")
        temp_path = Path(handle.name)

    try:
        text = temp_path.read_text(encoding="utf-8").strip()
        print("read file:", text)
    finally:
        temp_path.unlink(missing_ok=True)


def main() -> None:
    basics_page()
    classes_page()
    collections_page()
    io_and_errors_page()


if __name__ == "__main__":
    main()
