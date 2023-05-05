/**************************************************************************
  Name: Connor Brooks             NetID: cib52 
  Course: CSE 4714              Assignment: Part 4 
  Programming Environment: VSC  
  Purpose of File: Contains the productions
***************************************************************************/

#ifndef PRODUCTIONS_H
#define PRODUCTIONS_H

#include "lexer.h"
#include "parse_tree_nodes.h"
#include <iostream>
#include <string>
#include <map>

using namespace std;

extern map<string, int> symbolTable;
int nextToken = 0;  // token returned from yylex
int level = 0;  // used to indent output to approximate parse tree


extern "C"
{
	// Instantiate global variables used by flex
    extern FILE *yyin;
	extern char* yytext;       // text of current lexeme
	extern int   yylex();      // the generated lexical analyzer
}

// Forward declarations of production parsing functions
ProgramNode* program();
FactorNode* factor();
TermNode* term();
SimpleExprNode* simple_expr();
ExprNode* expr();
StatementNode* statement();
BlockNode* block();
WriteNode* write();
ReadNode* read();
WhileNode* whilestatement();
IfNode* ifstatement();
CompNode* compound();
AssignNode* assignment();
bool first_of_factor();
void error();
bool first_of_term();
bool first_of_expr();
bool first_of_simple_expr();

// Forward declarations of functions that check whether the current token is
// in the first set of a production rule
bool first_of_program();
bool in_symbol_table();


bool in_symbol_table(string varname) {
    map<string, int>::iterator i; // Look up values in symbol table

    i = symbolTable.find(varname);
    return !(i == symbolTable.end());
}

inline void indent(){
    for (int i = 0; i<level; i++)
        cout << ("    ");
}

void output(){
    indent();
    cout << "-->found " << yytext << endl;
}

//Parses Program
ProgramNode* program() {
    indent();
    string value;
    BlockNode* firstBlock = nullptr;
    cout << "enter <program>" << endl;
    ++level;
    output();
    nextToken = yylex();

    if (nextToken == TOK_IDENT)
    {
        output();
        value = string(yytext);
        nextToken = yylex();
        if (nextToken == TOK_SEMICOLON)
        {
            output();
            nextToken = yylex();
            firstBlock = block();
        }
    }
    ProgramNode* newProgramNode = new ProgramNode(firstBlock, value);
    --level;
    indent();
    cout << "exit <program>" << endl;
    return newProgramNode;

}

//Parses Block
BlockNode* block() {
    indent();
    cout << "enter <block>" << endl;
    ++level;
    if(nextToken == TOK_VAR) {
        output();
        nextToken = yylex();
        do {
            
            if (in_symbol_table(yytext))
                throw "101: identifier declared twice";
            else
                string var = yytext;
                symbolTable[yytext] = 0;
            
            output();
            nextToken = yylex();
            output();
            nextToken = yylex();
            output();
            nextToken = yylex();
            output();
            nextToken = yylex();
            cout << endl;
        }
        while(nextToken == TOK_IDENT);
    }
    CompNode* value = compound();
    BlockNode* newBlockNode = new BlockNode(value);
    --level;
    indent();
    cout << "exit <block>" << endl;
    return newBlockNode;
}

//Parses Statement (Shell for assignment, compound, if, while, read, and write)
StatementNode* statement()
{
    indent();

    cout << "enter <statement>" << endl;
    StatementNode* newStatementNode = nullptr;
    ++level;

    if (nextToken == TOK_IDENT)
    {
        newStatementNode = assignment();
    }

    else if (nextToken == TOK_BEGIN)
    {
        newStatementNode = compound();
    }

    else if (nextToken == TOK_IF)
    {
        newStatementNode = ifstatement();
    }

    else if (nextToken == TOK_WHILE)
    {
        newStatementNode = whilestatement();
    }

    else if (nextToken == TOK_READ)
    {
        newStatementNode = read();
    }

    else if (nextToken == TOK_WRITE)
    {
        newStatementNode = write();
    }
    --level;
    indent();
    cout << "exit <statement>" << endl;
    return newStatementNode;
}

//Parses Assignment Statements
AssignNode* assignment() {
    indent();
    cout << "enter <assignment>" << endl;
    AssignNode* newAssignNode = new AssignNode;
    ++level;
    
    if (!in_symbol_table(yytext))
        throw "104: identifier not declared";
    output();
    string var = yytext;
    symbolTable[var] = 0;
    newAssignNode->TakeIn1(string(yytext));
    nextToken = yylex();
    output();
    nextToken = yylex();
    newAssignNode->firstExpr = expr();
    --level;
    indent();
    cout << "exit <assignment>" << endl;
    return newAssignNode;
}

//Parses Compound Statements
CompNode* compound() {
    indent();
    cout << "enter <compound_statement>" << endl;
    CompNode* newCompNode = new CompNode;
    ++level;
    output();
    nextToken = yylex();
    newCompNode->statements.push_back(statement());
    while(nextToken == TOK_SEMICOLON ) {
      output();
      newCompNode->restStatementOps.push_back(nextToken);
      nextToken = yylex();
      newCompNode->statements.push_back(statement());
    }
    output();
    nextToken = yylex(); 
    --level;
    indent();
    cout << "exit <compound_statement>" << endl;
    return newCompNode;
}

//Parses If Statements
IfNode* ifstatement() {
    StatementNode* value2 = nullptr;
    StatementNode* value3 = nullptr;
    ExprNode* value1 = nullptr;
    int token = 0;
    indent();
    cout << "enter <if statement>" << endl;
    ++level;
    output();
    nextToken = yylex();
    value1 = expr();
    output();
    nextToken = yylex();
    value2 = statement();
    token = nextToken;
    if(nextToken == TOK_ELSE) {
        output();
        nextToken = yylex();
        value3 = statement();
    }
    IfNode* newIfNode = new IfNode(value1, value2, value3, token);
    --level;
    indent();
    cout << "exit <if statement>" << endl;
    return newIfNode;
}

//Parses While Statements
WhileNode* whilestatement() {
    indent();
    cout << "enter <while statement>" << endl;
    WhileNode* newWhileNode = new WhileNode;
    ++level;
    output();
    nextToken = yylex();
    newWhileNode->firstExpr = expr();
    newWhileNode->firstState = statement();
    --level;
    indent();
    cout << "exit <while statement>" << endl;
    return newWhileNode;
}

//Parses Read Statements
ReadNode* read() {
    indent();
    cout << "enter <read>" << endl;
    ReadNode* newReadNode = new ReadNode;
    ++level;
    output();
    nextToken = yylex();
    output();
    nextToken = yylex();
    
    if (!in_symbol_table(yytext))
        throw "104: identifier not declared";
    
    output();
    newReadNode->ReadValue(string(yytext));
    nextToken = yylex();
    output();
    nextToken = yylex();
    --level;
    indent();
    cout << "exit <read>" << endl;
    return newReadNode;
}

//Parses Write Statements
WriteNode* write() {
    indent();
    cout << "enter <write>" << endl;
    WriteNode* newWriteNode = new WriteNode;
    ++level;
    output();
    nextToken = yylex();
    if(nextToken == TOK_OPENPAREN) {
        output();
        nextToken = yylex();
    }
    if(nextToken == TOK_IDENT || nextToken == TOK_STRINGLIT) {
      newWriteNode->TokenValue(nextToken);      
      if(nextToken == TOK_IDENT)
          if (!in_symbol_table(yytext))
              throw "104: identifier not declared";
                
      output();
      newWriteNode->WriteValue(string(yytext));
      nextToken = yylex();
    }
    if(nextToken == TOK_CLOSEPAREN) {
        output();
        nextToken = yylex();
    }
    --level;
    indent();
    cout << "exit <write>" << endl;
    return newWriteNode;
}

//Parses Expressions
ExprNode* expr() {
  if (!first_of_expr())
    error();

  indent();
  cout << "enter <expression>" << endl;
  ++level;
  ExprNode* newExprNode = new ExprNode;
  newExprNode->firstSimple = simple_expr();
  if (nextToken == TOK_EQUALTO || nextToken == TOK_LESSTHAN || nextToken == TOK_GREATERTHAN || nextToken == TOK_NOTEQUALTO)
  {

    output();
    newExprNode->restSimpleOps.push_back(nextToken);
    nextToken = yylex();
    newExprNode->restSimple.push_back(simple_expr());
  }

  --level;
  indent();
  cout << "exit <expression>" << endl;
  return newExprNode;
}

bool first_of_expr(){
  return nextToken == TOK_IDENT || nextToken == TOK_INTLIT || nextToken == TOK_FLOATLIT || nextToken == TOK_OPENPAREN || nextToken == TOK_NOT || nextToken == TOK_MINUS;
}

//Parses Simple Expressions
SimpleExprNode* simple_expr() {
  /* Check that the <expr> starts with a valid token */
  if (!first_of_simple_expr())
    error();

  indent();
  cout << "enter <simple expression>" << endl;
  level = level + 1;
  SimpleExprNode* newSimpleExprNode = new SimpleExprNode;

  /* Parse the first term */
  newSimpleExprNode->firstTerm = term();

  /* As long as the next token is + or -, get
     the next token and parse the next term */
  while (nextToken == TOK_PLUS || nextToken == TOK_MINUS || nextToken == TOK_OR) {
    output();
    newSimpleExprNode->restTermOps.push_back(nextToken);
    nextToken = yylex();
    newSimpleExprNode->restTerms.push_back(term());
  }

  level = level - 1;
  indent();
  cout << "exit <simple expression>" << endl;

  return newSimpleExprNode;
} /* End of function expr */

bool first_of_simple_expr(){
  return nextToken == TOK_OPENPAREN || nextToken == TOK_IDENT || nextToken == TOK_INTLIT || nextToken == TOK_FLOATLIT || nextToken == TOK_NOT || nextToken == TOK_MINUS;
}


//Parses Terms
TermNode* term() {
  /* Check that the <term> starts with a valid token */
  if (!first_of_term())
    error();

  indent();
  cout << "enter <term>" << endl;
  ++level;
  TermNode* newTermNode = new TermNode;

  /* Parse the first factor */
  newTermNode->firstFactor = factor();

  /* As long as the next token is * or /, get the
     next token and parse the next factor */
  while (nextToken == TOK_MULTIPLY || nextToken == TOK_DIVIDE || nextToken == TOK_AND) {
    output();
    newTermNode->restFactorOps.push_back(nextToken);
    nextToken = yylex();
    newTermNode->restFactors.push_back(factor());
  }

  --level;
  indent();
  cout << "exit <term>" << endl;

  return newTermNode;
} /* End of function term */

bool first_of_term(){
  return nextToken == TOK_OPENPAREN || nextToken == TOK_IDENT || nextToken == TOK_INTLIT || nextToken == TOK_FLOATLIT || nextToken == TOK_NOT || nextToken == TOK_MINUS;
}

//Parses Factors 
FactorNode* factor() {
  /* Check that the <factor> starts with a valid token */
  if (!first_of_factor())
    error();
  indent();
  cout << "enter <factor>" << endl;
  ++level;
  FactorNode* newFactorNode = nullptr;

  /* Determine which RHS */
  // Determine what token we have
  switch (nextToken) {

    case TOK_IDENT:
      if (!in_symbol_table(yytext))
        throw "104: identifier not declared";
      output();
      newFactorNode = new IdNode(string(yytext));
      nextToken = yylex(); // Read past what we have found
      break;

    case TOK_INTLIT:
      output();
      newFactorNode = new IntLitNode(atoi(yytext));
      nextToken = yylex();
      break;

    case TOK_FLOATLIT:
      output();
      newFactorNode = new FloatLitNode(atof(yytext));
      nextToken = yylex();
      break;

    case TOK_OPENPAREN:
      output();
      nextToken = yylex();
      newFactorNode = new NestedExprNode(expr());
      output();
      nextToken = yylex();
      break;

    case TOK_NOT:
      output();
      nextToken = yylex();
      newFactorNode = new NotNode(factor());
      break;

    case TOK_MINUS:
      output();
      nextToken = yylex();
      newFactorNode = new MinusNode(factor());
      break;
	}
  --level;
  indent();
  cout << "exit <factor>" << endl;

  return newFactorNode;
} /* End of function factor */
//*****************************************************************************

bool first_of_factor(){
  return nextToken == TOK_OPENPAREN || nextToken == TOK_IDENT || nextToken == TOK_INTLIT || nextToken == TOK_FLOATLIT || nextToken == TOK_NOT || nextToken == TOK_MINUS;
}

void error() {
  cout << endl << "===========================" << endl;
  cout << "ERROR near: " << yytext;
  cout << endl << "===========================" << endl;
  if (yyin)
    fclose(yyin);
  exit(EXIT_FAILURE);
}

bool first_of_program(void) {
    return nextToken == TOK_PROGRAM;
}

#endif