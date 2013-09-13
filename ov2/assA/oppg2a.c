#include <stdio.h>
#include <unistd.h>
int a = 0;
void parent();

int main(){
	
	parent();

}

void parent(){
	vfork();
	int b = 0;
	while(b < 15){
	b++;
	a++;
	}
	printf("variable b: %d, variable a: %d\n", b, a);

}

// 
