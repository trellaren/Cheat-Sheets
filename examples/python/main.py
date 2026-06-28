from dataclasses import dataclass
from typing import Iterable


@dataclass
class User:
    name: str
    age: int

    def greet(self) -> str:
        return f"Hi, I'm {self.name}"


def squares(values: Iterable[int]) -> list[int]:
    return [value * value for value in values]


def safe_divide(a: float, b: float) -> float:
    if b == 0:
        raise ValueError("division by zero")
    return a / b


def countdown(start: int):
    while start > 0:
        yield start
        start -= 1


def main() -> None:
    user = User("Ada", 27)
    print(user.greet())
    print(squares([1, 2, 3, 4, 5]))

    try:
        print("10 / 2 =", safe_divide(10, 2))
    except ValueError as error:
        print(f"error: {error}")

    for n in countdown(3):
        print("count:", n)


if __name__ == "__main__":
    main()
