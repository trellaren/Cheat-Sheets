from fastapi import FastAPI

app = FastAPI(title="FastAPI Example")


@app.get("/health")
def health() -> dict[str, str]:
    return {"status": "ok"}


@app.get("/hello")
def hello(name: str = "world") -> dict[str, str]:
    return {"message": f"Hello, {name}!"}


if __name__ == "__main__":
    import uvicorn

    uvicorn.run(app, host="127.0.0.1", port=8000)
