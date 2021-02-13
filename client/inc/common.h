
#ifndef COMMON_H
#define COMMON_H

#define BUFFER_LEN 4096

//Change everything according to your wishes:
#define IP_ADDRESS "127.0.0.1" //Ip of server
#define PORT 12345 //Port of server
#define AUTO_PERSISTENCE false
#define TIMEOUT 20000 // Seconds for reconnect to server during a disconnection
#define NAME_KEY_REGISTER  "win64" 
#define XOR_KEY "ba38fab1b592c8f4b1fbbe8d915ee009a840efe504e3a833d0d8afae3b9ea148c859a8e8441877a0721d54f92fccbf12241007253cd6468194484e310798c01d8874dab3c4daa2c3ad612b7ba79ddc5b7ae80de0190f6842d6540da4b5b66c08987f8a15b3f5b80c588e42d1b6c9f149e99cc3c145cd31edaa25b095a6d509df93743a4861929533582d418aa3a766de439e938b7302001e17a6b4691480c4520a5e3a3b31012a003f7ced0ae59936953356f04b92b0e81b9f5b3703130d98118df5bdcc57e0f632b01b443a02bd23fc2f83ca27f9be70cdc657e8f4d74095f5dd88a4c88519db28a45e2e9e4ff714715d6ba53f1a88579f7f47e421b4a3526d40ead58521fbc0b56d373dce956fbb5a7d85f44e24085289b02e4782c3ed1154277fdc56467c1fe08a89d43e96ad2e73f4246c4e9238d660cf85dbc034543a13830dafb383ba61999de8eaee11ac9fa69b170461b241be69e0213e2a3f860edb29d3a1baf0f8a31b577d5893f13ca7377f0aa1bc433ff212e1e7c11cb2ad210df750cdcb6b3202655b215f7abca0f7f85803444e4fb06766a668975e7aa216abc8fdcda3bc7e2797a819b4a1eeccba75bdb49e2e0832ab171c73911362f1384480ca3152399c52b14deefc20a7ddb5e7cc9379688b37af6a42d54a32a85c20b86c4aa86361757e34e66ab0c13e7270788dc097d3db859b0474e4305dc5ffb51d" //The key to encrypt and decrypt data using the XOR algorithm

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

