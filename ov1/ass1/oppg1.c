#include<stdio.h>
#include <stdlib.h>



int main(){
	int value = 0;
	while(1){
		allocate(value);
		value++;
	}
}

void allocate(int value) {
	int *ptr = NULL;
	ptr = malloc(100000 * sizeof(int));
	*ptr = value;
	printf("test of allocated memory: %i\n");
}
