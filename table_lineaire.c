#include <stdio.h>
#include <inttypes.h>
typedef uint8_t byte_t;
typedef uint16_t block_t;
typedef byte_t sbox_t[16];

sbox_t sbox = {0xe, 0x4, 0xd, 0x1, 0x2, 0xf, 0xb, 0x8,
	       0x3, 0xa, 0x6, 0xc, 0x5, 0x9, 0x0, 0x7};


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


block_t sum(block_t x){
	block_t b=x;
	block_t res=0;
	while(b!=0){
		res^=(b&1);
		b>>=1;
	}
	return res;
}



int table_lin(int t [16][16]){
	
	block_t i,j,k;
	block_t x,y;
	for(i=0x00; i<=0xf;i++){
	
		for(j=0x0; j<=0xf;j++){
			x=sum(i&j);
			for(k=0x0; k<=0xf;k++){
				y=sum(k&sbox[j]);
				if(y==x){
					t[i][k]++;
				}
			}
		}
	}
	return 1;	

}



int main(){
	
	uint8_t i,j;
	int t[16][16];
	
	for(i=0x0; i<=0xf; i++){
		for(j=0x0; j<=0xf; j++){
			t[i][j]=-8;
		}
	}
	
	table_lin(t);
	int res=0;
	for(i=0x0; i<=0xf; i++){
		for(j=0x0; j<=0xf; j++){
			if(t[i][j]!=0){
				res++;	
			}
		}
	}

	print_table(t);

	printf("res: %d\n", res);
	return 0;

}
