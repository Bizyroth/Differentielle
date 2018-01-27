#ifndef MULTI_DIFF_H
#define MULTI_DIFF_H

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "heys.h"
#include <pthread.h>

#define RANDOMFILE "/dev/urandom"
#define NBR_TEST 4   // nombre de couple sur lesquel on va tester la cle
#define DIFF_SORTIE 0x0606
#define DIFF_ENTREE 0x0b00


/*Structure qui sert a passer les arguments de la fonction dans les threads*/
typedef struct argument{

	block_t clair [NBR_TEST];            //NBR_TEST couple clair chiffre a tester pour voir si l'on a la bonne cle
	block_t chiffre [NBR_TEST];
	block_t subkey;                      // sous cle que l'on a trouver à l'aide de la fonction sous_cle
	ckey_t debut;                        //debut de l'intervalle des valeurs à partir on va commencer pour 							la recherche de clé
	ckey_t fin;                          //fin de l'intervalle
	int num;                             //numero du thread (pour debuggage)
}argument;



/**************************************************************************************************
**
** Fonction qui prend en argument une table de distribution differentielle et l'affiche proprement
**
**************************************************************************************************/
void print_table(int t [16][16]);



/**************************************************************************************************
**
** Fonction qui prend en argument une table de distribution differentielle vierge et la rempli
**
**************************************************************************************************/
int table(int  t [16][16]);


/**************************************************************************************************************
**
** Fonction qui prend en argument la difference qu'il doit y avoir entre deux couples ainsi que leur nombre
** Cette fonction renvoie sur la sortie standard nombre_couple couple ayant chacun une difference de difference
**
***************************************************************************************************************/
void echantillon(block_t difference, int nombre_couple);

/*******************************************************************************************************************
**
** Fonction qui prend en argument un fichier d'echantillon chiffrer à l'aide du chiffrement que l'on souhaite casser
**
********************************************************************************************************************/
block_t sous_cle(char * fichier);


/**************************************************************************************************
**
** Thread qui prend une struct argument en argument. Elle va tester sur les NBR_TEST couple clair/chiffre
** si une cle comprise entre le debut et la fin est la bonne cle 
**
**************************************************************************************************/
void * retrouver_cle(void * arg1);


/************************************************************************************************************************
**
** Fonction qui prend en argument un fichier chiffre à l'aide du chiffre que l'on souhaite casser et d'une cle et qui le ** dechiffre
**
**************************************************************************************************************************/
void dechiffre(ckey_t key, char * fichier);




void affiche_key(ckey_t key);
#endif
