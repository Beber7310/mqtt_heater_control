#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <fcntl.h>
#include <termios.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>


#include "main.h"

#include "SerialManagement.h"

/** SerialManagement.c
 *
 *  Created on: 16 d�c. 2013
 *      Author: Bertrand
 */

int fd_fil_pilote;
#define BAUDRATE B9600
#define FIL_PILOTE_DEVICE "/dev/ttyACM0"

int fd_rf;
#define BAUDRATE B9600
#define RF_DEVICE "/dev/ttyACM0"


// extern int shared_power;
// extern sem_t sem_power;

int SerialFilPilote(void)
{

	struct termios oldtio, newtio;

	/*
	 On ouvre le p�riph�rique du modem en lecture/�criture, et pas comme
	 terminal de contr�le, puisque nous ne voulons pas �tre termin� si l'on
	 re�oit un caract�re CTRL-C.
	 */
	fd_fil_pilote = open(FIL_PILOTE_DEVICE, O_RDWR | O_NOCTTY);
	if (fd_fil_pilote < 0)
	{
		perror(FIL_PILOTE_DEVICE);
		exit(-1);
	}

	tcgetattr(fd_fil_pilote, &oldtio); /* sauvegarde de la configuration courante */
	bzero(&newtio, sizeof(newtio)); /* on initialise la structure � z�ro */

	/*
	 BAUDRATE: Affecte la vitesse. vous pouvez �galement utiliser cfsetispeed
	 et cfsetospeed.
	 CRTSCTS : contr�le de flux mat�riel (uniquement utilis� si le c�ble a
	 les lignes n�cessaires. Voir la section 7 du Serial-HOWTO).
	 CS8     : 8n1 (8 bits,sans parit�, 1 bit d'arr�t)
	 CLOCAL  : connexion locale, pas de contr�le par le modem
	 CREAD   : permet la r�ception des caract�res
	 */
	newtio.c_cflag = BAUDRATE |  CS8 | CLOCAL | CREAD;

	/*
	 IGNPAR  : ignore les octets ayant une erreur de parit�.
	 ICRNL   : transforme CR en NL (sinon un CR de l'autre c�t� de la ligne
	 ne terminera pas l'entr�e).
	 sinon, utiliser l'entr�e sans traitement (device en mode raw).
	 */
	newtio.c_iflag = IGNPAR | ICRNL;

	/*
	 Sortie sans traitement (raw).
	 */
	newtio.c_oflag = 0;

	/*
	 ICANON  : active l'entr�e en mode canonique
	 d�sactive toute fonctionnalit� d'echo, et n'envoit pas de signal au
	 programme appelant.
	 */
	newtio.c_lflag = ICANON;

	/*
	 initialise les caract�res de contr�le.
	 les valeurs par d�faut peuvent �tre trouv�es dans
	 /usr/include/termios.h, et sont donn�es dans les commentaires.
	 Elles sont inutiles ici.
	 */
	newtio.c_cc[VINTR] = 0; /* Ctrl-c */
	newtio.c_cc[VQUIT] = 0; /* Ctrl-\ */
	newtio.c_cc[VERASE] = 0; /* del */
	newtio.c_cc[VKILL] = 0; /* @ */
	newtio.c_cc[VEOF] = 4; /* Ctrl-d */
	newtio.c_cc[VTIME] = 0; /* compteur inter-caract�re non utilis� */
	newtio.c_cc[VMIN] = 1; /* read bloquant jusqu'� l'arriv�e d'1 caract�re */
	newtio.c_cc[VSWTC] = 0; /* '\0' */
	newtio.c_cc[VSTART] = 0; /* Ctrl-q */
	newtio.c_cc[VSTOP] = 0; /* Ctrl-s */
	newtio.c_cc[VSUSP] = 0; /* Ctrl-z */
	newtio.c_cc[VEOL] = 0; /* '\0' */
	newtio.c_cc[VREPRINT] = 0; /* Ctrl-r */
	newtio.c_cc[VDISCARD] = 0; /* Ctrl-u */
	newtio.c_cc[VWERASE] = 0; /* Ctrl-w */
	newtio.c_cc[VLNEXT] = 0; /* Ctrl-v */
	newtio.c_cc[VEOL2] = 0; /* '\0' */

	/*
	 � pr�sent, on vide la ligne du modem, et on active la configuration
	 pour le port
	 */
	tcflush(fd_fil_pilote, TCIFLUSH);
	tcsetattr(fd_fil_pilote, TCSANOW, &newtio);

	/*
	 la configuration du terminal est faite, � pr�sent on traite
	 les entr�es
	 Dans cet exemple, la r�ception d'un 'z' en d�but de ligne mettra
	 fin au programme.
	 */
	//while (STOP==FALSE) {     /* boucle jusqu'� condition de terminaison */
	/* read bloque l'ex�cution du programme jusqu'� ce qu'un caract�re de
	 fin de ligne soit lu, m�me si plus de 255 caract�res sont saisis.
	 Si le nombre de caract�res lus est inf�rieur au nombre de caract�res
	 disponibles, des read suivant retourneront les caract�res restants.
	 res sera positionn� au nombre de caract�res effectivement lus */
	/* res = read(fd_arduino,buf,255);
	 buf[res]=0;
	 printf(":%s", buf);
	 if (buf[0]=='z') STOP=TRUE;*/

	/* restaure les anciens param�tres du port */
	//tcsetattr(fd_arduino,TCSANOW,&oldtio);
	return 0;
}

void SerialFilPiloteSendCommande(char cmd)
{
	printf("cmd %i\n", cmd);
	write(fd_fil_pilote, &cmd, 1);
}

void * uart_filPilote_loop(void * arg)
{
	char str[255];
	char buf[255];
	int index = 0;


	int res;
	int pipe;



	SerialFilPilote();

	pipe = (int)arg;
	while (1)
	{
		res = read(fd_fil_pilote, &buf[index], 255);
		index += res;

		for (int ii = 0; ii < sizeof(buf); ii++)
		{
			if ((buf[ii] == '\n') || (buf[ii] == '\r'))
			{
				buf[ii] = 0;

				index = 0;

				if (buf[0] == 'A')
				{
					// data comming from arduino is 100ma by unit.
					power.current = atof(buf + 2) / 10;
					power.power = 230 * power.current;


					sprintf(str,"%i", (int)power.power);
					write(pipe, str, 6);

				}
			}
		}
	}

}
