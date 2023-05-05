/********************************************************************* 
  Name: Connor Brooks             NetID: cib52 
  Course: CSE 4714              Assignment: Part 2 
  Programming Environment:  
  Purpose of File: Contains the .... 
*********************************************************************/
%option yylineno
%option noyywrap
%{
#include "lexer.h"

%}

%%

 /*BEGIN */
BEGIN                               {return TOK_BEGIN; }

 /*BREAK */
BREAK                               {return TOK_BREAK; }

 /*CONTINUE */
CONTINUE                            {return TOK_CONTINUE; }

 /*DOWNTO */                  
DOWNTO                              {return TOK_DOWNTO; }

 /*ELSE */
ELSE                                {return TOK_ELSE; }

 /*END */
END                                 {return TOK_END; }

 /*FOR */
FOR                                 {return TOK_FOR; }

 /*IF */
IF                                  {return TOK_IF; }

 /*LET */
LET                                 {return TOK_LET; }

 /*PROGRAM */
PROGRAM                             {return TOK_PROGRAM; }

 /*READ */
READ                                {return TOK_READ; }

 /*THEN */
THEN                                {return TOK_THEN; }

 /*TO */
TO                                  {return TOK_TO; }

 /*VAR */
VAR                                 {return TOK_VAR; }

 /*WHILE */
WHILE                               {return TOK_WHILE; }

 /*WRITE */
WRITE                               {return TOK_WRITE; }

 /*INTEGER */
INTEGER                             {return TOK_INTEGER; }

 /*REAL */
REAL                                {return TOK_REAL; }

 /*; */
\;                                   {return TOK_SEMICOLON; }

 /*: */
\:                                   {return TOK_COLON; }

 /*( */
\(                                   {return TOK_OPENPAREN; }

 /*) */
\)                                   {return TOK_CLOSEPAREN; }

 /*{ */
\{                                   {return TOK_OPENBRACE; }

 /*} */
\}                                   {return TOK_CLOSEBRACE; }

 /*+ */
\+                                   {return TOK_PLUS; }

 /*- */
\-                                   {return TOK_MINUS; }

 /** */
\*                                   {return TOK_MULTIPLY; }

 /*/ */
\/                                   {return TOK_DIVIDE; }

 /*:= */
\:\=                                  {return TOK_ASSIGN; }

 /*= */
\=                                   {return TOK_EQUALTO; }

 /*< */
\<                                   {return TOK_LESSTHAN; }

 /*> */
\>                                   {return TOK_GREATERTHAN; }

 /*<> */
\<\>                                  {return TOK_NOTEQUALTO; }

 /*MOD */
MOD                                 {return TOK_MOD; }

 /*NOT */
NOT                                 {return TOK_NOT; }

 /*OR */
OR                                  {return TOK_OR; }

 /*AND */
AND                                 {return TOK_AND; }

 /*identifier */
[A-Z][A-Z0-9]*                      {if (strlen(yytext) > 8) {return TOK_UNKNOWN; } else {return TOK_IDENT; }} 

 /*integer literal */
[0-9]+                              {return TOK_INTLIT; }

 /*floating-point literal */
[0-9]+\.[0-9]+                      {return TOK_FLOATLIT; }

 /*string literal */
'[^'\n]*'                           {if (strlen(yytext) > 80) {return TOK_UNKNOWN; } else {return TOK_STRINGLIT; }}                                    

 /* Ignore whitespace */

[ \t\r\n]+              /* nop */

 /* Found an unknown character */

.         { return TOK_UNKNOWN; }

 /* Recognize end of file */

<<EOF>>   { return TOK_EOF; }
