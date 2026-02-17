#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(void){
	char buffer_fgets [1024];

	while(1){
		printf("MiniShell >>");
		fflush(stdout);
	
		fgets(buffer_fgets, 1024, stdin);
		//printf("You entered: %s", buffer_fgets, "\n");

		


	}

	return 0;
}
