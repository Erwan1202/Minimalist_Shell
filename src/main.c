// Inclusion des bibliothèques standard nécessaires
#include <stdio.h>      // Entrée/sortie standard (printf, fgets...)
#include <stdlib.h>     // Fonctions utilitaires (malloc, free, exit...)
#include <unistd.h>     // Fonctions POSIX (non utilisé ici mais utile pour exec...)
#include <string.h>     // Manipulation de chaînes de caractères

// À bouger dans un fichier séparé plus tard
// Définition des types de tokens possibles
typedef enum {WORD, PIPE, REDIRECT_IN, REDIRECT_OUT} TokenType;

// Structure représentant un token (type + valeur associée)
typedef struct {
    TokenType type;   // Type du token (mot, pipe, redirection...)
    char *value;      // Valeur du token (ex: "ls", ">", "|", etc.)
} Token;


int main(void){

	char buffer_fgets [1024]; // Buffer pour stocker l'entrée utilisateur (commande saisie)

	// Variables pour l'historique des commandes
	char history [10][1024]; // Tableau circulaire pour stocker les 10 dernières commandes
	int history_index = 0;   // Index du prochain emplacement dans l'historique
	int total_commands = 0;  // Nombre total de commandes saisies

	// Variables pour le lexer/tokenizer
	Token tokens[100];           // Tableau de tokens (max 100 tokens par ligne)
	int token_count = 0;         // Nombre de tokens trouvés
	char current_token[1024];    // Buffer pour accumuler le mot/token en cours
	int current_token_index = 0; // Position courante dans le buffer du token

	while(1){
		printf("MiniShell >>");
		fflush(stdout); // S'assurer que le prompt s'affiche immédiatement

		// Lecture de la ligne utilisateur
		if (fgets(buffer_fgets, sizeof(buffer_fgets), stdin)!= NULL){

			// Suppression du retour à la ligne final
			buffer_fgets[strcspn(buffer_fgets, "\n")] = 0;

			// Si la ligne est vide, on recommence la boucle
			if (strlen(buffer_fgets) == 0){
				continue; 
			}

			// Gestion de la commande "exit"
			if (strcmp(buffer_fgets, "exit") == 0){
				printf("Exiting MiniShell...\n");
				break;
			}

			// Gestion de la commande "history"
			if (strcmp(buffer_fgets, "history") == 0){
				for (int i = 0; i < (total_commands < 10 ? total_commands : 10); i++){
					int index = (history_index - 1 - i + 10) % 10; // Affichage en ordre inverse
					printf("%d: %s\n", total_commands - i, history[index]);
				}
				continue;
			}

			// Ajout de la commande à l'historique
			strcpy(history[history_index], buffer_fgets);
			history_index = (history_index + 1) % 10;
			total_commands++;

			// Début du découpage de la ligne en tokens
			for (int i = 0; buffer_fgets[i] != '\0'; i++){
				char c = buffer_fgets[i];

				// Si espace : fin d'un mot/token
				if (c == ' '){
					if (current_token_index > 0){
						current_token[current_token_index] = '\0'; // Termine le mot
						tokens[token_count].type = WORD;            // Type mot
						tokens[token_count].value = strdup(current_token); // Copie la valeur
						token_count++;
						current_token_index = 0; // Réinitialise le buffer
					}
				}
				// Si pipe : fin du mot + ajout du token PIPE
				else if (c == '|'){
					if (current_token_index > 0){
						current_token[current_token_index] = '\0';
						tokens[token_count].type = WORD;
						tokens[token_count].value = strdup(current_token);
						token_count++;
						current_token_index = 0;
					}
					tokens[token_count].type = PIPE;
					tokens[token_count].value = strdup("|");
					token_count++;
				}
				// Si redirection sortie : fin du mot + ajout du token REDIRECT_OUT
				else if (c == '>'){
					if (current_token_index > 0){
						current_token[current_token_index] = '\0';
						tokens[token_count].type = WORD;
						tokens[token_count].value = strdup(current_token);
						token_count++;
						current_token_index = 0;
					}
					tokens[token_count].type = REDIRECT_OUT;
					tokens[token_count].value = strdup(">");
					token_count++;
				}
				// Si redirection entrée : fin du mot + ajout du token REDIRECT_IN
				else if (c == '<'){
					if (current_token_index > 0){
						current_token[current_token_index] = '\0';
						tokens[token_count].type = WORD;
						tokens[token_count].value = strdup(current_token);
						token_count++;
						current_token_index = 0;
					}
					tokens[token_count].type = REDIRECT_IN;
					tokens[token_count].value = strdup("<");
					token_count++;
				}
				// Sinon, on accumule le caractère dans le mot/token en cours
				else {
					current_token[current_token_index++] = c;
				}
			}
			// Après la boucle, il peut rester un mot à ajouter
			if (current_token_index > 0){
				current_token[current_token_index] = '\0';
				tokens[token_count].type = WORD;
				tokens[token_count].value = strdup(current_token);
				token_count++;
				current_token_index = 0;
			}

			// Affichage des tokens pour debug
			for (int i = 0; i < token_count; i++) {
				printf("Token %d: type=%d, value='%s'\n", i, tokens[i].type, tokens[i].value);
				free(tokens[i].value); // Libération mémoire
			}
			token_count = 0; // Réinitialise pour la prochaine commande
		}
	}
	return 0;
}
