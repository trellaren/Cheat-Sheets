import argparse


def build_parser() -> argparse.ArgumentParser:
    parser = argparse.ArgumentParser(description="Simple argparse CLI example")
    parser.add_argument("--name", default="world", help="Name to greet")
    return parser


def main() -> None:
    args = build_parser().parse_args()
    print(f"Hello, {args.name}!")


if __name__ == "__main__":
    main()
