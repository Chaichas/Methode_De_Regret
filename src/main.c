#include <stdio.h>
#include <limits.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "../include/travel.h"
#include "../include/input.h"

int main(int argc, char *argv[])
{
	int nbr_cities; 		// Nombre de villes
	char **cities_labes; 	// Un tableau qui contient les nom des villes
	int **cost_matrix; 		// Matrice de cout
	int best_cost = INT_MAX; // cout du meilleur chemin, initialiser à grande valeu
	int **best_path; 		// Tableau contenant les trajet du chemin
	Node *root;				// Le noeud racine de l'arbre de recherche

	printf("****** Lecture du problème à partir de \"%s\" ****** \n", argv[1]);

	cities_labes = get_cities_labels(argv[1], &nbr_cities);

	// Allocation de la memoire pour cost_matrix
	cost_matrix = (int**)malloc(nbr_cities * sizeof(int*));
	for (int i = 0; i < nbr_cities; i++)
	{
	    cost_matrix[i] = (int*)malloc(nbr_cities * sizeof(int));
	}
	
	get_cost_matrix(argv[1], cost_matrix);
	
	printf("Lecture du problème avec succès. Le nombre de villes est %d . \n \n",nbr_cities);
	
	printf("****** Début de la résolution avec la méthode des regrets ****** \n");

	/*for (int i = 0; i < nbr_cities; i++)
	{
		fprintf(stderr," %s \n",cities_labes[i]);
	}*/

	// Allocation de la memoire au tableau best_path 
	best_path = (int**)malloc(nbr_cities * sizeof(int*));
	for (int i = 0; i < nbr_cities; i++)
	{
	    best_path[i] = (int*)malloc(2 * sizeof(int));
	}
	
	// Initalisation du noeud racine
	root = initial_node(cost_matrix, nbr_cities);
	// fonction recursive pour le parour de l'arbre de recherche
	travel_and_check(root, &best_cost, best_path);
	// Dernier ordenancement de le solution optimale
	reorder_paths(best_path, nbr_cities);
	
	printf("Terminaison de la résolution avec succès \n");
	
	printf("Le chemin optimal est: \n");
	
	for (int i = 0; i < nbr_cities; i++)
	{
		printf("%s -> ", cities_labes[best_path[i][0]]);
	}
	printf("%s \n",cities_labes[best_path[nbr_cities-1][1]]);
	printf("****** Fin de la résolution avec la méthode des regrets  ****** \n");
	return 0;
}
