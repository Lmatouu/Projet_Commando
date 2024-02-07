// remoteUI.h

#ifndef MANU_H
#define MANU_H

/**
 * @brief Énumération représentant l'état d'un processus.
 */
typedef enum
{
    STOPPED = 0, /**< Le processus n'est pas en cours d'exécution. */
    LIVE         /**< Le processus est en cours d'exécution. C'est l'état initial */
} process_state;

/**
 * @brief Démarre l'application robot.
 *
 * Cette fonction permet de démarrer l'application robot en attendant une interaction de l'utilisateur.
 * Elle lit un caractère non-bloquant depuis l'entrée standard et effectue une action en fonction de la touche appuyée.
 * Les actions possibles sont le mouvement du robot, quitter le mode manuel et afficher le menu.
 *
 * @param running Pointeur vers l'état d'exécution du processus principal.
 */
void start(process_state *running); // Fonction général qui gère le mode manuel

#endif //
