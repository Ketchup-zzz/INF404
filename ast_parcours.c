#include <stdlib.h>
#include <stdio.h>
#include "type_ast.h"
#include <math.h>

void aff_operateur(TypeOperateur op){
	switch (op) {
		case N_PLUS:
			printf(" + ") ;
			break;
		case N_MOINS:
			printf(" - ") ;
			break;
		case N_MUL:
			printf(" * ") ;
			break;
		case N_DIV:
			printf(" / ") ;
			break;
		case N_MOD:
			printf(" mod ") ;
			break;
		case N_POW:
			printf(" ^ ") ;
			break;
		case N_SIN:
			printf(" sin ") ;
			break;

	} 
}

void afficherA(Ast expr) {
	// if(expr==NULL)
	// {
	// 	printf("NULL\n");
	// }
	switch (expr->nature) {
               case OPERATION:
			            if (expr->gauche != NULL) {
                        printf("(");
                        afficherA(expr->gauche);
                        aff_operateur(expr->operateur) ;
                        afficherA(expr->droite);
                        printf(")");
						}
						else{
						printf("(");
						aff_operateur(expr->operateur) ;
                        afficherA(expr->droite);
                        printf(")");
						}
			break ;
               case VALEUR:
                  	printf("%.2f", expr->valeur);
			break ;
	}
}




double evaluation(Ast expr) {
    double denominateur ;
	switch(expr->nature)
	{
	  case OPERATION:
		switch (expr->operateur)
		{

			case N_MUL:
			  return evaluation(expr->gauche)*evaluation(expr->droite);
	
			case N_MOINS:
              if (expr->gauche == NULL) {
                return -evaluation(expr->droite);
              } else {
                return evaluation(expr->gauche) - evaluation(expr->droite); 
              }

			case N_PLUS:
			  return evaluation(expr->gauche)+evaluation(expr->droite);

			case N_DIV:
			  denominateur = evaluation(expr->droite) ;
			  if (denominateur != 0)
			      return evaluation(expr->gauche)/evaluation(expr->droite);
                          else 
			      printf("ERREUR : division par 0 !\n");
			      exit(1);

			case N_MOD:
			  if((evaluation(expr->gauche)==floor(evaluation(expr->gauche)))&&(evaluation(expr->droite)==evaluation(expr->droite)))
			  return (int)evaluation(expr->gauche)%(int)evaluation(expr->droite);
			  else{
				  printf("ERREUR : mod avec décimal !\n");
			      exit(1);
			  }

			case N_POW:
			// printf("gauche=%lf",evaluation(expr->gauche));
			// printf("droit=%lf",evaluation(expr->droite));
			// printf("pow=%lf",pow(evaluation(expr->gauche), evaluation(expr->droite)));
			  return pow(evaluation(expr->gauche), evaluation(expr->droite));
			
			default:
			  exit(1);  // erreur operateur
		}

	   case VALEUR:
	   //printf("eval=%lf",expr->valeur);
		return expr->valeur; 
		
	   default:
		exit(2); // erreur Ast 
	}

}