
#ifndef __TYPE_AST__
#define __TYPE_AST__ 

typedef enum {OPERATION, VALEUR} TypeAst ;
typedef enum {N_PLUS, N_MUL, N_MOINS , N_DIV, N_MOD, N_POW,N_SIN,N_COS,N_TAN,N_DEG,N_ABS} TypeOperateur ;

typedef struct noeud { 
	TypeAst nature ;
	TypeOperateur operateur ;
	struct noeud *gauche, *droite ;
      	double valeur ;
} NoeudAst ;

typedef NoeudAst *Ast ;

#endif  
