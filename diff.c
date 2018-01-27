#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "heys.h"
#define RANDOMFILE "/dev/urandom"


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
	int f_plaintext, f_cipher;
	
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
			printf("erreur read\n");
		}
		write(1, &s, sizeof(block_t));
		sd=s^difference;
		write(1, &sd, sizeof(block_t));
	}
	close(f);
	return;
}

block_t sous_cle(char * fichier){
	block_t res=0;
	block_t difference;
	block_t clair, chiffre;
	block_t chiffre_decrypt, clair_decrypt;
	float proba=0;
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
	
	for(l=0; l<256; l++){
		
		for(k=0; k<5000; k++){
			read(fd, &clair, sizeof(block_t));
			read(fd, &chiffre, sizeof(block_t));
			
			chiffre_decrypt=heys_subst(chiffre^subkey[l], isbox);
			clair_decrypt=heys_subst(clair^subkey[l], isbox);
			
			difference=chiffre_decrypt^clair_decrypt;
			
			if(difference==0x0606){
				count[l]++;
			}
			
		}
		
		lseek(fd, 0, SEEK_SET);
		
		
	}	
		float temp;
			
		for(k=0; k<256; k++){
			if(count[k]!=0){
				if(((float) (count[k]*100)/5000)>2){
					if(count[k]>temp){
						temp=count[k];
						res=subkey[k];
					}
					printf("  %f  %4x \n",(float) (count[k]*100)/5000, subkey[k]);
				}
			}
		}
		
	
	return res;
}

uint64_t retrouver_cle(block_t subkey, char * fichier_clair, char * fichier_chiffre){

	int fd=open(fichier_clair, O_RDONLY);
	int fd2=open(fichier_chiffre, O_RDONLY);
	block_t clair, chiffre,clair1, chiffre1;
	block_t test, test1;
	
	read(fd, &clair, sizeof(block_t));
	read(fd2, &chiffre, sizeof(block_t));

	read(fd, &clair1, sizeof(block_t));
	read(fd2, &chiffre1, sizeof(block_t));

	close(fd);
	close(fd2);
	uint64_t i;
	uint8_t j;
	uint64_t t;
	for(j=0x0; j<=0xf; j++){
		printf("j: %x\n", j);
		for(i=0x0; i<=0xfffffff; i++){
			t=((subkey&0xffff)^(j<<4));
			uint64_t key_test=t^(i<<12);
			test=heys_encrypt(clair, key_test);
			test1=heys_encrypt(clair1, key_test);
			if(test==chiffre && test1==chiffre){
				return key_test;
			}
		
		}
	}
	printf("Pas trouve...\n");
	

}

void dechiffre(ckey_t key, char * fichier){

	int f=open(fichier, O_RDONLY);
	
	block_t buffer;
	block_t deceipher;
	ssize_t r;
	while((r=read(f, &buffer, sizeof(block_t)))>0){
	
		deceipher=heys_decrypt(buffer, key);
		write(STDOUT_FILENO, &deceipher, sizeof(block_t));
	}
	
	close(f);
	return;
}

int main(){

	

}
