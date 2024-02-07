/**
 * hello commando main program
 */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <signal.h>
#include <unistd.h>
#include "commando_app/robot.h"
#include "commando_app/pilot.h"
#include "commando_app/server.h"

static void commando_loop(void);

/* global variable used for program clean exit */
static int running = 1;

move tab_move_left[1] = {{ROTATION, {LEFT}, 50}}; // Mouvement pour tourner à gauche

move tab_u_turn[1] = {{ROTATION, {U_TURN}, 50}}; // Mouvement pour faire demi-tour qui est équivalent à tourner à gauche

move tab_move_right[1] = {{ROTATION, {RIGHT}, 50}}; // Mouvement pour tourner à droite

move tab_move_forward[1] = {{FORWARD, {3}, 60}}; // Mouvement pour avancer

/**
 * Function for CTRL+C signal management
 */
static void sigint_handler(int dummy)
{
	running = 0;
}

/**
 * Main function
 *
 * @return EXIT_SUCCESS if the program exits successfully, otherwise EXIT_FAILURE
 */
int main(void)
{
	TRACE("Commando is running\n");

	if (robot_start() == -1)
	{
		return EXIT_FAILURE;
	}

	/* Interception d'un Ctrl+C pour arrêter le programme proprement. */
	signal(SIGINT, sigint_handler);

	/* Procédure hello de manipulation du robot. */
	commando_loop();

	/* Arrêt du robot. */
	robot_stop();

	return EXIT_SUCCESS;
}
/**
 * @brief Fonction de contrôle du digramme d'état du robot en fonction des commandes recues du serveur
 *
 */
static void commando_loop()
{
	pilot_start(); // Inilisation du diagramme état du pilote

	while (running)
	{

		char *msg = serveur_read_msg(); // Lecture du message du serveur

		if (msg != NULL)
		{

			switch (msg[0])
			{
			case 'Z':
				printf("ici");
				pilot_start_move(tab_move_forward[0]); // Action du mouvement vers l'avant
				break;
			case 'Q':
				pilot_start_move(tab_move_left[0]); // Action du mouvement vers la gauche
				break;
			case 'S':
				pilot_start_move(tab_u_turn[0]); // Action du demi-tour
				break;
			case 'D':
				pilot_start_move(tab_move_right[0]); // Action du mouvement vers la droite
				break;
			case ' ':
				pilot_check_bumped(); // Vérification de l'obstacle
				break;
			case 'O':
				pilot_stop(); // Arrêt du mouvement
				running = 0;  // Arrêt du programme
				break;
			default:
				break;
			}
			free(msg); /// Libération de la mémoire
		}
	}
	pilot_stop(); // Arrêt du pilote
}
