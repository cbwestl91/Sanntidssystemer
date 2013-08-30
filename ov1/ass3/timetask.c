#include<stdio.h>
#include <stdlib.h>
#include <errno.h>


int main(){
	int value = 0;
	while(1){
		allocate(value);
	}
}

void allocate(int value) {
	int *ptr = NULL;
	ptr = malloc(100000 * sizeof(int));

	if(ptr == NULL){
		perror("malloc");
		exit(1);
	}

	*ptr = value;
	printf("test of allocated memory: %i\n");
}
