#ifndef ERRORCODES_H
#define ERRORERRORCODES_H

/*This header is used to list and explain RATel's errors.*/

//GLOBAL 0 - 100:
#define RATEL_ERROR_SUCCESS  0 //0 // The operation completed successfully.
#define RATEL_ERROR_FAILS  1 //1  //The operation failed 

//KEYLOGGER 101 - 200:

#define RATEL_ERROR_FILE_MANIPULATION 101 // 101 The file is currently being used by the keylogger. 
#define RATEL_ERROR_THREAD_SENDKEY  102//sendKeystrokeThread | Launching the keyboard recording thread failed. 
#define RATEL_ERROR_THREAD_RECVDATA  102 //directRecvData | The launch of the end of connection listening thread failed. 
//PERSISTENCE 201- 301:

#define RATEL_ERROR_OPEN_KEY 201  //Unable to open the registry key. 
#define RATEL_ERROR_SET_SUBKEY 202//Impossible to register data in the registry subkey. 


#endif