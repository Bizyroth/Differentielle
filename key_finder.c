#include "multi_diff.h"
#define NBR_THREAD 16

/*./key_finder fichier1 fichier2         fichier1: couple de clair, fichier2: couple de chiffre */

int main(int argc, char * argv[]){

/*  test du nombre d'argument   */

	if(argc!=3){
	
		fprintf(stderr, "usage: ./key_finder fichier1 fichier2\n");
		fprintf(stderr, "Ou fichier1 contient les couples clairs et fichier2 les couples chiffres\n");
		return 0;
	}
	
	ckey_t * key[NBR_THREAD];
	int m;
	for(m=0; m<NBR_THREAD; m++){
		key[m]=NULL;
	}
	block_t subkey;
	
	subkey=sous_cle(argv[2]);               // On calcule la sous cle a l'aide du fichier de chiffre
	
	printf("La sous cle est %x\n", subkey);
	
	
	argument argu[NBR_THREAD];              //argument des threads 
	pthread_t t_thread[NBR_THREAD];         //ensemble de thread
	
	int fd=open(argv[1], O_RDONLY);         //ouverture du fichier de clair
	int fd2=open(argv[2], O_RDONLY);        //ouverture du fichier de chiffre
	
	
	/*============test d'ouverture================ */
	if(fd==-1){
		fprintf(stderr, "Probleme lors de l'ouverture du fichier1\n");
		return 0;
	}
	if(fd2==-1){
		fprintf(stderr, "Probleme lors de l'ouverture du fichier2\n");
		return 0;
	}
	
	
	
	block_t clair, chiffre;               // les clairs/chiffres qui permettent de tester si on a la bonne cle
	int i, j;
	/*=======================Remplissage des arguments à passer au thread===========================*/
	for(j=0; j<NBR_THREAD; j++){
	
	
		for(i=0; i<NBR_TEST; i++){
	
	
	
	
			ssize_t r;
			r=read(fd, &clair, sizeof(block_t));
			if(r==-1){
				fprintf(stderr, "Probleme de lecture dans le fichier1, verifier vos droits\n");
				return 0;
			}
	
			r=read(fd2, &chiffre, sizeof(block_t));
			if(r==-1){
				fprintf(stderr, "Probleme de lecture dans le fichier2, verifier vos droits\n");
				return 0;
			}
	
			argu[j].clair[i]=clair;                          //remplissage des couples clair chiffre
			argu[j].chiffre[i]=chiffre;
		}
	
		argu[j].subkey=subkey;                                  //ajout de la sous cle

/*===========================decoupage pour le recherche de cle en fonction de NBR_THREAD==========================*/
		if(j==0){
			argu[j].debut=0x0;
			argu[j].fin=0xfffffff/NBR_THREAD;
		}
		else{
			argu[j].debut=argu[j-1].fin+1;
			argu[j].fin=(0xfffffff*(j+1)) /(NBR_THREAD);
			
		}
		argu[j].num=j+1;
	
	}
	close(fd);
	close(fd2);
	
	
/*=============================Creation des threads===========================================*/		
	for(j=0; j<NBR_THREAD; j++){
		if (pthread_create(&t_thread[j], NULL, retrouver_cle, &argu[j])) {
			perror("pthread_create");
			return 1;
		}
	}
/*==============================Execution des threads========================================*/	
	for(j=0; j<NBR_THREAD; j++){
		if (pthread_join(t_thread[j], (void *) &key[j])) {
			perror("pthread_join");
			return 1;
		}

	}
	for(j=0; j<NBR_THREAD;j++){
		if(key[j]!=0){
			affiche_key(*key[j]);
		}
	}
	
	return 0;

}
