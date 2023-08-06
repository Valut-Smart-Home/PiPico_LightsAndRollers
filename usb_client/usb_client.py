from flask import Flask, Response, Request
import hid

app = Flask(__name__)
global dev
dev = hid.device()

@app.route('/', methods=['GET'])
def index():  # pragma: no cover
    return Response(open('static/index.html').read(), content_type="text/html")

@app.route('/index.js', methods=['GET'])
def index_js():
    return Response(open('static/index.js').read(), content_type="application/javascript")

@app.route('/connected', methods=['GET'])
def check_connected():
    global dev
    try:
        dev.error()
        return 'true'
    except ValueError:
        try:
            dev.open(0xcafe,0x4004)
            return 'true'
        except OSError:
            return 'false'
    

@app.route('/pwms', methods=['GET'])
def get_pwms():
    return [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0]

@app.route('/pwm', methods=['GET'])
def get_pwm():
    return str(0)
