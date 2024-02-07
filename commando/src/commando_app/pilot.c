// pilot.c

#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>

#include "pilot.h"

#define SENSOR_THRESHOLD 120

// ===== Constantes =====

static move_status robot_moving = S_FORGET;
static wheel reference_wheel = LEFT_WHEEL;
static int bum = 0;
static move move_save;

/**
 * @brief Liste des Evenements
 */
typedef enum
{
	E_NOP = 0,
	E_START,
	E_NO_SPEED,
	E_SPEED,
	E_CHECK_BUMP,
	E_BUMP_TRUE,
	E_BUMP_FALSE,
	E_STOP,
	NB_EVENT
} Event;

/**
 * @brief Liste des Actions
 */
typedef enum
{
	A_NOPE = 0,
	A_MOVE,
	A_BUMP,
	A_STOP,
	NB_ACTION
} Action;

typedef struct
{
	move_status etat;
	Action act;
} Transition;

static move_status myState = S_FORGET;

/**
 * @brief Matrice qui représente les transitions du diagramme à état
 */

static Transition transition[NB_STATUS][NB_EVENT] = {
	[S_IDLE][E_START] = {S_ID_RUN, A_NOPE},
	[S_ID_RUN][E_NO_SPEED] = {S_IDLE, A_NOPE},
	[S_ID_RUN][E_SPEED] = {S_RUNNING, A_MOVE},
	[S_RUNNING][E_CHECK_BUMP] = {S_BUMP, A_BUMP},
	[S_BUMP][E_BUMP_FALSE] = {S_RUNNING, A_NOPE},
	[S_BUMP][E_BUMP_TRUE] = {S_IDLE, A_NOPE},
	[S_RUNNING][E_START] = {S_RUN_ID, A_NOPE},
	[S_RUN_ID][E_NO_SPEED] = {S_IDLE, A_NOPE},
	[S_RUN_ID][E_SPEED] = {S_RUNNING, A_MOVE},
	[S_IDLE][E_STOP] = {S_DEATH, A_STOP},
	[S_RUNNING][E_STOP] = {S_DEATH, A_STOP},
};

static void pilot_etat_run(Event evt);
static int pilot_has_bumped(void);
static void pilot_make_move(move my_move);
static void pilot_performAction(Action anAction);

/**
 * @brief Fonction pour démarrer un mouvement
 *
 * @param my_move Le mouvement à démarrer
 */
extern void pilot_start_move(move my_move)
{
	// Appel de la fonction pour passer à l'état de démarrage
	pilot_etat_run(E_START);

	Event evt;

	// Vérification de la vitesse du mouvement
	if (my_move.speed == 0)
	{
		evt = E_NO_SPEED;
	}
	else
	{
		evt = E_SPEED;
		// Sauvegarde du mouvement
		move_save = my_move;
	}

	// Appel de la fonction pour passer à l'état correspondant à l'événement
	pilot_etat_run(evt);
}

/**
 * @brief Fonction pour exécuter une transition d'état en fonction d'un événement
 *
 * @param evt L'événement déclencheur de la transition
 */
static void pilot_etat_run(Event evt)
{
	Action anAction;
	move_status aState;
	anAction = transition[myState][evt].act;
	aState = transition[myState][evt].etat;
	if (aState != S_FORGET)
	{
		pilot_performAction(anAction);
		myState = aState;
	}
	if (myState == S_IDLE)
	{
		robot_stop();
	}
}

/**
 * @brief Fonction pour initialiser l'état initiale
 */
extern void pilot_start(void)
{
	myState = S_IDLE;
}

/**
 * @brief Fonction pour arrêter le mouvement en cours
 */
extern void pilot_stop(void)
{
	pilot_etat_run(E_STOP);
}

/**
 * @brief Fonction pour exécuter une action spécifique
 *
 * @param anAction L'action à exécuter
 */
static void pilot_performAction(Action anAction)
{
	switch (anAction)
	{
	case A_NOPE:
		// RIEN
		break;
	case A_MOVE:
		printf("pilot_performAction\n");
		pilot_make_move(move_save);
		break;
	case A_BUMP:
		bum = pilot_has_bumped();
		break;
	case A_STOP:
		robot_set_speed(0, 0);
		break;
	default:
		TRACE("ERROR [] Action inconnue pour pilote \n ");
		break;
	}
}

/**
 * @brief Fonction pour obtenir l'état du pilote
 *
 * @return move_status L'état actuel du pilote
 */
extern move_status pilot_get_status(void)
{
	return robot_moving;
}

/**
 * @brief Fonction pour vérifier si le robot a rencontré un obstacle
 */
extern void pilot_check_bumped(void)
{
	pilot_etat_run(E_CHECK_BUMP);

	Event evt;
	if (bum == 1)
	{

		evt = E_BUMP_TRUE;
	}
	else
	{
		evt = E_BUMP_FALSE;
	}

	pilot_etat_run(evt);
}

/**
 * @brief Fonction pour vérifier si le robot a rencontré un obstacle
 *
 * @return int 1 si le robot a rencontré un obstacle, 0 sinon
 */
static int pilot_has_bumped(void)
{
	robot_status my_status = robot_get_status();

	if (my_status.center_sensor <= SENSOR_THRESHOLD)
	{
		bum = 1;
	}
	else
	{
		bum = 0;
	}

	return bum;
}

/**
 * @brief Fonction pour effectuer un mouvement spécifique
 *
 * @param my_move Le mouvement à effectuer
 */
static void pilot_make_move(move my_move)
{
	switch (my_move.type)
	{
	case FORWARD:
		reference_wheel = LEFT_WHEEL;
		robot_set_speed(my_move.speed, my_move.speed);
		break;

	case ROTATION:
		switch (my_move.range.angle)
		{
		case NONE:
			return;
			break;

		case LEFT:
			reference_wheel = RIGHT_WHEEL;
			robot_set_speed(-my_move.speed, my_move.speed);
			break;

		case RIGHT:
			reference_wheel = LEFT_WHEEL;
			robot_set_speed(my_move.speed, -my_move.speed);
			break;

		case U_TURN:
			reference_wheel = RIGHT_WHEEL;
			robot_set_speed(-my_move.speed, my_move.speed);
			break;
		}
		break;

	default:
		TRACE("default case reached");
		break;
	}
}
