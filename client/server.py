import socket


sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

sock.bind(("",12345))
sock.listen(5)


file = open("test.txt","wb")

conn, address = sock.accept()
try: 
    while(True):
        data = conn.recv(400)
        print('writing {} to file ....'.format(data))
        file.write(data)


finally:
    file.close()
    conn.close()
    sock.close()