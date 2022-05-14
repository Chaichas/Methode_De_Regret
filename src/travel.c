#include "../include/travel.h"

/* Cette fonction reduit la matrice de cout
 * cost_matrix (IN/OUT)	: matrice de cout a reduire
 * nbr_cities (IN) 		: nbr des villes
 * return: somme des minima des lignes et des colonnes de la matrice des couts
 */
int reduce_cost_matrix(int **cost_matrix, const int nbr_cities)
{
	int sum_min = 0;
	// get min of each row and substact it
	for (int i = 0; i < nbr_cities; i++)
	{
		int min = INT_MAX;
		bool min_set = false;
		for (int j = 0; j < nbr_cities; j++)
		{
			if ((cost_matrix[i][j] < min) && (cost_matrix[i][j] > -1))
			{
				min = cost_matrix[i][j];
				min_set = true;
			}
		}
		if (min_set)
		{
    		sum_min += min;
    		for (int j = 0; j < nbr_cities; j++)
    		{
    			if (cost_matrix[i][j] > -1)
    				cost_matrix[i][j] -= min;
    		}
		}
	}
	// get min of each column and substact it
	for (int j = 0; j < nbr_cities; j++)
	{
		int min = INT_MAX;
		bool min_set = false;
		for (int i = 0; i < nbr_cities; i++)
		{
			if ((cost_matrix[i][j] < min) && (cost_matrix[i][j] > -1))
			{
				min = cost_matrix[i][j];
				min_set = true;
			}
		}
		if (min_set)
		{
    		sum_min += min;
    		for (int i = 0; i < nbr_cities; i++)
    		{
    			if (cost_matrix[i][j] > -1)
    				cost_matrix[i][j] -= min;
    		}
		}
	}
	return sum_min;
}

/* Cette fonction calule de regret d'une case de la matrice de cout à zero
 * cost_matrix (IN)	: matrice de cout
 * nbr_cities (IN) 	: nbr des villes
 * row_idx, column_idx (IN)	: position de la case
 * return: regret
 */
int calc_regret(int **cost_matrix, const int nbr_cities, const int row_idx, const int column_idx)
{
	int min_row = INT_MAX;
	bool min_row_set = false;
	int min_column = INT_MAX;
	bool min_column_set = false;
	// get min of the column
	for (int i = 0; i < nbr_cities; i++)
	{
		if ((i != row_idx) && (cost_matrix[i][column_idx] > -1) && (cost_matrix[i][column_idx] < min_column))
		{
			min_column = cost_matrix[i][column_idx];
			min_column_set = true;
		}
	}
	// get min of the row
	for (int j = 0; j < nbr_cities; j++)
	{
		if ((j != column_idx) && (cost_matrix[row_idx][j] > -1) && (cost_matrix[row_idx][j] < min_row))
		{
	        min_row = cost_matrix[row_idx][j];
	        min_row_set = true;
		}
	}
	if (!min_row_set)
	    min_row = 0;
	if (!min_column_set)
	    min_column = 0;
	return min_row + min_column;
}

/* Cette fonction calule matrice de regret pour toutes les case à zero. Elle appelle la fonction calc_regret
 * cost_matrix (IN)	: matrice de cout
 * nbr_cities (IN) 	: nbr des villes
 * regret_matrix (OUT) : matrice de regret
 */
void calc_regret_matrix(int **regret_matrix, int **cost_matrix, const int nbr_cities)
{
	for (int i = 0; i < nbr_cities; i++)
	{
		for (int j = 0; j < nbr_cities; j++)
		{
			if (cost_matrix[i][j] == 0)
			{
				regret_matrix[i][j] = calc_regret(cost_matrix, nbr_cities, i, j);
			}
			else
			{
				regret_matrix[i][j] = -1;
			}
		}
	}
}

/* Cette fonction choisit le target du regret maximal. Si plusieur taget possible, le premier est choisit
 * Le regret du trajet choisi est mis sur -1 pour l'exclure dans la suite de la recherche
 * nbr_cities (IN) 	: nbr des villes
 * regret_matrix (IN/OUT): matrice de regret
 * selected_path (OUT)	: traget choisi sous la forme [[indice de depart],[indice d'arrivee]]
 * return: regret max
 */
int select_path(int **regret_matrix, const int nbr_cities, int *selected_path)
{
	int max_regret = -1;
	for (int i = 0; i < nbr_cities; i++)
	{
		for (int j = 0; j < nbr_cities; j++)
		{
			if (regret_matrix[i][j] > max_regret)
			{
				max_regret = regret_matrix[i][j];
				selected_path[0] = i;
				selected_path[1] = j;
			}
		}
	}
	regret_matrix[selected_path[0]][selected_path[1]] = -1;
	return max_regret;
}

/* Fonction recursive pour le parcour de l'arbre de recherche (voir figure 5)
 * nbr_cities (IN/OUT) 	: noeud
 * best_cost (IN/OUT)	: meilleur cout
 * best_path (IN/OUT)	:meilleur chemin
 */
void travel_and_check(Node *node, int *best_cost, int **best_path)
{
	if ((node->type == INCLUSIVE) || (node->type == ROOT))
	{
		// reduce matrix
		node->weight += reduce_cost_matrix(node->cost_matrix, node->nbr_cities);
		if (node->weight >= *best_cost)  // Il y a deja une solution trouvee
		{
			if (node->mother->type == EXCLUSIVE)
			{
				if (node->mother->mother->type == ROOT)
					return; // Retour au noeud racine --> fin de la recherche
				travel_and_check(node->mother->mother->mother->exclusive_daughter, best_cost, best_path);
			}
			else if (node->mother->type == INCLUSIVE)
			{
				travel_and_check(node->mother->mother->exclusive_daughter, best_cost, best_path);
			}
		}
		else
		{
			if (node->nbr_cities == 1)
			{
				for (int i = 0; i < node->nbr_paths; i++)
				{
					best_path[i][0] = node->current_paths[i][0];
					best_path[i][1] = node->current_paths[i][1];
				}
				
				best_path[node->nbr_paths][0] = node->row_cities[0];
				best_path[node->nbr_paths][1] = node->column_cities[0];
				
				*best_cost = node->weight;
				
				travel_and_check(node->mother->exclusive_daughter, best_cost, best_path);
			}
			else
			{
				// compute regret matrix
				calc_regret_matrix(node->regret_matrix, node->cost_matrix, node->nbr_cities);
				// select path
				int selected_path[2];
				int max_regret = select_path(node->regret_matrix, node->nbr_cities, selected_path);
				// Creation des filles inclusive et exclusive
				node->inclusive_daughter = add_node(node, INCLUSIVE, selected_path, 0);
				node->exclusive_daughter = add_node(node, EXCLUSIVE, selected_path, max_regret);
			
				travel_and_check(node->inclusive_daughter, best_cost, best_path);
			}
		}
	}
	if (node->type == EXCLUSIVE)
	{	
		if (node->weight >= *best_cost)
		{
			if (node->mother->type == ROOT)
				return; // Retour au noeud racine --> fin de la recherche
			// On continue a verifier les noeuds EXLUSIVE
			travel_and_check(node->mother->mother->exclusive_daughter, best_cost, best_path);
		}
		else
		{	// C'est une branche qui peut presenter une solution meilleure que la solution trouvee
			int selected_path[2];
			int max_regret = select_path(node->regret_matrix, node->nbr_cities, selected_path);
			// Creation des filles inclusive et exclusive
			node->inclusive_daughter = add_node(node, INCLUSIVE, selected_path, 0);
			node->exclusive_daughter = add_node(node, EXCLUSIVE, selected_path, max_regret);
			
			travel_and_check(node->inclusive_daughter, best_cost, best_path);
		}
	}
}
