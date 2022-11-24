#include "ast.h"
#include "node_type.h"
#include "token.h"
/* i think we need to progrem all kind of node
 * not only for this but for the bison (lab3)
 */

bool isIDNode(pAst now) {
  return (now->nType == DECL_REF_EXPR || now->nType == UNARY_OPERATOR ||
          now->nType == PAREN_EXPR);
}
void displayAst(pAst now) {
  printf("nodeType: %d, ", now->nType);
  printf("{int%d|", now->val.int_value);
  printf("float%f|", now->val.float_value);
  printf("token%d|", now->val.token);
  if (isIDNode(now)) {
    printf("ID%s|", now->val.id_name);
  }
  printf("}\n");
  if (now->lnode) {
    displayAst(now->lnode);
  }
  if (now->rnode) {
    displayAst(now->rnode);
  }
  if (now->next) {
    displayAst(now->next);
  }
}

pAst newNode() {
  pAst tmp = malloc(sizeof(ast));
  if (tmp == NULL) {
    // out of mem
    exit(1);
  }
  memset(tmp, 0, sizeof(ast));
  return tmp;
}

void clean(pAst now) {
  if (!now) return;

  if (now->next) {
    clean(now->next);
  }
  if (now->lnode) {
    clean(now->lnode);
  }
  if (now->rnode) {
    clean(now->rnode);
  }
  if (isIDNode(now)) {
    free(now->val.id_name);
  }
  free(now);
}

// 我本来想要直接算出来这个节点的值，相当于直接常数
// 传递咧，没必要。
// 看作表达式处理就可以了。
pAst BinaryNode(pAst l, pAst r, enum yytokentype op) {
  pAst res = newNode();
  res->nType = BINARY_OPERATOR;

  res->val.token = op;
  res->lnode = l;
  res->rnode = r;

  return res;
}

pAst UnaryNode(pAst pram, char *name) {
  pAst res;
  // func call
  res = newNode();
  res->nType = UNARY_OPERATOR;

  res->lnode = pram;
  res->val.id_name = name;

  return res;
}

pAst LValNode(char *ID, pAst Array) {
  pAst res = newNode();
  res->nType = DECL_REF_EXPR;

  res->val.id_name = ID;
  res->lnode = Array;

  return res;
}

pAst IntNode(int val) {
  pAst res = newNode();
  res->nType = INTEGER_LITERAL;

  res->val.int_value = val;

  return res;
}

pAst FloatNode(float val) {
  pAst res = newNode();
  res->nType = FLOATING_LITERAL;

  res->val.float_value = val;

  return res;
}

pAst ArraySubscriptsNode(pAst exp, pAst Array) {
  pAst res = newNode();
  res->nType = ARRAY_SUBSCRIPT_EXPR;

  res->lnode = exp;
  res->next = Array;

  return res;
}

pAst ParamNode(pAst exp, pAst param) {
  pAst res = newNode();
  res->nType = PAREN_EXPR;

  res->val = exp->val;
  res->next = param;

  return res;
}

pAst WhileNode(pAst cond, pAst stmt) {
  pAst res = newNode();
  res->nType = WHILE_STMT;

  res->lnode = cond;
  res->rnode = stmt;

  return res;
}

pAst IfNode(pAst cond, pAst stmt, pAst ElseStmt) {
  pAst res = newNode();
  res->nType = IF_STMT;

  res->lnode = cond;
  res->rnode = stmt;

  // TODO trying to put else to next
  // we alway get a elsestmt but if no else then thats
  // empty
  if (ElseStmt) {
    res->next = ElseStmt;
  } else {
    res->next = newNode();
  }

  return res;
}

pAst RetNode(pAst stmt) {
  pAst res = newNode();
  res->nType = RETURN_STMT;

  res->lnode = stmt;

  return res;
}

pAst BrkNode() {
  pAst res = newNode();
  res->nType = BREAK_STMT;

  return res;
}

pAst CountinueNode() {
  pAst res = newNode();
  res->nType = CONTINUE_STMT;

  return res;
}

pAst BlockNode(pAst exps) {
  pAst res = newNode();
  res->nType = BLOCK_EXPR;

  res->next = exps;

  return res;
}
