#include "../include/input.h"

/* Cette fonction lit le fichier d'entree pour verifier sa coherence et determiner le nombre des villes et leurs noms
 * file_name (IN) 	: nom du fichier à lire 
 * nbr_cities (OUT)	: nombre des villes
 * return: tableau avec le nom des villes
 */
char** get_cities_labels(char *file_name, int *nbr_cities)
{
	// Lecture du fichier csv contenant la matrice de cout de taille quelconque
	// Cette methode est inspire de ces reference: 
	//	https://solarianprogrammer.com/2019/04/03/c-programming-read-file-lines-fgets-getline-implement-portable-getline/
	//  https://koor.fr/C/cstring/strtok.wp
	
	int nbr_lines = 0; // conteur du nombre des lignes
	int column;
	char **cities_labes;
	
	// ouverture du fichier
	FILE* fp = fopen(file_name,"r");
	
	// Verification de l'existance du fichier
	if(fp == NULL)
	{
		perror("Unable to open file!");
		exit(1);
	}
	
	char chunk[128];
	
	// Enrefitrement des parties du texte dans un buffer de ligne
	size_t len = sizeof(chunk);
    char *line = malloc(len);
    if(line == NULL) {
        perror("Unable to allocate memory for the line buffer.");
        exit(1);
    }

    // Vider le buffer de la line
    line[0] = '\0';
	
	while(fgets(chunk, sizeof(chunk), fp) != NULL) {
		// Redimentionner le buffer du ligne si necessaire
		size_t len_used = strlen(line);
		size_t chunk_used = strlen(chunk);
		
		if(len - len_used < chunk_used) {
			len *= 2;
			if((line = realloc(line, len)) == NULL) {
				perror("Unable to reallocate memory for the line buffer.");
				free(line);
				exit(1);
			}
		}
		
		// Copie de chunk a la fin du buffer de ligne
		strncpy(line + len_used, chunk, len - len_used);
		len_used += chunk_used;
		
		// Verifiersi line contient '\n', si oui traitement de la ligne
		if(line[len_used - 1] == '\n') 
		{
			if (nbr_lines == 0) // c'est la premiere lignes
			{
				// besoin d'une ligne temporaire puisque on va faire deux passage de strtoc, un premier pour compter et un deuxieme pour lire
				char tmp_line[strlen(line)];
				strncpy(tmp_line,line,strlen(line));
				// decomposition des donnees
				char* token = strtok(tmp_line, ",");
				column = 0;
				while (token != NULL)
				{
					column++;
					// On demande le token suivant
					token = strtok ( NULL, ",");
				}
				*nbr_cities = column - 1;
				// allouer la mémoire de cities_labes
				cities_labes = (char**)malloc(*nbr_cities * sizeof(char*));
				char* token1 = strtok(line, ",");
				column = 0;
				while (token1 != NULL)
				{
					//fprintf(stderr," I am here 3\n");
					if (column == 0)
					{
						column++;
						token1 = strtok ( NULL, ",");
						continue;
					}
					if (column == (*nbr_cities))
					{	// il faut enlever "\n" de la derniere ville
						cities_labes[column-1] = (char*)malloc((strlen(token1)-2)*sizeof(char));
						strncpy(cities_labes[column-1],token1,(strlen(token1)-2));
					}
					else
					{
						cities_labes[column-1] = (char*)malloc(strlen(token1)*sizeof(char));
						strncpy(cities_labes[column-1],token1,strlen(token1));
					}
					column++;
					// On demande le token suivant
					token1 = strtok ( NULL, ",");
				}
				nbr_lines++;
			}
			else
			{
				// decomposition des donnees
				char* token = strtok(line, ",");
				column = 0;
				while (token != NULL)
				{
					if (column == 0)
					{
						// la premiere colonne contient le nom des ville de depart
						// il faut verifier que c'est dans le meme ordre que les ville de distination
						if (strcmp(token,cities_labes[nbr_lines-1]) != 0)
						{	// il y a un probleme
							perror("Inconsistant cities labels in the cost matrix 1");
							free(line);
							exit(1);
						}
						// C'est bon on peut continuer
						column++;
						// On demande le token suivant
						token = strtok ( NULL, ",");
					}
					else
					{
						column++;
						// On demande le token suivant
						token = strtok ( NULL, ",");
					}
				}
				nbr_lines++;
				// Verifier que le nombre des lignes de la matrice est égal au nombre des colonnes
				if (nbr_lines > *nbr_cities+1)
				{
					perror("The number of rows and columns in the cost matrix is not the same");
					exit(1);
				}
			}
			
			// Vider le buffer de la line
			line[0] = '\0';
		}
	}
	// Verifier que le nombre des lignes de la matrice est égal au nombre des colonnes
	if (nbr_lines != *nbr_cities+1)
	{
		perror("The number of rows and columns in the cost matrix is not the same");
		exit(1);
	}
	
	// fermeture du fichier
	fclose(fp);
	free(line);
	
	return cities_labes;
}

/* Cette fonction lit le fichier d'entree et remplit la matrice des couts
 * file_name (IN) 	: nom du fichier à lire 
 * cost_matrix (OUT): matrice des couts
 */
void get_cost_matrix(char *file_name, int **cost_matrix)
{
	int nbr_lines = 0; // conteur du nombre des lignes
	int column;
	
	// ouverture du fichier
	FILE* fp = fopen(file_name,"r");
	
	char chunk[128];
	
	// Enrefitrement des parties du texte dans un buffer de ligne
	size_t len = sizeof(chunk);
    char *line = malloc(len);
    if(line == NULL) {
        perror("Unable to allocate memory for the line buffer.");
        exit(1);
    }

    // Vider le buffer de la line
    line[0] = '\0';
	
	while(fgets(chunk, sizeof(chunk), fp) != NULL) {
		// Redimentionner le buffer du ligne si necessaire
		size_t len_used = strlen(line);
		size_t chunk_used = strlen(chunk);
		
		if(len - len_used < chunk_used) {
			len *= 2;
			if((line = realloc(line, len)) == NULL) {
				perror("Unable to reallocate memory for the line buffer.");
				free(line);
				exit(1);
			}
		}
		
		// Copie de chunk a la fin du buffer de ligne
		strncpy(line + len_used, chunk, len - len_used);
		len_used += chunk_used;
		
		// Verifiersi line contient '\n', si oui traitement de la ligne
		if(line[len_used - 1] == '\n') 
		{
			if (nbr_lines == 0) // c'est la premiere lignes
			{
				nbr_lines++;
			}
			else
			{
				// decomposition des donnees
				char* token = strtok(line, ",");
				column = 0;
				while (token != NULL)
				{
					if (column == 0)
					{
						column++;
						// On demande le token suivant
						token = strtok ( NULL, ",");
					}
					else
					{
						cost_matrix[nbr_lines-1][column-1] = atoi(token);
						// Verification de la diagonale
						if ((nbr_lines-1 == column-1) && (cost_matrix[nbr_lines-1][column-1] != -1))
						{
							printf("Warning: La diagonale de la matrice de coût doit être égale à -1 \n");
							printf("La valeur dans la ligne %d colonne %d sera imposée \n",nbr_lines-1,column-1);
							cost_matrix[nbr_lines-1][column-1] = -1;
						}
						column++;
						// On demande le token suivant
						token = strtok ( NULL, ",");
					}
				}
				nbr_lines++;
			}
			// Vider le buffer de la line
			line[0] = '\0';
		}
	}
	
	// fermeture du fichier
	fclose(fp);
	free(line);
	
	return;

}
