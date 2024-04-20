/* ------------------------------------------------------------------------
-- paquetage analyse_lexicale
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

#include "lecture_caracteres.h"
#include "analyse_lexicale.h"

#define NB_MOTCLE 2
char motCle[2][20] = {"sin", "cos"};
   /* --------------------------------------------------------------------- */


/* Les variables et fonctions locales au module */

   Lexeme lexeme_en_cours ;	/* le lexeme courant */

   void ajouter_caractere (char *s, char c);
   Nature_Caractere nature_caractere (char c);
   int est_separateur(char c ) ;
   int est_chiffre(char c ) ;
   int est_symbole(char c ) ;
   int est_car(char c);
   void reconnaitre_lexeme();
   int estnegatif=0;
   int boocle=1;

   /* --------------------------------------------------------------------- */

   void demarrer(char *nom_fichier) {
      demarrer_car(nom_fichier);
      avancer();
   }

   /* --------------------------------------------------------------------- */

   void avancer() {
      reconnaitre_lexeme();
   } 

   /* --------------------------------------------------------------------- */

   Lexeme lexeme_courant() {
      return lexeme_en_cours;
   } 

   /* --------------------------------------------------------------------- */

   int fin_de_sequence() {
      return lexeme_en_cours.nature == FIN_SEQUENCE;
   }

   /* --------------------------------------------------------------------- */

   void arreter() {
      arreter_car() ;
   }

   /* --------------------------------------------------------------------- */


   // reconnaissance d'un nouveau lexeme
   // etat initial : le caractere courant est soit separateur 
   //                soit le 1er caractere d'un lexeme
   // etat final : 
   //       - un nouveau lexeme est reconnu dans lexeme_en_cours
   //       - le caractere courant est soit la fin de fichier, 
   //		soit un separateur,  soit le 1er caractere d'un lexeme

   void reconnaitre_lexeme() {
      typedef enum {E_INIT, E_ENTIER,E_CAR,E_ENTIER_DECIMAL,E_FIN} Etat_Automate ;
      Etat_Automate etat=E_INIT;

     // on commence par lire et ignorer les separateurs
     while (est_separateur(caractere_courant())) {
        	avancer_car() ;
     } ;

     lexeme_en_cours.chaine[0] = '\0' ;

     // on utilise ensuite un automate pour reconnaitre et construire le prochain lexeme

    while (etat != E_FIN) {
		boocle++;

	    switch (etat) {

		  case E_INIT: // etat initial

			switch(nature_caractere(caractere_courant())) {

			case C_FIN_SEQUENCE: 
             		lexeme_en_cours.nature = FIN_SEQUENCE;
                	etat = E_FIN;
			break ;


			case CHIFFRE: 
		     	lexeme_en_cours.nature = ENTIER;
                	lexeme_en_cours.ligne = numero_ligne();
                	lexeme_en_cours.colonne = numero_colonne();
		     		ajouter_caractere (lexeme_en_cours.chaine, caractere_courant()) ;
					// printf("test=%d\n",estnegatif);
					lexeme_en_cours.valeur = caractere_courant() - '0';
					if(estnegatif==1&&boocle==1)
					{
                        lexeme_en_cours.valeur=lexeme_en_cours.valeur*(-1);
					}
               		etat = E_ENTIER;
		   	avancer_car() ;
			break ;

			case CARACTERE: 
		     	lexeme_en_cours.nature = IDF;
                	lexeme_en_cours.ligne = numero_ligne();
                	lexeme_en_cours.colonne = numero_colonne();
		     		ajouter_caractere (lexeme_en_cours.chaine, caractere_courant()) ;
					//lexeme_en_cours.nature=CAR;
               		//lexeme_en_cours.valeur = (int)caractere_courant();
               		etat = E_CAR;
		   	avancer_car() ;
			break ;

			case SYMBOLE: 
		       	lexeme_en_cours.ligne = numero_ligne();
               		lexeme_en_cours.colonne = numero_colonne();
		       	ajouter_caractere (lexeme_en_cours.chaine, caractere_courant()) ;
               		switch (caractere_courant()) {
               		  case '+':
               			lexeme_en_cours.nature = PLUS;
               			etat = E_FIN;
						estnegatif=0;
			   			break;
               		  case '-':
               			  lexeme_en_cours.nature=MOINS;
						  estnegatif=1;
						//   printf("test=%d\n",estnegatif);
						  etat=E_FIN;
						break;
               		  case '*':
               			lexeme_en_cours.nature = MUL;
               			etat = E_FIN;
						estnegatif=0;
			   			break;
               		  case '/':
               			lexeme_en_cours.nature = DIV;
               			etat = E_FIN;
						estnegatif=0;
			   			break;
					  case '%':
					    lexeme_en_cours.nature = MOD;
						etat=E_FIN;
						estnegatif=0;
						break;
					  case '^':
					    lexeme_en_cours.nature = POW;
						etat=E_FIN;
						estnegatif=0;
						break;
               		  case '(':
               			lexeme_en_cours.nature = PARO;
               			etat = E_FIN;
						estnegatif=0;
			   			break;
               		  case ')':
               			lexeme_en_cours.nature = PARF;
               			etat = E_FIN;
						estnegatif=0;
			   			break;
					  case '=':
					    lexeme_en_cours.nature = AFF;
						etat=E_FIN;
						estnegatif=0;
						break;
					  case '.':
                        etat = E_FIN;
                        break;
					//   case ':':
               		// 	lexeme_en_cours.nature = IDF;
               		// 	etat = E_FIN;
					  case ';':
               			lexeme_en_cours.nature = SEPINST;
               			etat = E_FIN;
						estnegatif=0;
			   			break; 
	       		  default:
				printf("Erreur_Lexicale (ligne %u, colonne %u)\n", lexeme_en_cours.ligne, lexeme_en_cours.colonne) ;
		 		exit(0) ;
		 	} ;
		   		avancer_car() ;
				break ;

			default:
			   printf("Erreur_Lexicale (ligne %u, colonne %u)\n", lexeme_en_cours.ligne, lexeme_en_cours.colonne) ;
		           exit(0) ;
		} ;
		break ;
		

		case E_CAR:  // reconnaissance d'un char
			switch(nature_caractere(caractere_courant())) {
			    case CARACTERE:
		  			ajouter_caractere (lexeme_en_cours.chaine, caractere_courant()) ;
                  	avancer_car ();
					etat=E_CAR;
					break ;

				default:
                  	etat = E_FIN;
          	} ;
			for (int i=0 ; i<NB_MOTCLE ; i++)
                 if (strcmp(lexeme_en_cours.chaine, motCle[i]) == 0) {
                     switch(i) {
                        case 0: lexeme_en_cours.nature = SIN; break ;
                        case 1: lexeme_en_cours.nature = COS; break ;
                        default:
					    lexeme_en_cours.nature=IDF; 
						break ;
                        }
                    }
		break;

		case E_ENTIER:  // reconnaissance d'un entier
			switch(nature_caractere(caractere_courant())) {
			    case CHIFFRE:
		  			ajouter_caractere (lexeme_en_cours.chaine, caractere_courant()) ;
					lexeme_en_cours.valeur = lexeme_en_cours.valeur * 10 + caractere_courant() - '0';
					// if(estnegatif==1&&boocle==1)
					// {
					// 	lexeme_en_cours.valeur*=-1;
					// }
					// lexeme_en_cours.valeur=atof(lexeme_en_cours.chaine);
                  	etat = E_ENTIER;
                  	avancer_car ();
					break;

			case SYMBOLE:
            if (caractere_courant() == '.') 
			{
                ajouter_caractere(lexeme_en_cours.chaine, caractere_courant());
                etat = E_ENTIER_DECIMAL; // changer etat en decimale
                avancer_car();
            } else {
                 etat = E_FIN;
            }
            break;

				default:
                  	etat = E_FIN;
          	} ;
        break;
	case E_ENTIER_DECIMAL: // handling the decimal part of a number
    switch (nature_caractere(caractere_courant())) {
        case CHIFFRE:
            ajouter_caractere(lexeme_en_cours.chaine, caractere_courant());
            lexeme_en_cours.valeur=atof(lexeme_en_cours.chaine);
			// if(estnegatif==1&&boocle==1)
			// {
			// 	lexeme_en_cours.valeur*=-1;
			// }
            avancer_car();
            break;
        default:
            etat = E_FIN;
    }
    break;
		
	    case E_FIN:  // etat final
		break ;
	  } ; // fin du switch(etat)
	//    printf("boocle=%d\n",boocle);
	} ; // fin du while (etat != fin)
   }

   /* --------------------------------------------------------------------- */

   // cette fonction ajoute le caractere c a la fin de la chaine s 
   // (la chaine s est donc modifiee)
 
   void ajouter_caractere (char *s, char c) {
	int l ;
	
	l = strlen(s) ;
	s[l] = c ;
	s[l+1] = '\0' ;
   } ;

   /* --------------------------------------------------------------------- */

   Nature_Caractere nature_caractere (char c) {
	   if (fin_de_sequence_car(c)) return C_FIN_SEQUENCE;
	   if (est_chiffre(c)) return CHIFFRE;
	   if (est_symbole(c)) return SYMBOLE;
	   if(est_car(c)) return CARACTERE;
	   return ERREUR_CAR ;
   }
   /* --------------------------------------------------------------------- */

   // vaut vrai ssi c designe un caractere separateur
   int est_separateur(char c) { 
      return c == ' ' || c == '\t' || c == '\n' ;
   }

   /* --------------------------------------------------------------------- */

   // vaut vrai ssi c designe un caractere chiffre
   int est_chiffre(char c) {
      return c >= '0' && c <= '9' ;
   }

   /* --------------------------------------------------------------------- */

   // vaut vrai ssi c designe un caractere correspondant a un symbole arithmetique
   int est_symbole(char c)  {
      switch (c) {
        case '+':  
	 	case '-':  
	 	case '*':
	 	case '/':
		case '%':
		case '^':
	 	case '(':
	 	case ')':
		case '=':
		case ':':
		case ';':
		case '.':
            return 1;

        default:
            return 0;
      } 
   }
    /* --------------------------------------------------------------------- */
   int est_car(char c) {
    if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')) 
	  {
        return 1;
    } else {
        return 0;
    }
   }

   /* --------------------------------------------------------------------- */

   // renvoie la chaine de caracteres correspondant a la nature du lexeme
   char *Nature_vers_Chaine (Nature_Lexeme nature) {
	switch (nature) {
		case ENTIER: return "ENTIER" ;
		//case CAR: return "CARACTERE";
		case PLUS: return "PLUS" ;
      	case MOINS: return "MOINS" ;            
      	case MUL: return "MUL" ;              
      	case DIV: return "DIV" ;
		case MOD: return "MOD" ;
		case POW: return "POW" ;              
      	case PARO: return "PARO" ;              
      	case PARF: return "PARF" ;
		case SIN: return "SIN" ;
		case COS: return "COS" ;              
      	case FIN_SEQUENCE: return "FIN_SEQUENCE" ;
		case AFF: return "AFF";
		case IDF: return "IDF";
		case SEPINST: return "SEPINST";     
      	default: return "ERREUR" ;            
	} ;
   } 

   /* --------------------------------------------------------------------- */

   // affiche a l'ecran le lexeme l
   void afficher(Lexeme l) {

      switch (l.nature) {
         case FIN_SEQUENCE: 
            break;
         default: 
            printf("(ligne %d, ", l.ligne);
            printf("colonne %d) : ",l.colonne);
	    printf("[") ;
            printf("nature = %s", Nature_vers_Chaine(l.nature)) ;
            printf(", chaine = %s, ", l.chaine) ;
            switch(l.nature) {
                 case ENTIER:
                      printf("valeur = %.2f", l.valeur);
                 default:
                      break;
            } ;
	    printf("]") ;
	} ;
   }

   /* --------------------------------------------------------------------- */

