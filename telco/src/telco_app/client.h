// client.h

#ifndef CLIENT_H
#define CLIENT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>

/**
 * @authors Mathis VEBER
 *
 * @version 1
 *
 * @brief Envoie un message au serveur.
 *
 * Cette fonction crée une socket, se connecte au serveur en utilisant le port 80,
 * puis envoie le message fourni au serveur.
 *
 * @param msg Le message à envoyer qui correspond à la lettre du clavier appuyée par l'utilisateur.
 */
void client_send_msg(char msg);



#endif // CLIENT_H