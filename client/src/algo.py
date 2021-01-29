from subprocess import Popen
from subprocess  import PIPE

import os
def exec(command):

    out = b""
    err= b""

    proc = Popen(command, stderr = PIPE, stdout = PIPE,shell=True)    
    
    try:
        out, err = proc.communicate(timeout=4)
    except Exception as e:
        print(e)
        err = str(e).encode()

    out = out.decode("utf8","ignore")
    err = err.decode("utf8","ignore")

    if(out):
        #if not error
        #print(out)
        return str(out)
    else:
        return str(err)



buffer_len = 4096
index = 0
i = 0
tmp = ""
result = exec("dir C:\\windows\\system32")
list_result = []

print("goo")
while True:

    if(len(result) <= buffer_len): 
        #si le resultat de la commande est plus petit que le buffer
        list_result.append(result)
        print("first iterration" )
        break
    if(index >= len(result)):
        break
   # try:
    print(result[index : (index+buffer_len)],"\n\n")
    tmp += result[index : (index+buffer_len)]
    
    #except Exception as e:
    #    print(e)
        #Si une erreur alors cela veut dire que la taille de l'index est trop grand par rapport a la taille du buffer/
    tmp += result[index: len(result)]
    
    #finally:
    list_result.append(tmp)
    index += buffer_len

    print("index: ",index)


print("len of list: ", len(list_result))
os.system("PAUSE")