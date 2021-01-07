#ifndef COMMON_H
#define COMMON_H
//#pragma once

#define BUFFER_LEN 4096

//Change everything according to your wishes:
#define IP_ADDRESS "192.168.0.98" //Ip of server
#define PORT 8888 //Port of server
#define TIMEOUT 3000//2 seconds for reconnect to server during a disconnection
#define TIMEOUT_SOCK 2
#define SLEEP_RECV 200 // allows to delay in order to avoid bugs
#define NAME_PROG "12.exe" //Name of prog
#define PATH_ADMIN "C:\\Windows" //Persistence path if the client is running admin mode.
#define PATH_NOT_ADMIN "C:\\Users\\$USER\\AppData\\Roaming"
//#define PATH_NOT_ADMIN "C:\\Users\\$USER\\AppData\\Roaming\\Wireshark"
#define NAME_KEY_REGISTER  "tazamail"

#define SPLIT "|SPLIT|" 

#endif

/*

$USER is changed by the user who executed the program.
*/