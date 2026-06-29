from flask import Flask, jsonify, request

app = Flask(__name__)


@app.get("/health")
def health() -> tuple[dict[str, str], int]:
    return jsonify({"status": "ok"}), 200


@app.get("/hello")
def hello() -> tuple[dict[str, str], int]:
    name = request.args.get("name", "world")
    return jsonify({"message": f"Hello, {name}!"}), 200


if __name__ == "__main__":
    app.run(host="127.0.0.1", port=5000, debug=True)
