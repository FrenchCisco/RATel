# rat-el
 [![Python 3.x](https://img.shields.io/badge/Python3-blue.svg)](https://www.python.org/)  [![C++](https://img.shields.io/badge/C++-ff69b4.svg)](https://www.cplusplus.com/) 

## Description
---
RAT-el est un outil de test de pénétration open source qui permet de prendre le contrôle d'une machine windows.
Il fonctionne sur le modèle client-serveur, le serveur envoie des commandes et le client exécute les commandes puis renvoie le résultat au serveur. 

## Features
---
###### RATelServer:
- Multiple Connections 
- Broadcast commands to all clients 
- Stores client informations in the database
- Encryption of data on the network via XOR 
- Token management system to identify clients

###### Client: 
- encryption of data send over the network 
- Startup persistence 
- Remote command execution via CMD 
- Remote command execution via Powershell 
- Encryption of data on the network via XOR
- Automatic persistence when running the client 
- Automatic reconnection  

######  RATelGenerator:
- Automatic client compilation
---

## Installation
---
