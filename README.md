# Problème de voyageurs de commerce avec la methode des regrets

Ce projet consiste à résoudre le problème de voyageur de commerce grâce à la méthode de regret. L’algorithme repose sur le parcours d’un arbre de recherche binaire. 

Le programme en c se repose sur la récursivité. Il s'agit d'une implémentation générique avec la lecture facile du problème à partir d’un fichier csv fourni par l’utilisateur (on a aussi testé avec un fichier exemple.csv). La solution optimale est affichée sur le terminal comme une chaîne fermée des villes parcourues.
	
	
Le projet est construit à l'aide d'un Makefile. Pour compiler le programme:

	make all
	
Pour exécuter le programme après compilation:

	./regret exemple.csv
	
avec le fichier "exemple.csv" fourni dans le répertoire du code et contenant la matrice initiale de coût. 
	
Pour tout nettoyer :

	make clean
