import click


@click.command()
@click.option("--name", default="world", show_default=True)
def main(name: str) -> None:
    click.echo(f"Hello, {name}!")


if __name__ == "__main__":
    main()
