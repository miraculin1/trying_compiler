#include "ast.h"
#include "node_type.h"
#include "token.h"
/* i think we need to progrem all kind of node
 * not only for this but for the bison (lab3)
 */

/* void displayAst(pAst now) { */
  /* printf("nodeType: %d, ", now->nodeType); */
  /* printf("{int%d|", now->val.int_value); */
  /* printf("float%f|", now->val.float_value); */
  /* printf("token%d|", now->val.token); */
  /* if (isIDNode(now)) { */
    /* printf("ID%s|", now->val.id_name); */
  /* } */
  /* printf("}\n"); */
  /* if (now->left) { */
    /* displayAst(now->left); */
  /* } */
  /* if (now->right) { */
    /* displayAst(now->right); */
  /* } */
  /* if (now->next) { */
    /* displayAst(now->next); */
  /* } */
/* } */

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
  if (now->left) {
    clean(now->left);
  }
  if (now->right) {
    clean(now->right);
  }
  if (now->svalue) {
    free(now->svalue);
  }
  free(now);
}

// 我本来想要直接算出来这个节点的值，相当于直接常数
// 传递咧，没必要。
// 看作表达式处理就可以了。
pAst BinaryNode(pAst l, pAst r, char* op) {
  pAst res = newNode();
  res->nodeType = BINARY_OPERATOR;

  res->svalue = op;
  res->left = l;
  res->right = r;

  return res;
}

pAst UnaryNode(pAst pram, char *name) {
  pAst res;
  // func call
  res = newNode();
  res->nodeType = UNARY_OPERATOR;

  res->left = pram;
  res->svalue = name;

  return res;
}

pAst LValNode(char *ID, pAst Array) {
  pAst res = newNode();
  res->nodeType = DECL_REF_EXPR;

  res->svalue = ID;
  res->left = Array;

  return res;
}

pAst IntNode(int val) {
  pAst res = newNode();
  res->nodeType = INTEGER_LITERAL;

  res->ivalue = val;

  return res;
}

pAst FloatNode(float val) {
  pAst res = newNode();
  res->nodeType = FLOATING_LITERAL;

  res->fvalue = val;

  return res;
}

pAst ArraySubscriptsNode(pAst exp, pAst Array) {
  pAst res = newNode();
  res->nodeType = ARRAY_SUBSCRIPT_EXPR;

  res->left = exp;
  res->next = Array;

  return res;
}

pAst ParamNode(pAst exp, pAst param) {
  pAst res = newNode();
  res->nodeType = PAREN_EXPR;

  res->ivalue = exp->ivalue;
  res->fvalue = exp->fvalue;
  res->svalue = exp->svalue;
  res->next = param;

  return res;
}

pAst WhileNode(pAst cond, pAst stmt) {
  pAst res = newNode();
  res->nodeType = WHILE_STMT;

  res->left = cond;
  res->right = stmt;

  return res;
}

pAst IfNode(pAst cond, pAst stmt, pAst ElseStmt) {
  pAst res = newNode();
  res->nodeType = IF_STMT;

  res->if_cond = cond;
  res->right = stmt;

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
  res->nodeType = RETURN_STMT;

  res->left = stmt;

  return res;
}

pAst BrkNode() {
  pAst res = newNode();
  res->nodeType = BREAK_STMT;

  return res;
}

pAst CountinueNode() {
  pAst res = newNode();
  res->nodeType = CONTINUE_STMT;

  return res;
}

pAst BlockNode(pAst exps) {
  pAst res = newNode();
  res->nodeType = BLOCK_EXPR;

  res->next = exps;

  return res;
}
