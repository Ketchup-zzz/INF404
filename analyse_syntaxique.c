/* ------------------------------------------------------------------------
-- module analyse_syntaxique
--
-- analyse lexicale d'une expression arithmetique
--
-- J-P. Peyrin, G. Serasset (version initiale) : janvier 1999
-- P.Habraken - 18/01/2010
--
-- 10 juin 2006 - PH :
-- remplacement record a champs variants par record "plat"
-- remplacement traitement iteratif par automate
--
-- Aout 2016 - LM : version C
------------------------------------------------------------------------ */

#include <string.h>
#include <stdlib.h> 
#include <stdio.h> 

#include "type_ast.h"
#include "ast_construction.h"
#include "ast_parcours.h"
#include "analyse_lexicale.h"
#include "table_symboles.h"

	void rec_eag(Ast *A1);
	void seq_terme(Ast *A2);
 	void suite_seq_terme(Ast A1 , Ast *A2);
	void terme(Ast *A1);
	void seq_facteur(Ast *A2);
	void suite_seq_facteur(Ast A1 , Ast *A2);
	void facteur(Ast *A1);
	int op1(TypeOperateur *Op);
	int op2(TypeOperateur *Op);
	int op3(TypeOperateur *Op);
	TypeOperateur Operateur(Nature_Lexeme nature);
	extern int estPresentTS(char *idf, int *v) ;
	extern void insererTS(char *idf, int v) ;
	// void rec_seq_aff(Ast *A1);
	// void rec_aff(Ast *A1);
	void rec_seq_aff();
	void rec_aff();
 
   
 	 void rec_eag(Ast *A1)
 	 {
 		seq_terme(A1);
 	 }
 	 
 	 void seq_terme(Ast *A2)
 	 {
		Ast A1;
 		terme(&A1);
 		suite_seq_terme(A1,A2);
 	 }

 	void suite_seq_terme(Ast A1 , Ast *A2)
	{
		Ast A3,A4;
		TypeOperateur op;
			
		if(op1(&op))
		{
		terme(&A3);
		A4 = creer_operation(op,A1,A3);
		suite_seq_terme(A4,A2);		
		}	
		else
		{
			*A2=A1;		
		}
	}
	
	void terme(Ast *A1)
	{
		seq_facteur(A1);
	}
	
	void seq_facteur(Ast *A2)
	{
		Ast A1;
		facteur(&A1);
		suite_seq_facteur(A1,A2);
	}
	
	void suite_seq_facteur(Ast A1 , Ast *A2)
	{
		Ast A3,A4;
		TypeOperateur op;
		int r = op2(&op);
		if(r!=0)
		{
		facteur(&A3);
		A4 = creer_operation(op,A1,A3);
		suite_seq_facteur(A4,A2);
		}
		else
		{
			*A2=A1;		
		}
		
		
	}
	
	void facteur(Ast *A1)
	{
		// printf("facteur, current lexeme: %d\n", lexeme_courant().nature);
		int v;
		int trouve=0;
		switch(lexeme_courant().nature){
			case ENTIER:
				*A1 = creer_valeur(lexeme_courant().valeur);
				avancer();
				break;
			case PARO:
				avancer();
				rec_eag(A1);
				if(lexeme_courant().nature == PARF)
		  		{
		  			avancer();
		  		}
		  		else
		  		{
		  			printf("ERREUR : parenthese fermante attendue (ligne %u, colonne %u)\n",
                                                    lexeme_courant().ligne, lexeme_courant().colonne);
		  			exit(1);
		  		}
		  		break;
			case IDF:
			    trouve=estPresentTS(lexeme_courant().chaine,&v);
				if(trouve){
					//*A1 = creer_valeur(lexeme_courant().valeur);
					*A1 = creer_valeur(v);
				}else{
					printf("erreur\n");
				}
			    avancer();
				break;
			case MOINS:
			    avancer();
				Ast A2;
				// Ast A0;
		        //A0=creer_valeur(0);
				facteur(&A2);
				*A1=creer_operation(MOINS,NULL,A2);
			    break;
			case SIN:
			    avancer();
                if(lexeme_courant().nature == PARO) {
                    avancer();
                    rec_eag(A1);
                if(lexeme_courant().nature == PARF) {
                    avancer();
                    *A1 = creer_fonction(N_SIN, *A1);
                } else {
                    printf("ERREUR : parenthese fermante attendue (ligne %u, colonne %u)\n",lexeme_courant().ligne, lexeme_courant().colonne);
                    exit(1);
                }
                } else {
                    printf("Erreur : parenthèse ouvrante attendue après le SIN(ligne %u, colonne %u)\n",lexeme_courant().ligne, lexeme_courant().colonne);
                    exit(1);
                }
                break;
			case COS:
			    avancer();
                if(lexeme_courant().nature == PARO) {
                    avancer();
                    rec_eag(A1);
                if(lexeme_courant().nature == PARF) {
                    avancer();
                    *A1 = creer_fonction(N_COS, *A1);
                } else {
                    printf("ERREUR : parenthese fermante attendue (ligne %u, colonne %u)\n",lexeme_courant().ligne, lexeme_courant().colonne);
                    exit(1);
                }
                } else {
                    printf("Erreur : parenthèse ouvrante attendue après le COS(ligne %u, colonne %u)\n",lexeme_courant().ligne, lexeme_courant().colonne);
                    exit(1);
                }
                break;
			case TAN:
			    avancer();
                if(lexeme_courant().nature == PARO) {
                    avancer();
                    rec_eag(A1);
                if(lexeme_courant().nature == PARF) {
                    avancer();
                    *A1 = creer_fonction(N_TAN, *A1);
                } else {
                    printf("ERREUR : parenthese fermante attendue (ligne %u, colonne %u)\n",lexeme_courant().ligne, lexeme_courant().colonne);
                    exit(1);
                }
                } else {
                    printf("Erreur : parenthèse ouvrante attendue après le TAN(ligne %u, colonne %u)\n",lexeme_courant().ligne, lexeme_courant().colonne);
                    exit(1);
                }
                break;
		  	default:
		  			printf("ERREUR : entier ou parenthese ouvrante attendu (ligne %u, colonne %u)\n",
                            lexeme_courant().ligne, lexeme_courant().colonne);
		  			exit(0);

		
		}
		TypeOperateur op;			
		while (op3(&op)) {
			avancer(); // on avance pour passer le POW
			Ast base = *A1; // on sauvegarde AST pour la base
			Ast exposant;
			facteur(&exposant); // on récupère l'exposant
			*A1 = creer_operation(N_POW, base, exposant); // on crée l'AST pour la puissance
		}
	}
	
	int op1(TypeOperateur *Op)
	{
		switch(lexeme_courant().nature){
			case PLUS:
			case MOINS:
				*Op = Operateur(lexeme_courant().nature);
				avancer(); 
				return 1;
			default:
				return 0;
		}
	}
	
	int op2(TypeOperateur *Op)
	{ 
		switch(lexeme_courant().nature){
			case MUL: 
			case DIV:
			case MOD: 
				*Op = Operateur(lexeme_courant().nature);
				avancer();
				return 1;
			default: 
				return 0;
		}
	}

	int op3(TypeOperateur *Op)
	{
		if(lexeme_courant().nature == POW)
		{
			*Op = N_POW;
			return 1;
		}
		else
		{
			return 0;
		}
	}
		 
 	 	TypeOperateur Operateur(Nature_Lexeme nature)
{
		switch(nature)
		{
			case PLUS : return N_PLUS;
			case MOINS : return N_MOINS;
			case MUL: return N_MUL;
			case DIV : return N_DIV;
			case MOD : return N_MOD;
			case POW : return N_POW;
			default: printf("Erreur operateur"); exit(1) ; 
		}
}


void rec_seq_aff() {
    if (lexeme_courant().nature == IDF) {
        rec_aff();
        rec_seq_aff();
    }
    // epsilon transition, do nothing for it
}

void rec_aff() {
	// printf("OK1\n");
    Ast A;
    int v;
    if (lexeme_courant().nature == IDF) {
        char idf[256];  // Assuming the identifier won't exceed 255 characters
        strcpy(idf, lexeme_courant().chaine);
        avancer();
        if (lexeme_courant().nature == AFF) {
            avancer();
            rec_eag(&A);
            v = evaluation(A);
            insererTS(idf, v);
			// printf("OK2\n");
        } else {
            printf("ERREUR : opérateur d'affectation attendu\n");
            exit(1);
        }
    } else {
        printf("ERREUR : identificateur attendu\n");
        exit(1);
    }
    if (lexeme_courant().nature != SEPINST) {
        printf("ERREUR : séparateur de fin d'instruction attendu\n");
        exit(1);
    }
    avancer();
}
/* ----------------------------------------------------------------------- */
int analyser(char* nomFichier, Ast *A)
{
	demarrer(nomFichier) ;
	//rec_seq_aff();
   	rec_eag(A);
		
	if ((lexeme_courant().nature == FIN_SEQUENCE)) {
		/* syntaxe correcte */
		printf("\nSyntaxe correcte\n"); 
		afficherA (*A);  // on affiche l'AST
		afficheTS();
		printf("\n");
   		return 1 ;	
   	} else {
		printf("Erreur lors de l'analyse syntaxique ...\n");
		 printf("Lexeme inattendu: %d\n",lexeme_courant().nature);
		return 0 ;
        } ;
}	 
 	 
