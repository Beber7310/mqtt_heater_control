//============================================================================
// Name        : main.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++
//============================================================================
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include "SerialManagement.h"

using namespace std;

int shared_power;

#define MAIN_CODE

#include "main.h"
int tube[2];

//
// Print a greeting message on standard output and exit.
//
// On embedded platforms this might require semi-hosting or similar.
//
// For example, for toolchains derived from GNU Tools for Embedded,
// to enable semi-hosting, the following was added to the linker:
//
// `--specs=rdimon.specs -Wl,--start-group -lgcc -lc -lc -lm -lrdimon -Wl,--end-group`
//
// Adjust it for other toolchains.
//
// If functionality is not required, to only pass the build, use
// `--specs=nosys.specs`.
//

int main_mqtt(int pipe);
sem_t semaphore;

int main() {
	pipe(tube);

	pthread_t th_uart_fp,th_uart_rf;
	sem_init(&semaphore, 0, 1);

	cout << "Hello Arm World!" << endl;

	if (pthread_create(&th_uart_fp, NULL, uart_filPilote_loop,(void*) tube[1]) < 0) {
		printf("START UP pthread_create error for thread uart_filPilote_loop");
		exit(1);
	}

	if (pthread_create(&th_uart_rf, NULL, uart_rf_loop,(void*) tube[1]) < 0) {
			printf("START UP pthread_create error for thread uart_rf_loop");
			exit(1);
		}


	main_mqtt( tube[0]);

	return 0;
}
