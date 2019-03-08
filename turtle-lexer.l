%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
 
#include "turtle-ast.h"
#include "turtle-parser.h"
%}

%option warn 8bit nodefault noyywrap

DIGIT 		[0-9]
INTEGER  	0|([1-9]{DIGIT}*)
FLOAT 		(({DIGIT}+".")|("."{DIGIT}+)|({DIGIT}*"."{DIGIT}+)|({INTEGER}))([eE]("-"|"+")?{DIGIT}+)?
NAME 		[A-Z][A-Z0-9]*

%%

"forward"			{ return KW_FORWARD; }
"backward"			{ return KW_BACKWARD; }
"heading"			{ return KW_HEADING; }
"repeat"			{ return KW_REPEAT; }
"right"				{ return KW_RIGHT; }
"left"				{ return KW_LEFT; }
"call"				{ return KW_CALL; }
"position"			{ return KW_POSITION; }
"up"				{ return KW_UP; }
"down"				{ return KW_DOWN; }
"fw"				{ return KW_FORWARD; }
"bw"				{ return KW_BACKWARD; }
"proc"				{ return KW_PROC; }
"print"				{ return KW_PRINT; }
"set"				{ return KW_SET; }


"color"				{ return KW_COLOR; }
"red"				{ return KW_RED; }
"green"				{ return KW_GREEN; }
"blue"				{ return KW_BLUE; }
"black"				{ return KW_BLACK; }
"gray"				{ return KW_GRAY; }
"cyan"				{ return KW_CYAN; }
"yellow"			{ return KW_YELLOW; }
"magenta"			{ return KW_MAGENTA; }


","     	  		{ return ','; }
"+"           		{ return '+'; }
"-"           		{ return '-'; }
"*"           		{ return '*'; }
"/"           		{ return '/'; }
"("     	  		{ return '('; }
")"     	  		{ return ')'; }
"{"     	  		{ return '{'; }
"}"     	  		{ return '}'; }
"^"					{ return '^'; }

"sin"   			{ return KW_SIN; }
"cos"   			{ return KW_COS; }
"tan"   			{ return KW_TAN; }
"sqrt"   			{ return KW_SQRT; }
"random"   			{ return KW_RANDOM; }


{DIGIT}+			{ yylval.value = strtod(yytext, NULL); return VALUE; }
{FLOAT}				{ yylval.value = strtod(yytext, NULL); return VALUE; }
{NAME}				{ yylval.name = yytext; return NAME; }


[\n\t ]*			/* whitespace */
.					{ fprintf(stderr, "Unknown token: '%s'\n", yytext); exit(EXIT_FAILURE); }

%%
