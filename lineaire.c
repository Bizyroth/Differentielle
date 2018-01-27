//#include "lineaire.h"
#include <stdio.h>
#include "heys.h"

void print_table(int t [16][16]){
	int i,j;
	printf("       0   1   2   3   4   5   6   7   8   9   a   b   c   d   e   f\n");
	printf("====================================================================\n");		
	for(i=0;i<16;i++){
		printf("%x || ", i);
		for(j=0;j<16; j++){
			printf(" %2d ", t[i][j]);
		
		}
		printf("\n");

	}
	return;
}

int parity(block_t b){

	block_t temp=b;
	int res=0;
	
	while(temp!=0){
		res^=(temp&1);
		temp>>=1;
	}
	
	return res;
}

int table(int  t [16][16]){
//	block_t diffe;
	block_t i, j;
	block_t si, sj;
//	int par;
	
	for(i=0; i<=0xf; i++){
			si=sbox[i];	
		for(j=0; j<=0xf; j++){
			if(parity(si)==parity(j)){
				sj=sbox[j];
				t[j][si^sj]++;
			}
			
		}
	}
	
	/*for(i=0; i<16; i++){
		for(j=0; j<16; j++){
			t[i][j]-=8;
		}
	}*/
	return 1;	
}

/*int main(){
	
	int i,j;
	int t[16][16];

	for(i=0; i<=15; i++){
		for(j=0; j<=15; j++){
			t[i][j]=0;
		}
	}

	table(t);
	print_table(t);
	return 1;

}*/
#define TAB_SIZE 16

int main (void)
{
    block_t tab[TAB_SIZE][TAB_SIZE];
    uint8_t dx = 0, count = 0;;

    for ( int i = 0; i < TAB_SIZE; i++ ) {
        for ( int j = 0; j < TAB_SIZE; j++ ) {
            tab[i][j] = 0;
        }
    }

    for ( int i = 0; i < TAB_SIZE; i++ ) {
        for ( int j = 0; j < TAB_SIZE; j++ ) {
            tab[ i ^ j ][ sbox[i] ^ sbox[j] ]++;
        }
    }

    printf("  |");
    for ( int i = 0; i < TAB_SIZE; i++ ) {
        printf("%3x", i);
    }
    printf("\n---");
    for ( int i = 0; i < TAB_SIZE; i++ ) {
        printf("---");
    }
    printf("\n");
    for ( int i = 0; i < TAB_SIZE; i++ ) {
        printf("%x |", i);
        for ( int j = 0; j < TAB_SIZE; j++ ) {
            printf("%3d", tab[i][j]);
            if ( tab[i][j] > count && i != 0 && j != 0 ) {
                count = tab[i][j];
                dx = i;
            }
        }
        printf("\n");
    }

    block_t block = 0, nblock = 0;
    byte_t sublock = 0, nsublock = 0;

    // un peu sale les 4 for imbriqués :)
    for ( int test = 0; test < 4; test++ ) {
        block = (block_t) dx << (test * 4);
        printf("De : 0x%4x  -  ", block);
        for ( int round = 0 ; round < NROUNDS - 1; round++ ) {
            nblock = 0;
            for ( byte_t i = 0; i < 4; i++ ) {
                sublock = (block >> (i*4)) & 0xf;
                nsublock = 0;
                for ( byte_t j = 0; j < TAB_SIZE; j++ ) {
                    if ( tab[sublock][j] > tab[sublock][nsublock] ) {
                        nsublock = j;
                    }
                }
                nblock ^= (block_t)(nsublock << (i*4));
            }
            block = heys_perm(nblock);
        }
        printf("Ds : 0x%4x\n", block);
    }

    return 0;
}






