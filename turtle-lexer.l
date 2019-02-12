%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "turtle-ast.h"
#include "turtle-parser.h"
%}

%option warn 8bit nodefault noyywrap

DIGIT [0-9]

%%
"forward"             { return KW_FORWARD; }
"heading"             { return KW_HEADING; }
"repeat"              { return KW_REPEAT; }
"right"               { return KW_RIGHT; }
"left"				  { return KW_LEFT; }
"call"				  { return KW_CALL; }
"position"			  { return KW_POSITION; }
"up"				  { return KW_UP; }
"fw"				  { return KW_FW; }
"bw"				  { return KW_BW; }
"proc"				  { return KW_PROC; }
{DIGIT}+              { yylval.value = strtod(yytext, NULL); return VALUE; }

[\n\t ]*              /* whitespace */
.                     { fprintf(stderr, "Unknown token: '%s'\n", yytext); exit(EXIT_FAILURE); }

%%
