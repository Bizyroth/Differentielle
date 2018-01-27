#include "multi_diff.h"




void print_table(int t [16][16]){
	int i,j;
	printf("       0   1   2   3   4   5   6   7   8   9   a   b   c   d   e   f\n");
	printf("====================================================================\n");		
	for(i=0;i<16;i++){
		printf("%x || ", i);
		for(j=0;j<16; j++){
			printf(" %2u ", t[i][j]);
		
		}
		printf("\n");

	}
	return;
}

int table(int  t [16][16]){
	block_t diffe;
	int i, j;
	for(i=0; i<16; i++){
		for(j=0; j<16; j++){
			diffe=sbox[i]^sbox[i^j];
			t[j][diffe]++;
		}
	}
	return 1;	
}


/*Creer un echantilon de couple clair chiffre avec une difference*/

void echantillon(block_t difference, int nombre_couple){
	int i;
	int f;
	
	f=open(RANDOMFILE, O_RDONLY | O_APPEND);
	if(f<0){
		fprintf(stderr, "Probleme d'ouverture de dev/urandom\n");
		return;
	}
	block_t s;
	block_t sd;
	int r=0;
	for(i=0; i<nombre_couple; i++){
		
		if( (r=read(f, &s, sizeof(block_t))<0)){
			fprintf(stderr,"erreur read\n");
		}
		if( (r=write(1, &s, sizeof(block_t)))<0){
			fprintf(stderr, "erreur write\n");
		}
		sd=s^difference;
		
		if((r=write(1, &sd, sizeof(block_t)))<0){
			fprintf(stderr, "erreur write\n");
		}
	}
	close(f);
	return;
}

/*Calcule la sous cle à partir d'un fichier contenant des couples chiffre ayant chacun la meme difference*/

block_t sous_cle(char * fichier){
	block_t res=0;
	block_t difference;
	block_t clair, chiffre;
	block_t chiffre_decrypt, clair_decrypt;
	int count[256];
	
	byte_t i, j;
	int k=0;
	int l=0;
	int fd= open(fichier, O_RDONLY);
	
	block_t subkey [256];
	for(i=0; i<=0xf; i++){
		for(j=0; j<=0xf; j++){
		
			subkey[i^(j<<4)]=(i<<8)^j;
			count[i^(j<<4)]=0;
		}
	}
	
		
	for(k=0; k<5000; k++){
				
			if(read(fd, &clair, sizeof(block_t))<0){
				fprintf(stderr, "probleme read\n");
			}
			if(read(fd, &chiffre, sizeof(block_t))<0){
				fprintf(stderr, "probleme read\n");
			}
		for(l=0; l<256; l++){		
			chiffre_decrypt=heys_subst(chiffre^subkey[l], isbox);
			clair_decrypt=heys_subst(clair^subkey[l], isbox);
			
			difference=chiffre_decrypt^clair_decrypt;
			
			if(difference==DIFF_SORTIE){
				count[l]++;
			}
			
		}
		
	//	lseek(fd, 0, SEEK_SET);
		
	}	
		float temp=0;
		float proba=0;
			
		for(k=0; k<256; k++){
			//printf("cle: %d count: %d\n", k, count[k]); 
			if(count[k]!=0){
				proba=(float) (count[k]*100)/5000;
				//printf("cle: %3d proba: %f \n", k, proba);
				if(proba>2){
					
					if(count[k]>temp){
						temp=count[k];
						res=subkey[k];
					}
					//printf("  %f  %4x \n",(float) (count[k]*100)/5000, subkey[k]);
				}
			}
		}
		
	return res;
}

/*thread permettant de retrouver la cle a partir de la sous cle*/

int co=1;                            // variable globale au thread qui se met a 0 lorsqu'un thread trouve la bonne cle

void * retrouver_cle(void * arg1){
	
	argument * arg2=malloc(sizeof(argument));;
	
	arg2 = (argument *)arg1;                          //cast du void * en argument du thread
	int k;
	int boolean=1;	                  //boolean qui verifira pour chaque cle si en chiffrant le clair avec cette cle 
	                                  //  on retrouve bien le chiffre correspondant 
	uint64_t i;
	uint8_t j;
	uint64_t t;
	uint64_t subkey=arg2->subkey;
	ckey_t * key_test=malloc(sizeof(ckey_t));
	block_t test[NBR_TEST];

		for(j=0x0; j<=0xf; j++){
	
			for(i=(arg2->debut); i<=(arg2->fin); i++){
				if(co==0){
					pthread_exit(NULL);
				}
				t=((subkey&0xffff)^(j<<4));
				*key_test=t^(i<<12);
				for(k=0; k<NBR_TEST; k++){		
					test[k]=heys_encrypt(arg2->clair[k], *key_test);
					boolean&=(test[k]==arg2->chiffre[k]);
				}
				if(boolean==1){
					co=0;
					//printf("La cle est: %lx\n", key_test);
					//affiche_key(*key_test);
					pthread_exit(key_test);
				}
				boolean=1;
			}
		}
		printf("Pas trouve...\n");
		*key_test=0;
		pthread_exit(&key_test);

}

/*dechiffre un fichier a l'aide d'une cle renvoie la sortie sur la sortie standard*/

void dechiffre(ckey_t key, char * fichier){

	int f=open(fichier, O_RDONLY);
	if(f==-1){
		fprintf(stderr, "Probleme avec l'ouverture du fichier!!\n");
		return;
	}
	block_t buffer;
	block_t deceipher;
	ssize_t r;
	while((r=read(f, &buffer, sizeof(block_t)))!=0){
	
		deceipher=heys_decrypt(buffer, key);
		if(write(1, &deceipher, sizeof(block_t))<0){
			fprintf(stderr, "probleme write\n");
		}
	}
	
	close(f);
	return;
}












void affiche_key(ckey_t key){

printf("         _____                                       \n");
printf("     _-~~     ~~-_//                                 \n");
printf("   /~             ~\\                                \n");
printf("  |              _  |_                               \n");
printf(" |         _--~~~ )~~ )___                           \n");
printf("\\|        /   ___   _-~   ~-_                        \n");
printf("\\          _-~   ~-_         \\                      \n");
printf("|         /         \\         |                      \n");
printf("|        |           |     (O  |                     \n");
printf(" |      |             |        |                    __________________________      \n");
printf(" |      |   O)        |       |                    /                          \\     \n");
printf(" /|      |           |       /                    /                            \\    \n");
printf(" / \\ _--_ \\         /-_   _-~)                    | La cle est %lx      |              \n", key);
printf("   /~    \\ ~-_   _-~   ~~~__/                     |   coin coin!!              |    \n");
printf("  |   |\\  ~-_ ~~~ _-~~---~  \\                    \\                            /    \n");
printf("  |   | |    ~--~~  / \\      ~-_                  \\                          /     \n");
printf("   |   \\ |                      ~-_                \\ _______________________/      \n");
printf("    \\   ~-|                        ~~--__ _-~~-,     |/                             \n");
printf("     ~-_   |                             /     |     /                              \n");
printf("        ~~--|                                 /      \n");
printf("          |  |                               /       \n");
printf("          |   |              _            _-~        \n");
printf("          |  /~~--_   __---~~          _-~           \n");
printf("          |  \\                   __--~~              \n");
printf("          |  |~~--__     ___---~~                    \n");
printf("          |  |      ~~~~~                            \n");
printf("          |  |                                       \n");


}





















