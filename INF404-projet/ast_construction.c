#include <stdlib.h>
#include <stdio.h> 

#include "type_ast.h"

Ast creer_operation(TypeOperateur opr , Ast opde_gauche , Ast opde_droit) 
{
      Ast expr ;
      expr = (Ast) malloc(sizeof(NoeudAst));
      expr->nature = OPERATION;
      expr->operateur = opr;
      if (opde_gauche == NULL && opde_droit == NULL) {
         printf("ERREUR_EXPRESSION\n") ;
	 exit(1) ;
      } else {
         expr->gauche = opde_gauche;
         expr->droite = opde_droit;
      } ;
      return expr ;
}

Ast creer_valeur(double val) { 
      Ast expr ;
      expr = (Ast) malloc (sizeof(NoeudAst));
      expr->nature = VALEUR;
      expr->valeur = val;
      return expr ;
}

//gauche est valeur,droit est NULL
Ast creer_fonction(TypeOperateur opr , Ast x) 
{
      Ast expr ;
      expr = (Ast) malloc(sizeof(NoeudAst));
      expr->nature = OPERATION;
      expr->operateur = opr;
      if (x == NULL) {
         printf("ERREUR_EXPRESSION\n") ;
	 exit(1) ;
      } else {
         expr->gauche = x;
         expr->droite = NULL;
      } ;
      return expr ;
}



/*int lengthA(Ast *A)
{

		int ng = 0 , nd = 0;
			if(A->nature==NULL)
				return 0;
			ng = 1+ length(A->gauche);
			nd = 1 + length(A->droite);
			if(ng>nd)
				return ng;
			else
				return nd;
		}*/
