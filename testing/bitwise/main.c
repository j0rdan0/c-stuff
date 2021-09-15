#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
//Complete the following function.


void calculate_the_maximum(int n, int k) {
	int or,and,xor;
  for ( int i=2; i<=n;i++){
 	     or = n|i;
	     and = n&i;
	     xor = n^i;
	printf("N: %d I: %d OR:%d AND:%d XOR:%d\n",n,i,or, and,xor);
  }
}

int main() {
    int n, k;
  
    scanf("%d %d", &n, &k);
    calculate_the_maximum(n, k);
 
    return 0;
}
