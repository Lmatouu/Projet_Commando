// remoteUI.C
/**
 * @file remoteUI.c
 * @authors Mathis VEBER
 * @brief Classe permettant de détecter les touches du clavier sur le terminal
 * @version 2.3
 * @date 2024-02-06
 *
 * @copyright Copyright (c) 2024
 *
 */

#include <stdio.h>
#include <ctype.h>
#include <termios.h>
#include <unistd.h>
#include <remoteUI.h>
#include <sys/select.h> // Pour select(), FD_ZERO(), FD_SET(), fd_set
#include <sys/time.h>   // Pour struct timeval
#include <client.h>
#include <../utils.h>

static void manu_display_menu(void);
static void setBufferedInput(int enable);
static int non_blocking_getc(void);

/**
 * @version 1
 *
 * @brief Permet d'afficher les différentes interactions possibles avec la console en mode Manuel
 */
static void manu_display_menu()
{
    // ===== Affichage du Menu =====
    printf("\n=================MENU=================\n");
    printf("\033[1;4mDéplacement du Robot\033[0m\n");
    printf("\033[1m'Z'\033[0m permet de faire avancer le Robot\n");
    printf("\033[1m'Q'\033[0m permet de faire tourner le Robot vers la gauche\n");
    printf("\033[1m'D'\033[0m permet de faire un demi-tour équivalent à tourner à gauche\n");
    printf("\033[1m'S'\033[0m permet de faire tourner le Robot vers la droite\n");
    printf("\033[1m' '\033[0m permet de détecter la présence d'obstacle(s)\n");
    printf("\033[1m'M'\033[0m permet d'afficher les différentes interactions possibles avec la console\n");
    printf("\033[1m'O'\033[0m permet de stopper le Robot et stopper l'éxécutable\n");
}

/**
 * @author Intelligence Artificielle ChatGPT
 *
 * @version 1
 *
 * @brief Définit le mode d'entrée du terminal en mode tamponné ou non tamponné.
 *
 * @param enable Indicateur indiquant s'il faut activer ou désactiver le mode d'entrée tamponné.
 *               Défini à 0 pour désactiver le mode d'entrée tamponné et entrer en mode manuel.
 *               Défini à une valeur non nulle pour restaurer le mode d'entrée du terminal.
 */

static void setBufferedInput(int enable)
{
    static struct termios oldt, newt; // Conserver l'état du terminal entre les appels
    static int isFirstCall = 1;       // Vérifier si c'est le premier appel à la fonction

    // ===== Désactiver le mode de saisie tamponnée =====
    if (!enable && isFirstCall) // Vérifier si c'est le premier appel à la fonction
    {
        printf("Bonjour, vous entrez en mode Manuel.\nPour plus d'information, cliquez sur 'M'\n");
        tcgetattr(STDIN_FILENO, &oldt); // Sauvegarder l'état actuel du terminal

        newt = oldt;                      // Copier l'état du terminal dans une nouvelle structure
        newt.c_lflag &= ~(ICANON | ECHO); // Désactiver la saisie tamponnée et l'écho

        tcsetattr(STDIN_FILENO, TCSANOW, &newt); // Appliquer les nouveaux paramètres au terminal
        isFirstCall = 0;                         // Marquer que ce n'est plus le premier appel
    }
    // ===== Rétablir l'état du terminal =====
    else if (enable)
    {
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    }
}

/**
 * @author Intelligence Artificielle ChatGPT
 *
 * @version 1
 *
 * @brief Permet de lire un caractère sans bloquer l'éexécution du code
 *
 * @return int caractère voulue sinon -1
 */
static int non_blocking_getc(void)
{
    struct timeval tv = {0L, 50000L}; // Définir le délai d'attente à 50 000 microsecondes

    fd_set fds;      // Déclarer un ensemble de descripteurs de fichiers (fd_set)
    FD_ZERO(&fds);   // Initialiser l'ensemble à zéro
    FD_SET(0, &fds); // Ajouter l'entrée standard (0) à l'ensemble

    if (select(1, &fds, NULL, NULL, &tv)) // Surveiller l'entrée standard avec un délai d'attente
    {
        return getchar(); // Caractère lu avec getchar()
    }
    else
    {
        return -1; // Aucune entrée n'est disponible dans le délai d'attente
    }
}

/**
 * @brief Démarre l'application telco.
 *
 * Cette fonction permet de démarrer l'application telco en attendant une interaction de l'utilisateur.
 * Elle lit un caractère non-bloquant depuis l'entrée standard et effectue une action en fonction de la touche appuyée.
 *
 * @param running Pointeur vers l'état d'exécution du processus principal.
 */

extern void start(process_state *running)
{

    char key;

    setBufferedInput(0); // Désactive le tampon d'entrée pour obtenir un caractère immédiatement

    key = toupper(non_blocking_getc()); // Fonction non-bloquante qui lit un caractère

    // ===== Interaction =====
    switch (key)
    {
    // ===== Mouvement Robot=====
    case 'Z':
    case 'Q':
    case 'S':
    case 'D':
    case ' ':
        TRACE("Touche appuyée dans le terminal: %c\n", key);
        client_send_msg(key);
        break;

    // ===== Quitter la Mode Manuel =====
    case 'O':
        client_send_msg(key);
        TRACE("Bye, vous quittez le mode Manuel et le terminal\n");
        setBufferedInput(1); // Réactive le tampon d'entrée pour obtenir un caractère immédiatement
        *running = STOPPED;  // Met à jour la variable running dans la classe principale
        break;

    // ===== Affichage du Menu =====
    case 'M':
        manu_display_menu();
        break;

    default:
        break;
    }
}