// pilot.h

#ifndef PILOT_H
#define PILOT_H

#include "robot.h"
#include "../utils.h"

/**
 * @brief Liste des États possibles du robot
 */
typedef enum
{
	S_FORGET = 0,
	S_IDLE, 
	S_ID_RUN,
	S_RUNNING, 
	S_RUN_ID,
	S_BUMP,
	S_DEATH,
	NB_STATUS
} move_status;

/**
 * @brief Énumération représentant le type d'un mouvement
 */
typedef enum
{
	FORWARD = 0,
	ROTATION
} move_type;

/**
 * @brief Énumération représentant les angles de rotation disponibles
 */
typedef enum
{
	NONE = 0,
	LEFT,
	RIGHT,
	U_TURN
} angle;

/**
 * @brief Structure d'un mouvement
 */
typedef struct
{
	move_type type;
	union
	{
		int distance;
		angle angle;
	} range;
	speed_pct speed;
} move;

/**
 * @brief Fonction pour démarrer un mouvement
 *
 * @param my_move Le mouvement à démarrer
 */
extern void pilot_start_move(move a_move);

/**
 * @brief Fonction pour obtenir l'état du pilote
 *
 * @return move_status L'état actuel du pilote
 */
extern move_status pilot_get_status(void);

/**
 * @brief Fonction pour vérifier si le robot a rencontré un obstacle
 */
extern void pilot_check_bumped(void);

/**
 * @brief Fonction pour initialiser l'état du pilote
 */
extern void pilot_start(void);

/**
 * @brief Fonction pour arrêter le mouvement en cours
 */
extern void pilot_stop(void);

#endif // PILOT_H
