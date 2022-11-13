/*
 * SerialManagement.h
 *
 *  Created on: 8 nov. 2022
 *      Author: bdosd
 */

#ifndef SERIALMANAGEMENT_H_
#define SERIALMANAGEMENT_H_

#ifdef __cplusplus
extern "C" {
#endif


void * uart_filPilote_loop(void * arg);

void SerialFilPiloteSendCommande(char cmd);
int SendBlyssCmd(int id,int value);


#ifdef __cplusplus
}
#endif

#endif /* SERIALMANAGEMENT_H_ */
