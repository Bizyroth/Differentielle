#include "multi_diff.h"

int main(){
	
	uint8_t i,j;
	int t[16][16];
	
	for(i=0x0; i<=0xf; i++){
		for(j=0x0; j<=0xf; j++){
			t[i][j]=0;
		}
	}
	
	table(t);
	print_table(t);
	return 0;

}
