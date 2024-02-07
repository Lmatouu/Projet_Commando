// server.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "pilot.h"
#include "server.h"

#define PORT 80
#define BUFFER_SIZE 1024
#define MAX_CONNECTIONS 3

static int server_fd, new_socket;

/**
 * @brief Permet de récupérer le caractère envoyé de telco par le client
 *
 * @return Le message reçu (chaine de caractères)
 */
extern char *serveur_read_msg()
{
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    // Création de la socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("Échec de la création de la socket");
        exit(EXIT_FAILURE);
    }

    // Configuration des options de la socket
    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)))
    {
        perror("Échec de la configuration des options de la socket");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Liaison de la socket à l'adresse et au port
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("Échec de la liaison de la socket");
        exit(EXIT_FAILURE);
    }

    // Écoute des connexions entrantes
    if (listen(server_fd, MAX_CONNECTIONS) < 0)
    {
        perror("Échec de l'écoute des connexions");
        exit(EXIT_FAILURE);
    }

    TRACE("Serveur en écoute sur le port %d \n", PORT);

    // Accepte la connexion entrante
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
    {
        perror("Échec de l'acceptation de la connexion");
        exit(EXIT_FAILURE);
    }

    TRACE("Connexion acceptée\n");

    // Alloue de la mémoire pour une chaîne de caractères de taille BUFFER_SIZE
    // La mémoire allouée est initialisée à zéro (calloc)

    char *msg = (char *)calloc(BUFFER_SIZE, sizeof(char));
    if (msg == NULL)
    {
        TRACE("Échec de l'allocation de mémoire\n");
        exit(EXIT_FAILURE);
    }

    // Lit le message envoyé par le client
    ssize_t bytesRead;
    while ((bytesRead = read(new_socket, msg, BUFFER_SIZE - 1)) > 0)
    {
        msg[bytesRead] = '\0';
        TRACE("Message reçu du serveur : %s\n", msg);
    }
    if (bytesRead == -1)
    {
        perror("Erreur lors de la réception de la réponse du serveur");
        exit(EXIT_FAILURE);
    }

    // Ferme la connexion
    close(new_socket);
    close(server_fd);

    return msg;
}
