import platform
from subprocess import Popen
from subprocess import PIPE


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
        print(out)
        return True,out
    else:
        print("ERROR: ", err)
        return False,err


def setup_linux():
    pass


def setup_windows():
    print("[?] Installation of python dependencies...")
    pip_install = "py pip install colorama PTable pyreadline backports.pbkdf2"
    
    if(exec()[0]):
        print("[+] All the Python libraries were well installed !")
    else:
        print("[-] An error occurred during the installation of the Python libraries.")
    

#-----------------------------------------------------------------------------------------------------------------------

os = platform.system()
if(os == "Linux"):
    setup_linux()
else: #if windows
    setup_windows()

