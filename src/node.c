#include "../include/node.h"

/* Cette fonction permet de reordonner les trajets dans un chemin
 * current_paths (IN/OUT) 	: tableau contenant les trajets du chemin
 * nbr_paths (IN)			: nombre des trajets
 */
void reorder_paths(int **current_paths, const int nbr_paths)
{
	int ordered_paths[nbr_paths][2];
	int already_ordered = 0;
	bool ordered[nbr_paths];
	for (int i = 0; i < nbr_paths; i++)
	{
		ordered[i] = false;
	}
	int path_idx;
	bool good_choice;
	for (int i = 0; i < nbr_paths; i++)
	{
		good_choice = true;
		for(int j = 0; j < nbr_paths; j++)
		{
			if (current_paths[i][0] == current_paths[j][1])
			{
				good_choice = false;
				break;
			}
		}
		if (good_choice)
		{
			path_idx = i;
			break;
		}
	}
	int last_city;
	while (true)
	{
		ordered_paths[already_ordered][0] = current_paths[path_idx][0];
		ordered_paths[already_ordered][1] = current_paths[path_idx][1];
		ordered[path_idx] = true;
		last_city = current_paths[path_idx][1];
		already_ordered++;
		if (already_ordered == nbr_paths)
			break;
		for (int i = 0; i < nbr_paths; i++)
		{
			if (!ordered[i])
				path_idx = i;
			if (current_paths[i][0] == last_city)
			{
				path_idx = i;
				break;
			}
		}
	}
	for (int i = 0; i < nbr_paths; i++)
	{
		current_paths[i][0] = ordered_paths[i][0];
		current_paths[i][1] = ordered_paths[i][1];
	}
}

/* Cette fonction permet d'avoir la position d'un element dans un tableau
 * tab (IN) 	: tableau
 * length (IN)	: longueur du tableau
 * elmt (IN)	: element
 * return: position de l'element
 */
int get_index(const int *tab, const int length, const int elmt)
{
	int found;
	for (int i = 0; i < length; i++)
	{
		if (tab[i] == elmt)
		{
			found = i;
			break;
		}
	}
	return found;
}

/* Cette fonction permet l'eliminet les trajets incompatible
 * cost_matrix (IN/OUT) : matrice de cout
 * row_cities (IN)		: indices des villes de depart
 * column_cities (IN)	: indices des villes d'arrivee
 * nbr_cities (IN)		: nbr des villes dans la matrice
 * current_paths (IN)	: tableau des trajet inclus
 * nbr_paths (IN)		: nbr de trajets inclus
 */
void incompatiple_paths(int **cost_matrix, const int *row_cities, const int *column_cities, const int nbr_cities, int **current_paths, const int nbr_paths)
{
	int row_idx, column_idx;
	column_idx = get_index(column_cities, nbr_cities, current_paths[0][0]);
	for (int i = 1; i < nbr_paths; i++)
	{
		if (current_paths[i-1][1] == current_paths[i][0])
			continue;
		row_idx = get_index(row_cities, nbr_cities, current_paths[i-1][1]);
		cost_matrix[row_idx][column_idx] = -1;
		column_idx = get_index(column_cities, nbr_cities, current_paths[i][0]);
	}
	row_idx = get_index(row_cities, nbr_cities, current_paths[nbr_paths-1][1]);
	cost_matrix[row_idx][column_idx] = -1;
}

/* Cette fonction initialise le noeud racine
 * cost_matrix (IN) : matrice de cout
 * nbr_cities (IN)	: nbr des villes
 * return: noeud racine
 */
Node* initial_node(int **cost_matrix, const int nbr_cities)
{
	Node* p = (Node*)malloc(sizeof(Node));
	p->type = ROOT;
	p->nbr_cities = nbr_cities;
	p->row_cities = (int*)malloc(nbr_cities * sizeof(int));
	p->column_cities = (int*)malloc(nbr_cities * sizeof(int));
	// Allocate memory to cost_matrix and regret_matrixand and fill initial cost_matrix
	p->cost_matrix = (int**)malloc(nbr_cities * sizeof(int*));
	p->regret_matrix = (int**)malloc(p->nbr_cities * sizeof(int*));
	for (int i=0; i < nbr_cities; i++)
	{
		p->row_cities[i] = i;
		p->column_cities[i] = i;
		p->cost_matrix[i] = (int*)malloc(nbr_cities * sizeof(int));
		p->regret_matrix[i] = (int*)malloc(nbr_cities * sizeof(int));
		for (int j = 0; j < nbr_cities; j++)
		{
			p->cost_matrix[i][j] = cost_matrix[i][j];
		}
	}
	p->weight = 0;
	p->nbr_paths = 0;
	return p;
}

/* Cette fonction initialise un noeud en fonction de son type
 * mother (IN) 			: noeud mere
 * selected_path (IN)	: trajet selectionner dans l'etape precedante pour le noeud type INCLUSIVE
 * regret (IN)			: regret de l'etape precedante pour le calcul du poid du noeud type EXCLUSIVE
 * return: noeud
 */
Node* add_node(Node *mother, int type, const int selected_path[2], int regret)
{
	Node* p = (Node*)malloc(sizeof(Node));
	p->type = type;
	p->mother = mother;
	if (type == INCLUSIVE)
	{
		p->nbr_paths = mother->nbr_paths + 1;
		p->nbr_cities = mother->nbr_cities - 1;
	}
	else if (type == EXCLUSIVE)
	{
		p->nbr_paths = mother->nbr_paths;
		p->nbr_cities = mother->nbr_cities;
	}
	p->row_cities = (int*)malloc(p->nbr_cities * sizeof(int));
	p->column_cities = (int*)malloc(p->nbr_cities * sizeof(int));
	// Allocate memory to cost_matrix and regret_matrix
	p->cost_matrix = (int**)malloc(p->nbr_cities * sizeof(int*));
	p->regret_matrix = (int**)malloc(p->nbr_cities * sizeof(int*));
	for (int i=0; i < p->nbr_cities; i++)
	{
		p->cost_matrix[i] = (int*)malloc(p->nbr_cities * sizeof(int));
		p->regret_matrix[i] = (int*)malloc(p->nbr_cities * sizeof(int));
	}
	// Allocate
	p->current_paths = (int**)malloc(p->nbr_paths * sizeof(int*));
	for (int i = 0; i < p->nbr_paths; i++)
	{
		p->current_paths[i] = (int*)malloc(2 * sizeof(int));
	}
	for (int i = 0; i < mother->nbr_paths; i++)
	{
		p->current_paths[i][0] = mother->current_paths[i][0];
		p->current_paths[i][1] = mother->current_paths[i][1];
	}
	// Filling the cost matrix depend on its type
	if (type == INCLUSIVE)
	{
		int row_idx = 0;
		int column_idx = 0;
		for (int i = 0; i < mother->nbr_cities; i++)
		{
			if (i != selected_path[0])
			{
				p->row_cities[row_idx] = mother->row_cities[i];
				row_idx++;
			}
			if (i != selected_path[1])
			{
				p->column_cities[column_idx] = mother->column_cities[i];
				column_idx++;
			}
		}
		// Fill cost_matrix
		row_idx = 0;
		for (int i = 0; i < mother->nbr_cities; i++)
		{
			if (i != selected_path[0])
			{
				column_idx = 0;
				for (int j = 0; j < mother->nbr_cities; j++)
				{
					if (j != selected_path[1])
					{
						p->cost_matrix[row_idx][column_idx] = mother->cost_matrix[i][j];
						column_idx++;
					}
				}
				row_idx++;
			}
		}
		p->current_paths[p->nbr_paths-1][0] = mother->row_cities[selected_path[0]];
		p->current_paths[p->nbr_paths-1][1] = mother->column_cities[selected_path[1]];
		reorder_paths(p->current_paths, p->nbr_paths);
		incompatiple_paths(p->cost_matrix, p->row_cities, p->column_cities, p->nbr_cities, p->current_paths, p->nbr_paths);
	}
	else if (type == EXCLUSIVE)
	{
		// Fill cost_matrix and regret_matrix
		for (int i = 0; i < mother->nbr_cities; i++)
		{
			for (int j = 0; j < mother->nbr_cities; j++)
			{
				p->cost_matrix[i][j] = mother->cost_matrix[i][j];
				p->regret_matrix[i][j] = mother->regret_matrix[i][j];
			}
		}
	}
	p->weight = mother->weight + regret;
	return p;
}
