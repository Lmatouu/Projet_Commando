// client.C
/**
 * @file client.c
 * @authors Mathis VEBER
 * @brief Classe permettant l'envoie du message au serveur
 * @version 0.3
 * @date 2024-01-22
 *
 * @copyright Copyright (c) 2024
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <client.h>

#define PORT 80
#define MAX_CONNECTIONS 3

static int socket_fd;

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
void client_send_msg(char msg)
{
    struct sockaddr_in server_addr;

    // Création de la socket
    if ((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Configuration de l'adresse du serveur
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK); // Connexion à localhost
    server_addr.sin_port = htons(PORT);

    // Connexion au serveur
    if (connect(socket_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("connect failed");
        exit(EXIT_FAILURE);
    }

    // Envoi du message au serveur
    write(socket_fd, &msg, sizeof(msg));

    // Fermeture de la socket
    close(socket_fd);
}
