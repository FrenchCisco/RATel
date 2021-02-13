import socket

sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

sock.bind(("",12345))
sock.listen(5)

conn,address = sock.accept()

print("CONNECT: {}:{}".format(address[0], address[1]))

while True:
    print("wait..")
    data = conn.recv(4096).decode("utf-16-le")
    print(data)

    inp = "абвгдеж эюя абвгдеж эюя  fántǐzì".encode("utf-16-le")  #https://stackoverflow.com/questions/13499920/what-unicode-encoding-utf-8-utf-16-other-does-windows-use-for-its-unicode-da#:~:text=1%20Answer&text=The%20values%20stored%20in%20memory,16%20little%2Dendian%2C%20always.
    conn.send(inp)
    print("send: ", inp)
    print(len(inp.decode("utf-16-le")))
    #https://stackoverflow.com/questions/46799465/why-my-applicaion-cannot-display-unicode-character-correctly