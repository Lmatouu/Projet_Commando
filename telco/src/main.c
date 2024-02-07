/**
 * hello telco main program
 */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <signal.h>
#include <unistd.h>

#include "telco_app/remoteUI.h"
#include "telco_app/client.h"
#include "utils.h"

static void telco_loop(void);

/* global variable used for program clean exit */
static process_state running = LIVE;

/**
 * Function for CTRL+C signal management
 */
static void sigint_handler(int dummy)
{
	running = STOPPED;
}

int main(void)
{
	TRACE("Telco is runing\n");
	/* Interception d'un Ctrl+C pour arrêter le programme proprement. */
	signal(SIGINT, sigint_handler);

	/* Procédure hello de manipulation du Telco. */
	telco_loop();

	/* Arrêt du robot. */
	char msg = ' ';
	client_send_msg(msg);

	return EXIT_SUCCESS;
}

/**
 * \brief Fonction de contrôle de Telco
 *
 * Cette fonction est la fonction principale de l'application Telco.
 */

static void telco_loop()
{
	while (running)
	{
		start(&running);
	}
}
