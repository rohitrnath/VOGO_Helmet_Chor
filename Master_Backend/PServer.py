import os, errno, time
from flask import Flask, request

fifoP2J = "/opt/hmi/pipe"
fifoJ2P = "/opt/hmi/fifoJ2P"

Name = "Rohit R Nath"
start = 0
helmet = 0
startTime = 0

if not os.path.exists(fifoP2J):
    os.mkfifo(fifoP2J, 0o777)
if not os.path.exists(fifoJ2P):
    os.mkfifo(fifoJ2P, 0o777)


def send_data(str):
    print ("enter")
    fd = os.open(fifoP2J, os.O_CREAT | os.O_WRONLY | os.O_NONBLOCK)
    os.write(fd, str)
    os.close(fd)


dat = 0
app = Flask(__name__)

@app.route('/post', methods = ["POST"])
def post():

    print(request.data.decode('utf-8'))
    global dat
    if(request.data.decode('utf-8') == "Start"):
        send_data("S")
        dat = 1
    elif (request.data.decode('utf-8') == "End"):
        send_data("B")
        dat = 2
    elif(request.data.decode('utf-8') == "Error"):
        send_data("E")
        dat = 3
    else:
        print("invalid")
    print(dat)
    return ''

app.run(host='0.0.0.0', port= 8090)


# bufferSize = 100;
# while 1:
#     pipe = os.open(fifoJ2P, os.O_RDONLY | os.O_NONBLOCK);
#     try:
#         buffer = os.read(pipe, bufferSize);
#     except OSError as err:
#         if err.errno == errno.EAGAIN or err.errno == errno.EWOULDBLOCK:
#             buffer = None
#         else:
#             raise  # something else has happened -- better reraise
#
#     if buffer is None:
#         pass
#     elif buffer == "":
#         pass
#     else:
#         print(buffer);
#         print(type(buffer));
#         if(buffer == "R"):
#             send_data("S")
#     os.close(pipe);
#     time.sleep(0.2);


