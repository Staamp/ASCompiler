/**	PROJET Turtle
 *  Analyse syntaxique
 *
 * Poncot cédric
 * Courvoisier Nicolas
 * TPAB
 *
 * Annee 2018-2019
 *
 */


#include "turtle-ast.h"

#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>




/*
 * CONSTANTE
 */
#define PI    	3.14159265358979323846
#define SQRT2   1.41421356237309504880
#define SQRT3   1.7320508075688772935




/*
 *
 *	STRUCTURE POUR CREER LES NOEUDS
 *
 */


/*
 * Crée un noeud avec une valeur
 */
struct ast_node *make_expr_value(double value) {
	struct ast_node *node = calloc(1, sizeof(struct ast_node));
	node->kind = KIND_EXPR_VALUE;
	node->u.value = value;
	node->children_count = 0;
	return node;
}

/*
 * Crée un noeud avec un block
 */
struct ast_node *make_expr_block(struct ast_node *cmds){
	struct ast_node *node = calloc(1, sizeof(struct ast_node));
	node->kind = KIND_EXPR_BLOCK;
	node->children_count = 1;
	node->children[0] = cmds;
	return node;
}

/*
 * Crée un noeud pour réaliser un moins unaire
 */
struct ast_node *make_expr_moins(struct ast_node *expr){
	struct ast_node *node = calloc(1, sizeof(struct ast_node));
	node->kind = KIND_EXPR_MOINS;
	node->children_count = 1;
	node->children[0] = expr;
	return node;
}

/*
 * Crée un noeud avec nom
 */
struct ast_node *make_expr_name(const char *name) {
	struct ast_node *noeud = calloc(1, sizeof(struct ast_node));
	noeud->kind = KIND_EXPR_NAME;
	char *current = calloc(strlen(name)+1, sizeof(char));
	strcpy(current, name);
	noeud->u.name = current;
	noeud->children_count = 0;
	return noeud;
}

/*
 * Crée un noeud pour une répétition
 */
struct ast_node *make_cmd_repeat(struct ast_node *nb, struct ast_node *cmd){
	struct ast_node *node = calloc(1, sizeof(struct ast_node));
	node->kind = KIND_CMD_REPEAT;
	node->children_count = 2;
	node->children[0] = nb;
	node->children[1] = cmd;
	return node;
}

/*
 * Crée un noeud pour une procédure
 */
struct ast_node *make_cmd_proc(struct ast_node *name, struct ast_node *cmd){
	struct ast_node *node = calloc(1, sizeof(struct ast_node));
	node->kind = KIND_CMD_PROC;
	node->children_count = 2;
	node->children[0] = name;
	node->children[1] = cmd;
	return node;
}

/*
 * Crée un noeud pour un appel
 */
struct ast_node *make_cmd_call(struct ast_node *name){
	struct ast_node *node = calloc(1, sizeof(struct ast_node));
	node->kind = KIND_CMD_CALL;
	node->children_count = 1;
	node->children[0] = name;
	return node;
}

/*
 * Crée un noeud pour ajouter une variable
 */
struct ast_node *make_cmd_set(struct ast_node *name, struct ast_node * expr){
	struct ast_node *node = calloc(1, sizeof(struct ast_node));
	node->kind = KIND_CMD_SET;
	node->children_count = 2;
	node->children[0] = name;
	node->children[1] = expr;
	return node;
}

//COMMANDE

/*
 * Crée un noeud pour dessiner en forward
 */
struct ast_node *make_cmd_forward(struct ast_node *expr) {
	struct ast_node *node = calloc(1, sizeof(struct ast_node));
	node->kind = KIND_CMD_SIMPLE;
	node->u.cmd = CMD_FORWARD;
	node->children_count = 1;
	node->children[0] = expr;
	return node;
}

/*
 * Crée un noeud pour dessiner en backward
 */
struct ast_node *make_cmd_backward(struct ast_node *expr){
	struct ast_node *node = calloc(1, sizeof(struct ast_node));
	node->kind = KIND_CMD_SIMPLE;
	node->u.cmd = CMD_BACKWARD;
	node->children_count = 1;
	node->children[0] = expr;
	return node; 
}

/*
 * Crée un noeud pour lever le crayon
 */
struct ast_node *make_cmd_up(){
	struct ast_node *node = calloc(1, sizeof(struct ast_node));
	node->kind = KIND_CMD_SIMPLE;
	node->u.cmd = CMD_UP;
	node->children_count = 0;
	return node;
}

/*
 * Crée un noeud pour descendre le crayon
 */
struct ast_node *make_cmd_down(){
	struct ast_node *node = calloc(1, sizeof(struct ast_node));
	node->kind = KIND_CMD_SIMPLE;
	node->u.cmd = CMD_DOWN;
	node->children_count = 0;
	return node;
}

/*
 * Crée un noeud pour faire une rotation à droite
 */
struct ast_node *make_cmd_right(struct ast_node *expr){
	struct ast_node *node = calloc(1, sizeof(struct ast_node));
	node->kind = KIND_CMD_SIMPLE;
	node->u.cmd = CMD_RIGHT;
	node->children_count = 1;
	node->children[0] = expr;
	return node;
}

/*
 * Crée un noeud pour une rotation à gauche
 */
struct ast_node *make_cmd_left(struct ast_node *expr){
	struct ast_node *node = calloc(1, sizeof(struct ast_node));
	node->kind = KIND_CMD_SIMPLE;
	node->u.cmd = CMD_LEFT;
	node->children_count = 1;
	node->children[0] = expr;
	return node;
}

/*
 * Crée un noeud pour afficher la commande
 */
struct ast_node *make_cmd_print(struct ast_node *expr){
	struct ast_node *node = calloc(1, sizeof(struct ast_node));
	node->kind = KIND_CMD_SIMPLE;
	node->u.cmd = CMD_PRINT;
	node->children_count = 1;
	node->children[0] = expr;
	return node;
}

/*
 * Crée un noeud heading
 */
struct ast_node *make_cmd_heading(struct ast_node *expr){
	struct ast_node *node = calloc(1, sizeof(struct ast_node));
	node->kind = KIND_CMD_SIMPLE;
	node->u.cmd = CMD_HEADING;
	node->children_count = 1;
	node->children[0] = expr;
	return node ;
}

/*
 * Crée un noeud pour une position
 */
struct ast_node *make_cmd_position(struct ast_node *x ,struct ast_node *y){
	struct ast_node *node = calloc(1, sizeof(struct ast_node));
	node->kind = KIND_CMD_SIMPLE;
	node->u.cmd = CMD_POSITION;
	node->children_count = 2;
	node->children[0] = x;
	node->children[1] = y;
	return node; 
}


//COLOR

/*
 * Crée un noeud pour une couleur
 */
struct ast_node *make_cmd_color(struct ast_node *r, struct ast_node *g, struct ast_node *b){
	struct ast_node *node = calloc(1, sizeof(struct ast_node));
	node->kind = KIND_CMD_SIMPLE;
	node->u.cmd = CMD_COLOR;
	node->children_count = 3;
	node->children[0] = r;
	node->children[1] = g;
	node->children[2] = b;
	return node;
}


//BINOP

/*
 * Crée un noeud une opération binaire
 */
struct ast_node *make_expr_binop(struct ast_node *operande1, char operator, struct ast_node *operande2){
	struct ast_node *node = calloc(1, sizeof(struct ast_node));
	node->kind = KIND_EXPR_BINOP;
	node->u.op = operator;
	node->children_count = 2;
	node->children[0] = operande1;
	node->children[1] = operande2;
	return node;
}



//FONCTION ANGLE

/*
 * Crée un noeud pour la fonction sinus
 */
struct ast_node *make_func_sin(struct ast_node *expr){
	struct ast_node *node = calloc(1, sizeof(struct ast_node));
	node->kind = KIND_EXPR_FUNC;
	node->u.func = FUNC_SIN;
	node->children_count = 1;
	node->children[0] = expr;
	return node;
}

/*
 * Crée un noeud fonctions cosinus
 */
struct ast_node *make_func_cos(struct ast_node *expr){
	struct ast_node *node = calloc(1, sizeof(struct ast_node));
	node->kind = KIND_EXPR_FUNC;
	node->u.func = FUNC_COS;
	node->children_count = 1;
	node->children[0] = expr;
	return node;
}

/*
 * Crée un noeud pour la fonction tangeante
 */
struct ast_node *make_func_tan(struct ast_node *expr){
	struct ast_node *node = calloc(1, sizeof(struct ast_node));
	node->kind = KIND_EXPR_FUNC;
	node->u.func = FUNC_TAN;
	node->children_count = 1;
	node->children[0] = expr;
	return node;
}

/*
 * Crée un noeud la fonction racine (d'un nombre)
 */
struct ast_node *make_func_sqrt(struct ast_node *expr){
	struct ast_node *node = calloc(1, sizeof(struct ast_node));
	node->kind = KIND_EXPR_FUNC;
	node->u.func = FUNC_SQRT;
	node->children_count = 1;
	node->children[0] = expr;
	return node;
}

/*
 * Crée un noeud pour une fonction random
 */
struct ast_node *make_func_random(struct ast_node *inf, struct ast_node *sup){
	struct ast_node *node = calloc(1, sizeof(struct ast_node));
	node->kind = KIND_EXPR_FUNC;
	node->u.func = FUNC_RANDOM;
	node->children_count = 2;
	node->children[0] = inf;
	node->children[1] = sup;
	return node;
}




/*
 *
 *		DESTRUCTIONS DES NOEUDS ET DE L'ARBRE
 *
 */

/*
 * Destruction des variables et des procédures ajoutées dans les tableaux au cours de l'analyse
 */
void destroyVariableProcedure(struct context *ctx) {
	for (int i = 0; i < ctx->nbVars; i++){
		free(ctx->vars[i].name);
	}
	for (int i = 0; i < ctx->nbProcs; i++){
		free(ctx->procs[i].name);
	}
}

/*
 * Destruction de l'arbre d'analyse
 */
void ast_destroy(struct ast *self) {
	ast_node_destroy(self->unit);
}

/*
 * Destruction des noeuds de l'arbre d'analyse
 */
void ast_node_destroy(struct ast_node *self) {
	if(self != NULL) {
		if (self->kind == KIND_EXPR_NAME) {
			free((void *)self->u.name);
			self->u.name = NULL;
		}
		for (int i = 0; i < self->children_count; i++) {
			ast_node_destroy(self->children[i]);
		}
		ast_node_destroy(self->next);
		free(self);
		self = NULL;
	}
}




/*
 * Création du contexte : toutes les variables sont initialisées
 */
void context_create(struct context *ctx) {
	//INITIALISATION
	ctx->x = 0;
	ctx->y = 0;

	ctx->angle = 0;
	ctx->up = false;

	ctx->r = 0;
	ctx->g = 0;
	ctx->b = 0;

	ctx->nbVars = 0;
	ctx->nbProcs = 0;
	
	setVariable(ctx, "PI", PI);
	setVariable(ctx, "SQRT2", SQRT2);
	setVariable(ctx, "SQRT3", SQRT3);
}




/*
 *
 *		FONCTION SUR LES VARIABLES
 *
 */


/*
 * permet d'ajouter une varibale
 */
int setVariable(struct context *ctx, const char *name, double var) {
	if (variableIsSet(ctx, name)) {
		for (size_t i = 0; i < ctx->nbVars; i++) {
			if(strcmp(ctx->vars[i].name, name) == 0) {
				ctx->vars[i].var = var;
			}
		}
	} else {
		ctx->vars[ctx->nbVars].var = var;
		char *curr = calloc(strlen(name)+1, sizeof(char));
		strcpy(curr, name);
		ctx->vars[ctx->nbVars].name = curr;
		ctx->nbVars++;
	}
	return 0;
}

/*
 * vérifie si une variable est déjà ajouté
 */
bool variableIsSet(struct context* ctx, const char *name) {
	for (size_t i = 0; i < ctx->nbVars; i++) {
		if (strcmp(ctx->vars[i].name, name) == 0){
			return true;
		}
	}
	return false;
}

/*
 * retourne une variable si elle existe, 0 sinon
 */
double getVariable(struct context *self, const char *name) {
	for (size_t i = 0; i < self->nbVars; i++) {
		if(strcmp(self->vars[i].name, name) == 0) {
			return self->vars[i].var;
		}
	}
	return 0;
}




/*
 *
 *		FONCTION SUR LES PROCEDURES
 *
 */


/*
 * permet d'ajouter une procédure
 */
int setProcedure(struct context *ctx, const char *name, struct ast_node *self) {
	if (procedureIsSet(ctx, name)) {
		return 1;
	} else {
		ctx->procs[ctx->nbProcs].proc = self;
		char *current = calloc(strlen(name)+1, sizeof(char));
		strcpy(current, name);
		ctx->procs[ctx->nbProcs].name = current;
		ctx->nbProcs++;
	}
	return 0;
}

/*
 * permet de vérifier si une procédure à déjà été ajouté
 */
bool procedureIsSet(struct context *ctx, const char *name) {
	for (size_t i = 0; i < ctx->nbProcs; i++) {
		if (strcmp(ctx->procs[i].name, name) == 0) {
			return true;
		}
	}
	return false;
}

/*
 * retourne le noeud de la procédure passer en paramètre, retourne NULL sinon
 */
struct ast_node *getProcedure(struct context *ctx, const char *name) {
	for (size_t i = 0; i < ctx->nbProcs; i++) {
		if (strcmp(ctx->procs[i].name, name) == 0) {
			return ctx->procs[i].proc;
		}
	}
	return NULL;
}




/*
 *
 *		EVALUATION
 *
 */


/*
 * Evaluation des expressions
 */
double expr_eval(struct ast_node *self, int *valRetour, struct context *ctx) {
	switch (self->kind) {
		case KIND_EXPR_VALUE :
			return self->u.value;
		break;
		case KIND_EXPR_MOINS :
			return -expr_eval(self->children[0], valRetour , ctx);
		break;
		case KIND_EXPR_FUNC : 
			return evaluation_fonction(self, valRetour, ctx);
		break;
		case KIND_EXPR_BINOP :
			return evaluation_operationBinaire(self, valRetour, ctx);
		break;
		case KIND_EXPR_NAME :
			if (!variableIsSet(ctx, self->u.name)) {
				fprintf(stdout, "Erreur : variable inconnu\n");
				return 0;
			}
			return getVariable(ctx, self->u.name);
		default :
		return 1.0;
	}
}

/*
 * Evaluation sur les operations binaires
 */
double evaluation_operationBinaire(struct ast_node* self, int *valRetour, struct context *ctx) {
	double op1 = expr_eval(self->children[0], valRetour, ctx);
	double op2 = expr_eval(self->children[1], valRetour, ctx);
	switch (self->u.op) {
		case '+' :
			return op1 + op2;
		break;
		
		case '-' :
			return op1 - op2;
		break;
		
		case '/' :
			if (op2 == 0) {
				fprintf(stdout, "Erreur, division par zero\n");
				return 0;
			}
			return op1 / op2;
		break;
		
		case '*' :
			return op1 * op2;
		break;
		
		case '^' : {
			return pow(op1, op2);
		}
		break;
		
		default:
		return 0;
	}
}

/*
 * Evaluation sur les fonctions
 */
double evaluation_fonction(struct ast_node* self, int *valRetour, struct context *ctx) {
	double val = expr_eval(self->children[0], valRetour, ctx);
	
	switch(self->u.func) {
		
		case FUNC_SIN :
			return sin(val*PI/180);
		break;
		
		case FUNC_COS :
			return cos(val*PI/180);
		break;
		
		case FUNC_TAN : 
			return tan(val*PI/180);
		break;
		
		case FUNC_SQRT : {
			if (val <= 0) {
				return 0;
			}
			return sqrt(val);
		}
		
		case FUNC_RANDOM : {
			double max = expr_eval(self->children[1], valRetour, ctx);
			if(val > max) {
				return 0;
			}
			return val + (double)rand() /((double)RAND_MAX/(max - val));
		}
		break;
		
		default:
		return 0;
	}
	return -1.0;
}

/*
 * evaluer l'arbre, pour cela on doit parcourir tous les noeuds
 */
int ast_eval(const struct ast *self, struct context *ctx) {
	int retour = 0;
	retour = ast_node_eval(self->unit, ctx);
	return retour;
}

/*
 * Evaluation du noeuds
 */
int ast_node_eval(const struct ast_node *self, struct context *ctx) {
	int valRetour = 0;
	double res_exprEval;
	
	switch (self->kind) {
		
		case KIND_CMD_SIMPLE :
			
			switch (self->u.cmd) {
			
				case CMD_FORWARD : {
					res_exprEval = expr_eval(self->children[0], &valRetour, ctx);
					if (valRetour != 0) {
						return valRetour;
					}
					ctx->x = ctx->x + sin(ctx->angle*PI/180)*res_exprEval;
					ctx->y = ctx->y - cos(ctx->angle*PI/180)*res_exprEval;
					if(ctx->up) {
						fprintf(stdout, "MoveTo %f %f\n", ctx->x, ctx->y);
					} else {
						fprintf(stdout, "LineTo %f %f\n", ctx->x, ctx->y);
					}		
				}
				break;
				
				case CMD_BACKWARD : {
					res_exprEval = expr_eval(self->children[0], &valRetour, ctx);
					if (valRetour != 0) {
						return valRetour;
					}
					ctx->angle += 180;
					ctx->x = ctx->x + sin(ctx->angle*PI/180)*res_exprEval;
					ctx->y = ctx->y - cos(ctx->angle*PI/180)*res_exprEval;
					if(ctx->up) {
						fprintf(stdout, "MoveTo %f %f\n", ctx->x, ctx->y);
					} else {
						fprintf(stdout, "LineTo %f %f\n", ctx->x, ctx->y);
					}
					ctx->angle -= 180;
				}
				break;
				
				case CMD_PRINT:
					res_exprEval = expr_eval(self->children[0] , &valRetour , ctx);
					if (valRetour != 0) {
						return valRetour;
					}
					fprintf(stdout, "> %f\n", res_exprEval);
				break ;
				
				case CMD_UP:
					ctx->up = true;
				break;
				
				case CMD_DOWN :
					ctx->up = false;
				break;
				
				case CMD_RIGHT :
					res_exprEval = expr_eval(self->children[0], &valRetour, ctx);
					if (valRetour != 0) {
						return valRetour;
					}
					ctx->angle = ctx->angle + res_exprEval;
					while (ctx->angle >= 360) {
						ctx->angle = ctx->angle - 360;
					}
					while (ctx->angle < 0) {
						ctx->angle = ctx->angle + 360;
					}
				break;
				
				case CMD_LEFT :
					res_exprEval = expr_eval(self->children[0], &valRetour, ctx);
					if (valRetour != 0) {
						return valRetour;
					}
					ctx->angle = ctx->angle - res_exprEval;
					while (ctx->angle >= 360) {
						ctx->angle = ctx->angle - 360;
					}
					while (ctx->angle < 0) {
						ctx->angle = ctx->angle + 360;
					}
				break;
				
				case CMD_HEADING :
					res_exprEval = expr_eval(self->children[0], &valRetour, ctx);
					if (valRetour != 0) {
						return valRetour;
					}
					ctx->angle = res_exprEval;
					while (ctx->angle >= 360) {
						ctx->angle = ctx->angle - 360;
					}
					while (ctx->angle < 0) {
						ctx->angle = ctx->angle + 360;
					}
				break;
				
				case CMD_POSITION : {
					double x = expr_eval(self->children[0], &valRetour, ctx);
					if (valRetour != 0) {
						return valRetour;
					}
					double y = expr_eval(self->children[1], &valRetour, ctx);
					if (valRetour != 0) {
						return valRetour;
					}
					ctx->x = x;
					ctx->y = y;
					if(ctx->up){
						fprintf(stdout, "MoveTo %f %f\n", ctx->x, ctx->y);
					} else{
						fprintf(stdout, "LineTo %f %f\n", ctx->x, ctx->y);
					}
				}	
				break;
				
				case CMD_HOME :
					ctx->x = 0;
					ctx->y = 0;
					ctx->angle =0;
					ctx->r = 0.0;
					ctx->g = 0.0;
					ctx->b = 0.0;
					if(ctx->up){
						fprintf(stdout, "MoveTo %f %f\n", ctx->x, ctx->y);
					} else{
						fprintf(stdout, "LineTo %f %f\n", ctx->x, ctx->y);
					}
				break;
				
				case CMD_COLOR : {
					double c0 = expr_eval(self->children[0], &valRetour, ctx);
					if (valRetour != 0) {
						return valRetour;
					}
					double c1 = expr_eval(self->children[1], &valRetour, ctx);
					if (valRetour != 0) {
						return valRetour;
					}
					double c2 = expr_eval(self->children[2], &valRetour, ctx);
					if (valRetour != 0) {
						return valRetour;
					}
					if (c0 < 0 || c1 < 0 || c2 < 0 || c0 > 1 || c1 > 1 || c2 > 1) {
						fprintf(stdout, "Erreur dans la saisie des couleurs\n");
						return 1;
					}
					ctx->r = c0;
					ctx->g = c1;
					ctx->b = c2;
					fprintf(stdout, "Color %f %f %f\n", c0, c1, c2);
				}
				break;
				
				default:	
				break;
			}
		break;
		
		case KIND_CMD_REPEAT : {
			double valeur_Expression = expr_eval(self->children[0], &valRetour, ctx);
			if (valRetour != 0) { return valRetour; }
			if (valeur_Expression < 0) {
				fprintf(stdout, "Erreur nombre négatif\n");
				return 1;
			}
			double val;
			for (size_t i = 0; i < valeur_Expression; i++) {
				val = ast_node_eval(self->children[1], ctx);
				if (val != 0){
					return val;
				}
			}
		}	
		break;
		
		case KIND_CMD_PROC :
			valRetour = setProcedure(ctx, self->children[0]->u.name, self->children[1]);
			fprintf(stdout, "Erreur lors de l'ajout d'une procédure\n");
		break;
		
		case KIND_CMD_CALL :
			if (!procedureIsSet(ctx, self->children[0]->u.name)) {
				fprintf(stdout, "Erreur, la procedure n'est pas défnit\n");
				return 1;
			}
			ast_node_eval(getProcedure(ctx, self->children[0]->u.name), ctx);
		break;
		
		case KIND_CMD_SET : {
			double varSet = expr_eval(self->children[1], &valRetour, ctx);
			if(valRetour != 0) {
				return valRetour;
			}
			valRetour = setVariable(ctx, self->children[0]->u.name, varSet);
		}
		break;
		
		case KIND_EXPR_BLOCK :
			ast_node_eval(self->children[0], ctx);
		break;

		default :
		break;

	}
	if(self->next != NULL) {
		ast_node_eval(self->next, ctx);
	}
	return 0;
	
}

/*
 * print
 */

void ast_print(const struct ast *self) {

}


