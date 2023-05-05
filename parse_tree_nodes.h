/**************************************************************************
  Name: Connor Brooks             NetID: cib52 
  Course: CSE 4714              Assignment: Part 4 
  Programming Environment: VSC  
  Purpose of File: Contains the parse_tree_nodes
***************************************************************************/

#ifndef PARSE_TREE_NODES_H
#define PARSE_TREE_NODES_H

#include <iostream>
#include <vector>
#include <string>
#include "lexer.h"

using namespace std;

extern map<string, int> symbolTable;

// Forward declaration of classes
class ExprNode;
class SimpleExprNode;
class FactorNode;
class IdNode;
class IntLitNode;
class FloatLitNode;
class NestedExprNode;
class TermNode;
class StatementNode;
class AssignNode;
class CompNode;
class IfNode;
class NotNode;
class MinusNode;
class WhileNode;
class ReadNode;
class ReadNode;
class WriteNode;
class ProgramNode;
class BlockNode;

// Forward declaration of operator<< 
ostream& operator<<(ostream& os, FactorNode& fn);
ostream& operator<<(ostream& os, ExprNode& en);
ostream& operator<<(ostream& os, SimpleExprNode& en);
ostream& operator<<(ostream& os, ProgramNode& en);
ostream& operator<<(ostream& os, BlockNode& en);
ostream& operator<<(ostream& os, StatementNode& fn);

//<<<<<<<<<<<<<<<<<<<<<<<< Classes >>>>>>>>>>>>>>>>>>>>>>>>>>

//ProgramNode Class Declaration
class ProgramNode {
public:
	string* value = nullptr;
	BlockNode* firstBlock = nullptr;
	ProgramNode(BlockNode* block, string name);
    ~ProgramNode();
	int interpret();
};

ProgramNode::ProgramNode(BlockNode* block, string name) {
	firstBlock = block;
	value = new string(name);
}

//BlockNode Class Declaration
class BlockNode {
public:
	CompNode* firstComp = nullptr;
	BlockNode(CompNode* firstComp);
    ~BlockNode();
	int interpret();
};

BlockNode::BlockNode(CompNode* comp) {
	firstComp = comp;
}

//StatementNode Class Declaration (Shell for assignment, compound, if, while, read, and write)
class StatementNode {
public:
	virtual int interpret() = 0;
    virtual void printTo(ostream &os) = 0; // pure virtual method, makes the class Abstract
    virtual ~StatementNode(); // labeling the destructor as virtual allows 
};

//AssignNode Class Declaration
class AssignNode : public StatementNode {
public:
	void printTo(ostream &os);
	ExprNode* firstExpr = nullptr;
	string* firstAssignName = nullptr;
	void TakeIn1(string value) {
		firstAssignName = new string(value);
	}
	~AssignNode();
	int interpret();
};

//CompNode Class Declaration
class CompNode : public StatementNode {
public:
	void printTo(ostream &os);
	vector<StatementNode*> statements;
    vector<int> restStatementOps; // TOK_:
	~CompNode();
	int interpret();
};

//IfNode Class Declaration
class IfNode : public StatementNode {
public:
	void printTo(ostream &os);
	ExprNode* firstExpr = nullptr;
	StatementNode* firstState = nullptr;
	StatementNode* secondState = nullptr;
	int token = 0;
	IfNode(ExprNode* expr, StatementNode* state1, StatementNode* state2, int name);
	~IfNode();
	int interpret();
};

//IfNode Class Constructor
IfNode::IfNode(ExprNode* expr, StatementNode* state1, StatementNode* state2, int name) {
	firstExpr = expr;
	firstState = state1;
	secondState = state2;
	token = name;
	int interpret();
}

//WhileNode Class Declaration
class WhileNode : public StatementNode {
public:
	void printTo(ostream &os);
	ExprNode* firstExpr = nullptr;
	StatementNode* firstState = nullptr;
	~WhileNode();
	int interpret();
};

//ReadNode Class Declaration
class ReadNode : public StatementNode {
public:
    string* value = nullptr;

    void ReadValue(string name) {
		value = new string(name);
	}
    ~ReadNode();
    void printTo(ostream & os);
	int interpret();
};

//WriteNode Class Declaration
class WriteNode : public StatementNode {
public:
	string* value = nullptr;
	int token = 0;
    void WriteValue(string name) {
		value = new string(name);
	}
	void TokenValue(int name) {
		token = name;
	}
    ~WriteNode();
    void printTo(ostream & os);
	int interpret();
};

//ExprNode Class Declaration
class ExprNode {
public:
	SimpleExprNode* firstSimple = nullptr;
    vector<int> restSimpleOps; // TOK_EQUALTO or TOK_LESSTHAN or TOK_GREATERTHAN
    vector<SimpleExprNode*> restSimple;
	 ~ExprNode();
	 int interpret();
};

//SimpleExprNode Class Declaration
class SimpleExprNode {
public:
    TermNode* firstTerm = nullptr;
    vector<int> restTermOps; // TOK_ADD_OP or TOK_SUB_OP
    vector<TermNode*> restTerms;
	int interpret();
    ~SimpleExprNode();
};

//TermNode Class Declaration
class TermNode {
public:
    FactorNode* firstFactor = nullptr;
    vector<int> restFactorOps; // TOK_MULT_OP or TOK_DIV_OP
    vector<FactorNode*> restFactors;

	int interpret();
    ~TermNode();
};

//FactorNode Class Declaration (Shell for IDNode, IntLitNode, FloatLitNode, NestedExprNode, NotNode, and MinusNode)
class FactorNode {
public:
	virtual int interpret() = 0;
    virtual void printTo(ostream &os) = 0; // pure virtual method, makes the class Abstract
    virtual ~FactorNode(); // labeling the destructor as virtual allows 
	                       // the subclass destructors to be called
};

//FloatLitNode CLass Declaration
class FloatLitNode : public FactorNode {
public:
    float float_literal = 0;

    FloatLitNode(float value);
    ~FloatLitNode();
    void printTo(ostream & os);
	int interpret();
};

//IntLitNode CLass Declaration
class IntLitNode : public FactorNode {
public:
    int int_literal = 0;

    IntLitNode(int value);
    ~IntLitNode();
    void printTo(ostream & os);
	int interpret();
};

//IdNode CLass Declaration
class IdNode : public FactorNode {
public:
    string* id = nullptr;

    IdNode(string name);
    ~IdNode();
    void printTo(ostream & os);
	int interpret();
};

//MinusNode CLass Declaration
class MinusNode : public FactorNode {
public:
    FactorNode* firstFactor = nullptr;

    MinusNode(FactorNode* fF);
    ~MinusNode();
    void printTo(ostream & os);
	int interpret();
};

//NotNode CLass Declaration
class NotNode : public FactorNode {
public:
    FactorNode* firstFactor = nullptr;

    NotNode(FactorNode* fF);
    ~NotNode();
    void printTo(ostream & os);
	int interpret();
};

//NestedExprNode Class Declaration
class NestedExprNode : public FactorNode {
public:
    ExprNode* exprPtr = nullptr;

    NestedExprNode(ExprNode* en);
    void printTo(ostream & os);
	int interpret();
    ~NestedExprNode();
};

//Classes ostreams
ostream& operator<<(ostream& os, ProgramNode& fn) {
	os << "Program Name ";
	os << *(fn.value) << endl;
	os << *(fn.firstBlock);
	return os;
}

ostream& operator<<(ostream& os, BlockNode& fn) {
	os << *(fn.firstComp);
	return os;
}

ostream& operator<<(ostream& os, FactorNode& fn) {
	fn.printTo(os);
	return os;
}


NotNode::NotNode(FactorNode* fF) {
	firstFactor = fF;
}

void NotNode::printTo(ostream& os) {
	os << "factor( NOT " << *firstFactor << " )";
}

MinusNode::MinusNode(FactorNode* fF) {
	firstFactor = fF;
}

void MinusNode::printTo(ostream& os) {
	os << "factor( - " << *firstFactor << " )";
}

IdNode::IdNode(string name) {
	id = new string(name);
}

void IdNode::printTo(ostream& os) {
	os << "factor( " << *id << " )";
}

IntLitNode::IntLitNode(int value) {
	int_literal = value;
}

void IntLitNode::printTo(ostream& os) {
	os << "factor( " << int_literal << " )";
}

FloatLitNode::FloatLitNode(float value) {
	float_literal = value;
}

void FloatLitNode::printTo(ostream& os) {
	os << "factor( " << float_literal << " )";
}

NestedExprNode::NestedExprNode(ExprNode* en) {
	exprPtr = en;
}

void NestedExprNode::printTo(ostream& os) {
	os << "nested_expression( " << *exprPtr << " )";
}

ostream& operator<<(ostream& os, TermNode& tn) {
	os << "term( ";
	os << *(tn.firstFactor);

	int length = tn.restFactorOps.size();
	for (int i = 0; i < length; ++i) {
		int op = tn.restFactorOps[i];
		if (op == TOK_MULTIPLY)
			os << " * ";
		else
			os << " / ";
		os << *(tn.restFactors[i]);
	}
	os << " )";
	return os;
}

ostream& operator<<(ostream& os, ExprNode& en) {
	os << "expression( ";
	os << *(en.firstSimple);
	int length = en.restSimpleOps.size();
	for (int i = 0; i < length; ++i) {

		int op = en.restSimpleOps[i];
		if (op == TOK_EQUALTO)
			os << " = ";
		if (op == TOK_LESSTHAN) 
			os << " < ";
		if (op == TOK_NOTEQUALTO)
			os << " <> ";
		if (op == TOK_GREATERTHAN)
			os << " > ";
		os << *(en.restSimple[i]);
	}
	os << " )";
	return os;
}

ostream& operator<<(ostream& os, SimpleExprNode& en) {
	os << "simple_expression( ";
	os << *(en.firstTerm);

	int length = en.restTermOps.size();
	for (int i = 0; i < length; ++i) {

		int op = en.restTermOps[i];
		if (op == TOK_PLUS)
			os << " + ";
		else
			os << " - ";
		os << *(en.restTerms[i]);
	}
	os << " )";
	return os;
}

ostream& operator<<(ostream& os, StatementNode& fn) {
	fn.printTo(os);
	return os;
}

void AssignNode::printTo(ostream& os) {
	os << "Assignment " << *firstAssignName << " := " << *firstExpr << endl;
}

void CompNode::printTo(ostream &os) {
	os << "Begin Compound Statement" << endl;
	int length = statements.size();
	for (int i = 0; i < length; ++i) {		
		os << *statements[i];
	}
	os << "End Compound Statement";
}

void IfNode::printTo(ostream &os) {
	os << "If "<< *firstExpr << endl;
	os << "%%%%%%%% True Statement %%%%%%%%" << endl;
	os << *firstState;
	os << "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%" << endl;
	
	if (token == TOK_ELSE) {
		os << "%%%%%%%% False Statement %%%%%%%%" << endl;
		os << *secondState << endl;
		os << "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%" << endl;
	}
}

void WhileNode::printTo(ostream &os) {
	os << "While " << *firstExpr << endl;
	os << "%%%%%%%% Loop Body %%%%%%%%" << endl;
	os << *firstState << endl;
	os << "%%%%%%%%%%%%%%%%%%%%%%%%%%%" << endl;
}

void ReadNode::printTo(ostream& os) {
	os << "Read Value " << *value << endl;
}

void WriteNode::printTo(ostream& os) {
	if (token == TOK_STRINGLIT) {
		os << "Write String " << *value << endl;
	}
	if (token == TOK_IDENT) {
		os << "Write Value " << *value << endl;
	}
	
}

//<<<<<<<<<<<<<<<<<<<<<<<< Class Interpreters >>>>>>>>>>>>>>>>>>>>>>>>>>
//IdNode Interpreter
int IdNode::interpret() {
	int var = symbolTable.find(*id)->second;
	return var;
}

//MinusNode Interpreter
int MinusNode::interpret() {
	int returnValue = firstFactor->interpret();
	returnValue = returnValue * -1;

	return returnValue;
}

//NotNode Interpreter
int NotNode::interpret() {
	int returnValue = firstFactor->interpret();
	if(returnValue == 1) {
		returnValue = 0;
	}
	else {
		returnValue = 1;
	}

	return returnValue;
}

//FloatLitNode Interpreter
int FloatLitNode::interpret() {
	return int(float_literal);
}

//IntLitNode Interpreter
int IntLitNode::interpret() {
    return int_literal;   
}

//NestedExprNode Interpreter
int NestedExprNode::interpret() {
    return exprPtr->interpret();
}

//TermNode Interpreter
int TermNode::interpret() {
    // a Term is a Factor followed by 0 or more Factors separated by * or / operations

    // get the value of the first Factor
    int returnValue = firstFactor->interpret();

    int length = restFactorOps.size();
    for (int i = 0; i < length; ++i) {
        // get the value of the next Factor
        int nextValue = restFactors[i]->interpret();

        // perform the operation (* or /) that separates the Factors
        if (restFactorOps[i] == TOK_MULTIPLY) {
            returnValue = returnValue * nextValue;
		}
		else if (restFactorOps[i] == TOK_AND) {
			if((returnValue == 1) && (nextValue == 1)) {
				returnValue = 1;
			}
		}
        else
            returnValue = returnValue / nextValue;
    }

    return returnValue;
}

//SimpleExprNode Interpreter
int SimpleExprNode::interpret() {
    int returnValue = firstTerm->interpret();

    int length = restTermOps.size();
    for (int i = 0; i < length; ++i) {
        int nextValue = restTerms[i]->interpret();
        if (restTermOps[i] == TOK_PLUS)
            returnValue = returnValue + nextValue;
        else
            returnValue = returnValue - nextValue;
    }
	
    return returnValue;
}

//ExprNode Interpreter
int ExprNode::interpret() {
    int returnValue = firstSimple->interpret();

    int length = restSimpleOps.size();
    for (int i = 0; i < length; ++i) {
        int nextValue = restSimple[i]->interpret();

        if (restSimpleOps[i] == TOK_EQUALTO) {
			if (returnValue == nextValue) {
				returnValue = 1;
			}
			else {
				returnValue = 0;
			}
		}
        else if(restSimpleOps[i] == TOK_LESSTHAN) {
			if (returnValue < nextValue) {
				returnValue = 1;
			}
			else {
				returnValue = 0;
			}
		}
        else if(restSimpleOps[i] == TOK_GREATERTHAN) {
			if (returnValue > nextValue) {
				returnValue = 1;
			}
			else {
				returnValue = 0;
			}
		}
        else if(restSimpleOps[i] == TOK_NOTEQUALTO) {
			if (returnValue != nextValue) {
				returnValue = 1;
			}
			else {
				returnValue = 0;
			}
		}
    }
    return returnValue;
}

//WriteNode Interpreter
int WriteNode::interpret() {
	if(token == TOK_STRINGLIT) {
		string word = *value;
		cout << word.substr(1, word.size() - 2) << endl;
	}
	else {
		int var = symbolTable.find(*value)->second;
		cout << var << endl;
		
	}
	
}

//ReadNode Interpreter
int ReadNode::interpret() {
	int input;
	cin >> input;

	map<string, int>::iterator it;

	it = symbolTable.find(*value);
	it->second = input;
}

//WhileNode Interpreter
int WhileNode::interpret() {
	while(firstExpr->interpret() == 1) {
		firstState->interpret();
	}
}

//IfNode Interpreter
int IfNode::interpret() {
	if(firstExpr->interpret() == 1) {
		firstState->interpret();
	}
	else {
		if(secondState != nullptr) {
			secondState->interpret();
		}
	}
}

//CompNode Interpreter
int CompNode::interpret() {	
	int length = statements.size();
	for(int i = 0; i < length; ++i) {
		statements[i]->interpret();
	}
}

//AssignNode Interpreter
int AssignNode::interpret() {
	symbolTable.find(*firstAssignName)->second = firstExpr->interpret();
}

//StatementNode Interpreter
int StatementNode::interpret() {
	return this->interpret();
}

//BlockNode Interpreter
int BlockNode::interpret() {
	firstComp->interpret();
}

//ProgramNode Interpreter 
int ProgramNode::interpret() {
	firstBlock->interpret();
}

//<<<<<<<<<<<<<<<<<<<<<<<< Class Deconstrutors >>>>>>>>>>>>>>>>>>>>>>>>>>
ProgramNode::~ProgramNode() {
	cout << "Deleting a programNode" << endl;
	delete firstBlock;
	firstBlock = nullptr;
	delete value;
	value = nullptr;
}

BlockNode::~BlockNode() {
	cout << "Deleting a blockNode" << endl;
	delete firstComp;
	firstComp = nullptr;
}

StatementNode::~StatementNode() {}

CompNode::~CompNode() {
	cout << "Deleting a compoundNode" << endl;
	int length = statements.size();
	for (int i = 0; i < length; ++i) {		
		delete statements[i];
		statements[i] = nullptr;
	}
}

AssignNode::~AssignNode() {
	cout << "Deleting an assignmentNode" << endl;
	delete firstExpr;
	firstExpr = nullptr;
	
}

IfNode::~IfNode() {
	cout << "Deleting an ifNode" << endl;
	delete firstExpr;
	firstExpr = nullptr;
	delete firstState;
	firstState = nullptr;
	if(secondState != nullptr) {
		delete secondState;
		secondState = nullptr;
	}
	
}

WhileNode::~WhileNode() {
	cout << "Deleting a whileNode" << endl;
	delete firstExpr;
	firstExpr = nullptr;
	delete firstState;
	firstState = nullptr;

}

ReadNode::~ReadNode() {
	cout << "Deleting a readNode" << endl;
	delete value;
	value = nullptr;
}

WriteNode::~WriteNode() {
	cout << "Deleting a writeNode" << endl;
	delete value;
	value = nullptr;
}

ExprNode::~ExprNode() {
	cout << "Deleting an expressionNode" << endl;
	delete firstSimple;
	firstSimple = nullptr;
	if (!(restSimple.empty())) {
		int length = restSimpleOps.size();
		for (int i = 0; i < length; ++i) {
			delete restSimple[i];
			restSimple[i] = nullptr;
		}
	}
}

SimpleExprNode::~SimpleExprNode() {
	cout << "Deleting a simpleExpressionNode" << endl;
	delete firstTerm;
	firstTerm = nullptr;

	int length = restTermOps.size();
	for (int i = 0; i < length; ++i) {
		delete restTerms[i];
		restTerms[i] = nullptr;
	}
	
}

TermNode::~TermNode() {
	cout << "Deleting a termNode" << endl;
	delete firstFactor;
	firstFactor = nullptr;

	int length = restFactorOps.size();
	for (int i = 0; i < length; ++i) {
		delete restFactors[i];
		restFactors[i] = nullptr;
	}
}

FactorNode::~FactorNode() {}

NotNode::~NotNode() {
	delete firstFactor;
	firstFactor = nullptr;
}

MinusNode::~MinusNode() {
	delete firstFactor;
	firstFactor = nullptr;
}

IdNode::~IdNode() {
	cout << "Deleting a factorNode" << endl;
	delete id;
	id = nullptr;
}

IntLitNode::~IntLitNode() {
	cout << "Deleting a factorNode" << endl;
	// Nothing to do since the only member variable is not a pointer
}

FloatLitNode::~FloatLitNode() {
	cout << "Deleting a factorNode" << endl;
	// Nothing to do since the only member variable is not a pointer
}

NestedExprNode::~NestedExprNode() {
	cout << "Deleting a factorNode" << endl;
	delete exprPtr;
	exprPtr = nullptr;
}

#endif /* PARSE_TREE_NODES_H */
