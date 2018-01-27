#include "multi_diff.h"

int main(int argc, char * argv[]){

	if(argc!=2){
		fprintf(stderr, "usage: ./echantillon nbr_diff\n");
		fprintf(stderr, "Ou nbr_diff est le nombre de couple que vous voulez stocker\n");
		return 0;
	}

	int nbr_couple=atoi(argv[1]);
	echantillon(DIFF_ENTREE, nbr_couple);
	return 1;
	
}
