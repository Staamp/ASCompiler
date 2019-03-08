#ifndef TURTLE_AST_H
#define TURTLE_AST_H

#include <stddef.h>
#include <stdbool.h>

// simple commands
enum ast_cmd {
  CMD_UP,
  CMD_DOWN,
  CMD_RIGHT,
  CMD_LEFT,
  CMD_HEADING,
  CMD_FORWARD,
  CMD_BACKWARD,
  CMD_POSITION,
  CMD_HOME,
  CMD_COLOR,
  CMD_PRINT,
};

// internal functions
enum ast_func {
  FUNC_COS,
  FUNC_RANDOM,
  FUNC_SIN,
  FUNC_SQRT,
  FUNC_TAN,
};

// kind of a node in the abstract syntax tree
enum ast_kind {
  KIND_CMD_SIMPLE,
  KIND_CMD_REPEAT,
  KIND_CMD_BLOCK,
  KIND_CMD_PROC,
  KIND_CMD_CALL,
  KIND_CMD_SET,

  KIND_EXPR_FUNC,
  KIND_EXPR_VALUE,
  KIND_EXPR_BINOP,
  KIND_EXPR_BLOCK,
  KIND_EXPR_NAME,
  KIND_EXPR_MOINS,
};

#define AST_CHILDREN_MAX 3
// a node in the abstract syntax tree
struct ast_node {
  enum ast_kind kind; // kind of the node
  union {
    enum ast_cmd cmd;   // kind == KIND_CMD_SIMPLE
    double value;       // kind == KIND_EXPR_VALUE, for literals
    char op;            // kind == KIND_EXPR_BINOP, for operators in expressions
    const char *name;   // kind == KIND_EXPR_NAME, the name of procedures and variables
    enum ast_func func; // kind == KIND_EXPR_FUNC, a function
  } u;
  size_t children_count;  // the number of children of the node
  struct ast_node *children[AST_CHILDREN_MAX];  // the children of the node (arguments of commands, etc)
  struct ast_node *next;  // the next node in the sequence
};


//
//	CONSTRUCTEUR
//


struct ast_node *make_expr_value(double value);
struct ast_node *make_expr_name(const char *name);
struct ast_node *make_expr_block(struct ast_node *cmds);
struct ast_node *make_expr_moins(struct ast_node *expr);
struct ast_node *make_cmd_repeat(struct ast_node *nb, struct ast_node *cmd);
struct ast_node *make_cmd_proc(struct ast_node *name, struct ast_node *cmd);
struct ast_node *make_cmd_call(struct ast_node *name);
struct ast_node *make_cmd_set(struct ast_node *name, struct ast_node * expr);

struct ast_node *make_cmd_forward(struct ast_node *expr);
struct ast_node *make_cmd_backward(struct ast_node *expr);
struct ast_node *make_cmd_up();
struct ast_node *make_cmd_down();
struct ast_node *make_cmd_right(struct ast_node *expr);
struct ast_node *make_cmd_left(struct ast_node *expr);
struct ast_node *make_cmd_print(struct ast_node *expr);
struct ast_node *make_cmd_heading(struct ast_node *expr);
struct ast_node *make_cmd_position(struct ast_node *x ,struct ast_node *y);

struct ast_node *make_cmd_color(struct ast_node *r, struct ast_node *g, struct ast_node *b);

struct ast_node *make_expr_binop(struct ast_node *operande1, char operator, struct ast_node *operande2);


struct ast_node *make_func_sin(struct ast_node *expr);
struct ast_node *make_func_cos(struct ast_node *expr);
struct ast_node *make_func_tan(struct ast_node *expr);
struct ast_node *make_func_sqrt(struct ast_node *expr);
struct ast_node *make_func_random(struct ast_node *inf, struct ast_node *sup);












// root of the abstract syntax tree
struct ast {
  struct ast_node *unit;
};








//
//	 PROCEDURE
//

// procedure handling
struct procedure {
	char *name;
	struct ast_node *proc;
};


//
//	VARIABLE
//

// variable handling
struct variable {
	char *name;
	double var;
};


//
//	CONTEXT
//

// the execution context
struct context {
	double x;
	double y;

	double angle;
	bool up;

	//color
	double r;
	double g;
	double b;

	
	struct variable vars[1024];
	size_t nbVars;

	struct procedure procs[1024];
	size_t nbProcs;
};




// do not forget to destroy properly! no leaks allowed!
void destroyVariableProcedure(struct context *ctx);
void ast_destroy(struct ast *self);
void ast_node_destroy(struct ast_node *self);

// create an initial context
void context_create(struct context *self);

int setVariable(struct context *ctx, const char *name, double var);
bool variableIsSet(struct context* ctx, const char *name);
double getVariable(struct context *self, const char *name);

int setProcedure(struct context *ctx, const char *name, struct ast_node *self);
bool procedureIsSet(struct context *ctx, const char *name);
struct ast_node *getProcedure(struct context *ctx, const char *name);

// print the tree as if it was a Turtle program
void ast_print(const struct ast *self);

// evaluate the tree and generate some basic primitives
int ast_eval(const struct ast *self, struct context *ctx);
int ast_node_eval(const struct ast_node *self, struct context *ctx);

double expr_eval(struct ast_node *self, int *valRetour, struct context *ctx);
double evaluation_operationBinaire(struct ast_node* self, int *valRetour, struct context *ctx);
double evaluation_fonction(struct ast_node* self, int *valRetour, struct context *ctx);





#endif /* TURTLE_AST_H */
