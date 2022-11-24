#ifndef _AST_H
#define _AST_H
#include "token.h"
#include "node_type.h"
#include "token.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

// enum valType {
  // vFloat = 0,
  // vInt,
  // vToken,
  // vID,
//
// };

typedef struct _ast ast;
typedef struct _ast* pAst;

// think up a suitable ast structure
struct _ast {
  node_type nType;
  _YYLVAL val;

  pAst lnode;
  pAst rnode;
  pAst next;
};

pAst UnaryNode(pAst pram, char* name);
pAst newNode();
pAst LValNode(char* ID, pAst Array);
pAst ArraySubscriptsNode(pAst exp, pAst Array);
pAst FloatNode(float val);
pAst IntNode(int val);
pAst ParamNode(pAst exp, pAst param);
pAst BinaryNode(pAst l, pAst r, enum yytokentype op);
pAst WhileNode(pAst cond, pAst stmt);
pAst IfNode(pAst cond, pAst stmt, pAst ElseStmt);
pAst RetNode(pAst stmt);
pAst BrkNode();
pAst CountinueNode();
pAst BlockNode(pAst exps);

void displayAst(pAst now);
void clean(pAst now);
bool IDNode(pAst now);

#endif
