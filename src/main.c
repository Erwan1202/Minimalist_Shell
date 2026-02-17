#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(void){

	char buffer_fgets [1024];

	char history [10][1024];
	int history_index = 0;
	int total_commands = 0;

	while(1){
		printf("MiniShell >>");
		fflush(stdout);
		// Read user input using fgets
		// Vérification du fgets : la lecture est elle réussi (si non NULL on exit la boucle)
		if (fgets(buffer_fgets, sizeof(buffer_fgets), stdin)!= NULL){

			// Nettoyage du "\n"
			buffer_fgets[strcspn(buffer_fgets, "\n")] = 0; // strcspn : retourne la position du premier caractère de "\n" dans buffer_fgets, et on le remplace par '\0' pour terminer la chaîne de caractères proprement.
			
			// Vérification de la chaine vide
			if (strlen(buffer_fgets) == 0){
				continue; 
			}

			// Vérification de la commande "exit"
			if (strcmp(buffer_fgets, "exit") == 0){
				printf("Exiting MiniShell...\n");
				break;
			}
			
			if (strcmp(buffer_fgets, "history") == 0){
				for (int i = 0; i < (total_commands < 10 ? total_commands : 10); i++){
					int index = (history_index - 1 - i + 10) % 10; // Calcul de l'index pour afficher les commandes dans l'ordre inverse
					printf("%d: %s\n", total_commands - i, history[index]);
				}
				continue;
			}

			strcpy(history[history_index], buffer_fgets);
			history_index = (history_index + 1) % 10;
			total_commands++;


		};
		//printf("You entered: %s", buffer_fgets, "\n");
		

		// History command




	};

	return 0;
};
