#include <stdio.h>
#include <limits.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

// definition des type des noeuds
#define ROOT 0
#define INCLUSIVE 1
#define EXCLUSIVE 2

typedef struct Node
{
	int type; // ROOT, INCLUSIVE ou EXCLUSIVE
	struct Node *mother; // parent du noeud
	struct Node *inclusive_daughter; // fille INCLUSIVE
	struct Node *exclusive_daughter; // fille EXCLUSIVE
	int nbr_cities; // nbr des villes qui reste à visiter
	int *row_cities; // tableaux des indices des villes de depart
	int *column_cities; // tableaux des indices des villes d'arrivee
	int **cost_matrix; // matrice des couts
	int **regret_matrix; // matrice des regrets
	int weight; // poids du noeud
	int nbr_paths; // nbr des trahets inclus
	int **current_paths; // tableaux des trajets inclus
} Node;

Node* add_node(Node *mother, int type, const int selected_path[2], int regret);

Node* initial_node(int **cost_matrix, const int nbr_cities);

void reorder_paths(int **current_paths, const int nbr_paths);