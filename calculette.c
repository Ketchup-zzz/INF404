
#include <stdlib.h> 
#include <stdio.h> 

#include "type_ast.h"
#include "analyse_syntaxique.h"
#include "ast_parcours.h"



 
int main (int argc, char *argv[]) {
   int syntaxeCorrecte ;
   int modele;
   Ast A ;
   printf("saisir: 1 pour calcul un seul polynôme,2 Pour calculer plusieurs polynômes\n");
   scanf("%d",&modele);
   switch (argc) {
   	case 1: 
   		syntaxeCorrecte = analyser("", &A,modele);
		break ;
   	case 2: 
   		syntaxeCorrecte = analyser(argv[1], &A,modele);
		break ;
	default:
		printf("nombre d'arguments incorrects !\n");
		exit(1) ;
   } ;	
   if (syntaxeCorrecte&&modele==1) {
	//printf("%d",evaluation(A));
	printf(" \n RESULTAT = %.2f\n", evaluation(A));
   } ;
}
		
	
		
		
		
		

