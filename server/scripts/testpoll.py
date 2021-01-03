import select
import socket

host = ""
port = 8888



sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

sock.bind((host,port))

sock.listen(1)

print("listening")
conn, address = sock.accept()
print(address[0])




try:
    while True: #millisecondes:

        conn.settimeout(3)
        print("TIMEOUT TEST...")
        try:
            data =conn.recv(1024)
        except socket.timeout:
            print("Time out !!!!!!")
        else:
            print(data)
        
        print("TESR 2")
        conn.settimeout(None) #réinitialisation  | if zero is given, the socket is put in non-blocking mode. If None is given, the socket is put in blocking mode.
        data2 = conn.recv(1234)
        print(data2)

finally:
    sock.close()
    conn.close()