#ifndef ERRORCODES_H
#define ERRORERRORCODES_H

/*This header is used to list and explain RATel's errors.*/

//GLOBAL 0 - 100:
#define RATEL_ERROR_SUCCESS  0 //0 // The operation completed successfully.
#define RATEL_ERROR_FAILS  1 //1  //The operation failed 
#define RATEL_ERROR_ACCESS_DENIED 2 
#define RATEL_ERROR_NETWORK_ERROR 3

//KEYLOGGER 101 - 200:
#define RATEL_ERROR_FILE_MANIPULATION 101 // 101 The file is currently being used by the keylogger. 
#define RATEL_ERROR_THREAD_SENDKEY  102//sendKeystrokeThread | Launching the keyboard recording thread failed. 
#define RATEL_ERROR_THREAD_RECVDATA  103 //directRecvData | The launch of the end of connection listening thread failed. 
#define RATEL_ERROR_LOG_FILE_NOT_PRESENT 104//The log file is not present on the machine.
#define RATEL_ERROR_SILENCIOUS_NOT_STARTED  105 //The mod didn't start  
#define RATEL_ERROR_KEYLOGGER_IS_WORKING 106 //the keylogger is working

//PERSISTENCE 201 - 300:
#define RATEL_ERROR_OPEN_KEY 201  //Unable to open the registry key. 
#define RATEL_ERROR_SET_SUBKEY 202//Impossible to register data in the registry subkey. 

//OTHER 301 - 400:
#define RATEL_ERROR_CHANGE_DIRECTORY 301 //Cannot change directory. 


#endif