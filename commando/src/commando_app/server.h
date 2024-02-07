// serveur.h

#ifndef SERVER_H
#define SERVER_H

#include "robot.h"
#include "../utils.h"

/**
 * @brief Permet de récupérer le caractère envoyé de telco par le client
 *
 * @return Le message reçu (chaine de caractères)
 */
char *serveur_read_msg(void);

#endif // SERVER_H
