from flask import Flask, render_template
import os

app = Flask(__name__)

DATA_FILE = "/tmp/worker_data.txt"

@app.route('/')
def index():
    data = []
    if os.path.exists(DATA_FILE):
        with open(DATA_FILE, "r") as f:
            data = [line.strip() for line in f.readlines() if line.strip()]
    return render_template('index.html', data=data)

if __name__ == '__main__':
    print("=== Flask demarre ===")
    app.run(host='0.0.0.0', port=5000, debug=True, use_reloader=False)
