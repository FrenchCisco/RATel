import threading
import socket 
#from .other import XOREncryption

from colorama import Fore,Style

class FakeCmd:
    '''
    Create 2 threads, one to receive the other to send commands.
    '''
    thread_in_progress = None

    def __init__(self,sock):
        self.sock = sock

    
    def recvthread(self):
        while FakeCmd.thread_in_progress:
            try:
                data = self.sock.recv(4096)
            except ConnectionError as err:
                print(err)
                FakeCmd.thread_in_progress = False
            else:
                data_decode =  data.decode("utf8","replace")
                print("\033[32m" + data_decode.replace("\r\n\r\n","\n") + "\033[34m",end="") #supprimer les double entryyy

    def sendthread(self):

        while FakeCmd.thread_in_progress:
            
            inp  = input("")
            try:
                if(inp == "-b" or inp =="--back" or inp == "exit"):

                    self.sock.send(("exit"+"\n").encode())
                    FakeCmd.thread_in_progress = False
                
                else:
                    self.sock.send((inp+"\n").encode())
            
            except ConnectionError as err:
            
                print(err)
                FakeCmd.thread_in_progress = False 

    def main(self):

        FakeCmd.thread_in_progress = True #Reset

        trecv = threading.Thread(target = self.recvthread) 
        tsend = threading.Thread(target = self.sendthread)

        trecv.start()
        tsend.start()
        
        #0101010010101010100101010011001010100101010101010101010101010010101010101010100101010101010
        trecv.join()
        tsend.join()
        print("\n")
'''
sock = socket.socket(socket.AF_INET,socket.SOCK_STREAM)
sock.bind(("",8888))
sock.listen(2)

conn, address = sock.accept()
print("---->",conn)

fakecmd = FakeCmd(conn)

fakecmd.main()
'''

