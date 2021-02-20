import threading
import socket 
from colorama import Fore,Style

class FakeCmd: #TO TEST FOR UNICODE ALL
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
            
            inp  = input(">")
            try:
                
                if(inp == "-b" or inp =="--back" or inp == "exit"):

                    self.sock.send(("exit"+"\n").encode("utf8"))
                    FakeCmd.thread_in_progress = False
                
                else:
                    self.sock.send((inp+"\n").encode("utf8"))

            except ConnectionError as err:
            
                print(err)
                FakeCmd.thread_in_progress = False 

    def main(self):

        FakeCmd.thread_in_progress = True #Reset

        trecv = threading.Thread(target = self.recvthread) 
        tsend = threading.Thread(target = self.sendthread)
    
        trecv.start()
        tsend.start()


        trecv.join()
        tsend.join()

        print("\n")

