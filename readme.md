# Server-Client File Transfer

## Introduction

Ce projet consiste en un programme client-serveur permettant le transfert de fichiers depuis un serveur vers un client. Le serveur écoute sur un port spécifié et le client se connecte au serveur pour demander un fichier spécifique.

## Fichiers du Projet

- **server.c** : Le code source du serveur qui écoute sur un port spécifié, accepte les connexions des clients, gère les demandes de fichiers et envoie les données des fichiers demandés aux clients.
- **client.c** : Le code source du client qui se connecte au serveur, envoie une demande de fichier avec le nom du fichier à récupérer, reçoit les données du fichier et les affiche sur la sortie standard.
- **makefile** : Un fichier makefile pour compiler facilement les programmes.
- **readme.md** : Ce fichier, contenant des explications sur le fonctionnement du projet.

## Compilation

Pour compiler le serveur et le client, exécutez la commande suivante dans le terminal :

```bash
make
```

Cela générera deux exécutables : **server** et **client**
## Utilisation
Serveur

Le serveur doit être exécuté avec le chemin du répertoire contenant les fichiers à transférer. Voici comment lancer le serveur :

```bash
./server "/chemin/vers/le/répertoire"
```

Le serveur écoutera les connexions entrantes sur le port spécifié (dans ce cas, le port 3264) et répondra aux demandes de fichier des clients.
## Client

Le client doit être exécuté avec la taille de la fenêtre de réception et le nom du fichier à récupérer. Voici comment lancer le client :
```bash
./client <window_size> <nom_du_fichier>
```
Par exemple :
```bash
./client 1024 fichier.txt
```

Cela enverra une demande de fichier pour récupérer fichier.txt avec une taille de fenêtre de réception de 1024 octets.

## Fonctionnalités:
### Sécurité:

Le serveur vérifie la sécurité du nom de fichier envoyé par le client pour éviter les attaques de type traversal directory. Il vérifie que le nom de fichier ne contient pas de caractères de contrôle et ne contient pas de séquence .. qui pourrait permettre à un utilisateur malveillant de remonter dans l'arborescence des répertoires.

### Gestion des Erreurs:
Le serveur gère les erreurs lors de l'ouverture des fichiers et envoie des messages d'erreur appropriés aux clients en cas d'échec. Les messages d'erreur incluent des informations sur la raison de l'échec, telles que l'absence du fichier demandé ou l'absence de permissions d'accès.

### Interruption Propre:
Le serveur peut être arrêté proprement en utilisant le signal SIGINT (généralement envoyé par Ctrl+C). Lorsque le serveur reçoit ce signal, il affiche un message indiquant qu'il se ferme et termine son exécution.


**Remarque :)**
    dès fois, si vous donner un fichier qui n'éxiste pas, le programme beug (^_^) mais dès que vous relancer le client il va s'éxecuter normalemet :)