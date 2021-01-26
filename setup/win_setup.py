from subprocess import Popen
from subprocess import PIPE
import os
import time

def exec(command):

    out = b""
    err= b""

    proc = Popen(command, stderr = PIPE, stdout = PIPE,shell=True)    
    
    try:
        out, err = proc.communicate(timeout=30)
    except Exception as e:
        print(e)
        err = str(e).encode()

    out = out.decode("utf8","ignore")
    err = err.decode("utf8","ignore")

    if(out):
        #if not error
        #print(out)
        return True,str(out)
    else:
        print("ERROR: ", err)
        return False,str(err)


def setup_windows():

    result_command = ""

    print("[?] Installation of python dependencies...")
    pip_install = "py -m pip  install colorama PTable pyreadline backports.pbkdf2"
    
    result_command = exec(pip_install)

    if(result_command[0]):#Install lib in Python
        print("[+] All the Python libraries were well installed !")
    else:
        print("\n",result_command[1])
        print("[-] An error occurred during the installation of the Python libraries.")
        os.system("PAUSE")
        exit(1)

    time.sleep(1)
    print("\n[?] Test if Python and MinGw is well installed...\n")
    time.sleep(0.5)

    result_command = exec("g++ --help")
    if(result_command[0]): #test if g++ is ok 
        print("[+] MinGw is well installed on the machine.")
    else:
        print("\n",result_command[1])
        print("[-] MinGw is not installed on the machine. You need to install MinGw. You can follow the Windows installation tutorial at https://github.com/FrenchCisco/RATel/wiki/Windows-Installation-Tutorial")
        os.system("PAUSE")
        exit(1)

    result_command = exec("py -c \"print('test')\" ")
    if(result_command[0]):
        print("[+] Python3 is well installed on the machine.\n")
    else:
        print(result_command[1])
        print("[-] Python3 is not installed on the machine. You need to install Python3. You can follow the Windows installation tutorial at https://github.com/FrenchCisco/RATel/wiki/Windows-Installation-Tutorial")
        os.system("PAUSE")
        exit(1)        


#-----------------------------------------------------------------------------------------------------------------------
setup_windows()

print("\n[+] Finished installation")
os.system("PAUSE")
exit(0)

