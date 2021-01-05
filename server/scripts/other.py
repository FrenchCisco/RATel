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

#session, False, ip,port, is_he_alive, is_he_admin, path_rat, usename, token

def exec(command):
    with Popen([str(command)], stdout=PIPE,stderr=PIPE,shell=True) as cmd:
        out,err = str(cmd.stdout.read(),"UTF8",errors="ignore"),str(cmd.stderr.read(),"UTF8",errors="ignore")
        if not err:
            print(out)
            return out
        else:
            print(err)
            return err


def recvall(sock,buffer):
    
    result =b""
    sock.settimeout(3)
    while True:
        try:            
            data = sock.recv(buffer)
        except socket.timeout:
            #print("Timeout")
            break
        
        else:
            if(data ==b"\r\n"):
                break

            result+=data
    sock.settimeout(None)
    return result


def printColor(status,message):
    ERROR_COLOR = "\033[31m" #red
    HELP_COLOR = "\033[35m" #magenra
    TERMINAL_COLOR = "\033[34m" #blue
    INFO_COLOR =  "\033[33m"
    ARRAY_COLOR = "\033[36m" #cayen
    RESET_COLOR = Style.RESET_ALL

    if status == "error":
        print(ERROR_COLOR+message+TERMINAL_COLOR)
    elif status == "information":
        print(INFO_COLOR+message+TERMINAL_COLOR)
    elif status == "help":
        print(HELP_COLOR+message+TERMINAL_COLOR)


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

