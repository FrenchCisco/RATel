import argparse	
from subprocess import PIPE	
from subprocess import Popen	
from subprocess import DETACHED_PROCESS	

import time	

parser = argparse.ArgumentParser()	
parser.add_argument("-nb","--number", dest="NUMBER")	
parser.add_argument("-n","--name", dest="NAME")	

try:	
    argv = vars(parser.parse_args())	

    number = int(argv["NUMBER"])	
    name = str(argv["NAME"])	
except:	
    print("ERROR")	
    exit(0)	

i = 0	
while i < number:	
    time.sleep(0.1)	
    Popen([name], shell=True ,creationflags=DETACHED_PROCESS)	

    i+=1	


print("FINISH") 