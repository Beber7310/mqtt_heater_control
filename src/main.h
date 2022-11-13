/*
 * main.h
 *
 *  Created on: 8 nov. 2022
 *      Author: bdosd
 */

#ifndef MAIN_H_
#define MAIN_H_

typedef struct {
	float current;
	float power;
} power_t;

#ifdef MAIN_CODE
	#define EXT
#else
	#define EXT extern
#endif

EXT power_t			power;



#endif /* MAIN_H_ */
