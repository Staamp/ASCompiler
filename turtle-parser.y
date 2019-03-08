%{
#include <stdio.h>

#include "turtle-ast.h"

	bool isProcedure = false;
	int yylex();
	void yyerror(struct ast *ret, const char *);

%}

%debug
%defines

%define parse.error verbose

%parse-param { struct ast *ret }

%union {
  double value;
  const char *name;
  struct ast_node *node;
}

%token <value>    VALUE       "value"
%token <name>     NAME        "name"

%token            KW_FORWARD  "forward"
%token            KW_BACKWARD "backward"
%token            KW_DOWN	  "down"
%token            KW_HEADING  "heading"
%token            KW_REPEAT   "repeat"
%token            KW_RIGHT	  "right"
%token            KW_LEFT	  "left"
%token            KW_CALL	  "call"
%token            KW_POSITION "position"
%token            KW_UP		  "up"
%token            KW_FW		  "fw"
%token            KW_BW		  "bw"
%token            KW_PROC	  "proc"
%token            KW_PRINT	  "print"
%token            KW_SET	  "set"

%token            KW_COLOR	  "color"
%token            KW_RED      "red"
%token            KW_GREEN    "green"
%token            KW_BLUE     "blue"
%token            KW_BLACK    "black"
%token            KW_GRAY     "gray"
%token            KW_CYAN     "cyan"
%token            KW_YELLOW   "yellow"
%token            KW_MAGENTA  "magenta"

%token            KW_SIN      "sin"
%token            KW_COS      "cos"
%token            KW_TAN      "tan"
%token            KW_SQRT     "sqrt"
%token            KW_RANDOM   "random"


%type <node> unit cmds cmd expr color expr_name block proc

%left '+' '-'
%left '*' '/'
%left MOINS
%right '^'
%%

unit:
    cmds              { $$ = $1; ret->unit = $$; }
;

cmds:
	  cmd cmds          { $1->next = $2; $$ = $1; }
	| proc cmds 		{ isProcedure = true; $1->next = $2; $$ = $1; }
	| /* empty */       { $$ = NULL; }
;

cmd: 
	  KW_FORWARD expr				{ $$ = make_cmd_forward($2); }
	| KW_BACKWARD expr				{ $$ = make_cmd_backward($2); }
  	| KW_UP                         { $$ = make_cmd_up(); }
	| KW_DOWN                      	{ $$ = make_cmd_down(); }
	| KW_RIGHT expr                 { $$ = make_cmd_right($2); }
	| KW_LEFT expr                  { $$ = make_cmd_left($2); }
	| KW_PRINT expr                 { $$ = make_cmd_print($2); }
	| KW_HEADING expr               { $$ = make_cmd_heading($2); }
	| KW_POSITION  expr ',' expr	{ $$ = make_cmd_position($2, $4); }

	| KW_REPEAT expr cmd			{ $$ = make_cmd_repeat($2, $3); }
	
	| KW_CALL expr_name				{ $$ = make_cmd_call($2); }
	
	| KW_COLOR color 				{ $$ = $2; }
	
	| KW_SET expr_name expr			{ $$ = make_cmd_set($2, $3); }
	
	| block 						{ $$ = $1; }
	
;

proc:
	KW_PROC expr_name cmd			{ $$ = make_cmd_proc($2, $3); }
;

block : 
	'{' cmds '}'					{ $$ = make_expr_block($2); 
										if(isProcedure) {
											yyerror(ret, "La procedure est déjà définie\n");
											return 1;
										}
									}
;


expr_name:
	NAME		{ $$ = make_expr_name($1); }
;

color:
	  expr ',' expr ',' expr    { $$ = make_cmd_color($1 , $3 , $5); }
	| KW_RED					{ $$ = make_cmd_color(make_expr_value(1.0),make_expr_value(0.0),make_expr_value(0.0)); }
	| KW_GREEN					{ $$ = make_cmd_color(make_expr_value(0.0),make_expr_value(1.0),make_expr_value(0.0)); }
	| KW_BLUE					{ $$ = make_cmd_color(make_expr_value(0.0),make_expr_value(0.0),make_expr_value(1.0)); }
	| KW_BLACK					{ $$ = make_cmd_color(make_expr_value(0.0),make_expr_value(0.0),make_expr_value(0.0)); }
	| KW_GRAY					{ $$ = make_cmd_color(make_expr_value(0.5),make_expr_value(0.5),make_expr_value(0.5)); }
	| KW_CYAN					{ $$ = make_cmd_color(make_expr_value(0.0),make_expr_value(0.1),make_expr_value(0.1)); }
	| KW_YELLOW					{ $$ = make_cmd_color(make_expr_value(1.0),make_expr_value(0.0),make_expr_value(1.0)); }
	| KW_MAGENTA				{ $$ = make_cmd_color(make_expr_value(1.0),make_expr_value(1.0),make_expr_value(0.0)); }
	 
;




expr:
      VALUE             { $$ = make_expr_value($1); }
    | NAME				{ $$ = make_expr_name($1); }
    
    | '(' expr ')'		{ $$ = $2; }
	| expr '+' expr		{ $$ = make_expr_binop($1, '+', $3); }
	| expr '-' expr		{ $$ = make_expr_binop($1, '-', $3); }
	| expr '/' expr		{ $$ = make_expr_binop($1, '/', $3); }
	| expr '*' expr		{ $$ = make_expr_binop($1, '*', $3); }
	| expr '^' expr		{ $$ = make_expr_binop($1, '^', $3); }
	
	| '-' expr %prec MOINS { $$ = make_expr_moins($2); }

	| KW_SIN '(' expr')'				{ $$ = make_func_sin($3); }
	| KW_COS '(' expr')'				{ $$ = make_func_cos($3); }
	| KW_TAN '(' expr')'				{ $$ = make_func_tan($3); }
	| KW_SQRT '(' expr')'				{ $$ = make_func_sqrt($3); }
	| KW_RANDOM '(' expr ',' expr ')'	{ $$ = make_func_random($3, $5); }

;

%%

void yyerror(struct ast *ret, const char *msg) {
  (void) ret;
  fprintf(stderr, "%s\n", msg);
}
