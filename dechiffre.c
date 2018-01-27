#include "multi_diff.h"

int main(int argc, char * argv[]){

	if(argc!=3){
		fprintf(stderr, "usage: ./dechiffre key fichier\n");
		fprintf(stderr, "ou key (en HEXA) est la cle pour dechiffrer et fichier le fichier a dechiffre\n");
		return 0;
	}
	
	ckey_t key= strtoul(argv[1], NULL, 16);
	
	dechiffre(key, argv[2]);
	
	return 0;

}
