#include <stdlib.h>
#define SIZE 1000000

int main()
{
    float *tab1 = malloc(sizeof(int)*SIZE);
    float *tab2 = malloc(sizeof(int)*SIZE);

    int i;
    int s=0;
    for(i=0; i<SIZE;i++){
	tab1[i]=1;
	tab2[i]=1;
    }

    for(i=0; i<SIZE;i++){
	s += tab1[i] * tab2[i];	
    }
    

    return 0;
}
