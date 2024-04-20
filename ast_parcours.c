#include <stdlib.h>
#include <stdio.h>
#include "type_ast.h"
#include <math.h>
#define PI 3.14159265358979323846
#define EPSILON 1e-12
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
		case N_COS:
			printf(" cos ") ;
			break;
		case N_TAN:
			printf(" tan ") ;
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
			            if (expr->gauche == NULL&&expr->operateur==N_MOINS) {
							printf("(");
						    aff_operateur(expr->operateur) ;
                            afficherA(expr->droite);
                            printf(")");
						}//case normal (a+-*/b)
						else if(expr->droite!=NULL&&expr->droite!=NULL){
                            printf("(");
                            afficherA(expr->gauche);
                            aff_operateur(expr->operateur) ;
                        	afficherA(expr->droite);
                        	printf(")");
						}//case fonction
						else{
							printf("(");
						    aff_operateur(expr->operateur) ;
                            afficherA(expr->gauche);
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
	double result, normalized_angle, fraction_part;
    int k;
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
			
			case N_SIN:
                return sin(evaluation(expr->gauche)/180*PI);

            case N_COS:
                return cos(evaluation(expr->gauche)/180*PI);

            case N_TAN:
			    result = evaluation(expr->gauche)/180*PI;
                    // normalisé à [0, 2π)
                    normalized_angle = fmod(result, 2 * PI);
                    if (normalized_angle < 0) {
                        normalized_angle += 2 * PI;  // non negatif
                    }
                    // tester(π/2) + kπ
                    fraction_part = fabs((normalized_angle - PI/2) / PI);
                    k = round(fraction_part);  // entier le plus proche
                    if (fabs(fraction_part - k) < EPSILON) {
                        printf("Erreur : Le calcul de la tangente à un angle spécifique (π/2) + kπ n'est pas autorisé\n");
                        exit(1);
                    }
                    return tan(result);
                // return tan(evaluation(expr->gauche)/180*PI); 

			  
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