#include "rdparserAST.h"
#include "token.h"

/* we define return 1 as problem
 * explicit "return;"
 */
// tok alway point to next "to analyse token"
int tok;
FILE *IN;

static long checkPoint() { return scaned - yyleng; }
static void advance() { tok = yylex(); }

static char *getIDCopy(_YYLVAL now) {
  char *tmp = malloc(sizeof(char) * strlen(yylval.id_name) + 1);
  strcpy(tmp, yylval.id_name);
  return tmp;
}

static char *getOpCopy() {
  char *tmp = malloc(sizeof(char) * strlen(yylval.id_name) + 1);
  strcpy(tmp, yytext);
  return tmp;
}

void error() { /* printf("error"); */
}

static void fallBack(FILE *IN, long tar) {
  // from begin;
  fseek(IN, tar, SEEK_SET);
  yyrestart(IN);
  scaned = tar;
  advance();
}

pAst BlockItem() {
  /* // still in old form */
  /* long scanTmp = checkPoint(); */
  /* if (Decl()) { */
  /* fallBack(IN, scanTmp); */
  /* } else { */
  /* return 0; */
  /* } */

  pAst tmp0;
  if (!(tmp0 = Stmt())) {
    error();
    return NULL;
  }

  return tmp0;
}

pAst BlockItems() {
  pAst tmp0;
  if (!(tmp0 = BlockItem())) {
    error();
    return NULL;
  }

  pAst tmp1;
  long scanTmp = checkPoint();
  if (!(tmp1 = BlockItems())) {
    fallBack(IN, scanTmp);
    return tmp0;
  }
  tmp0->next = tmp1;
  return tmp0;
}

pAst Block() {
  if (tok != Y_LBRACKET) {
    error();
    return NULL;
  }
  advance();

  long scanTmp = checkPoint();
  pAst tmp0;
  if (!(tmp0 = BlockItems())) {
    fallBack(IN, scanTmp);
    BlockNode(newNode());
  }
  if (tok != Y_RBRACKET) {
    error();
    return NULL;
  }
  advance();

  return BlockNode(tmp0);
}

pAst Stmt() {
  long scanTmp = checkPoint();
  pAst tmp0;
  if (!(tmp0 = LVal())) {
    fallBack(IN, scanTmp);
  } else {
    if (tok != Y_ASSIGN) {
      fallBack(IN, scanTmp);
    } else {
      char *op = getOpCopy();
      advance();

      pAst tmp1;
      if (!(tmp1 = Exp())) {
        error();
        return NULL;
      }

      if (tok != Y_SEMICOLON) {
        error();
        return NULL;
      }
      advance();
      return BinaryNode(tmp0, tmp1, op);
    }
  }

  pAst tmp2;
  if (!(tmp2 = Exp())) {
    fallBack(IN, scanTmp);
  } else {
    if (tok != Y_SEMICOLON) {
      error();
      return NULL;
    }
    advance();
    return tmp2;
  }

  if (tok != Y_SEMICOLON) {
    fallBack(IN, scanTmp);
  } else {
    advance();
    return newNode();
  }

  // TODO debuging here
  pAst tmp3;
  if (!(tmp3 = Block())) {
    fallBack(IN, scanTmp);
  } else {
    return tmp3;
  }

  if (tok != Y_WHILE) {
    fallBack(IN, scanTmp);
  } else {
    advance();
    if (tok != Y_LPAR) {
      error();
      return NULL;
    }
    advance();

    pAst tmp4;
    if (!(tmp4 = LOrExp())) {
      error();
      return NULL;
    }

    if (tok != Y_RPAR) {
      error();
      return NULL;
    }
    advance();

    pAst tmp5;
    if (!(tmp5 = Stmt())) {
      error();
      return NULL;
    }

    return WhileNode(tmp4, tmp5);
  }

  if (tok != Y_IF) {
    fallBack(IN, scanTmp);
  } else {
    advance();
    if (tok != Y_LPAR) {
      error();
      return NULL;
    }
    advance();

    pAst tmp6;
    if (!(tmp6 = LOrExp())) {
      error();
      return NULL;
    }

    if (tok != Y_RPAR) {
      error();
      return NULL;
    }
    advance();

    pAst tmp7;
    if (!(tmp7 = Stmt())) {
      error();
      return NULL;
    }

    long scanTmp1 = checkPoint();
    pAst tmp8 = NULL;
    if (tok != Y_ELSE) {
      fallBack(IN, scanTmp1);
    } else {
      advance();
      if (!(tmp8 = Stmt())) {
        error();
        return NULL;
      }
    }

    return IfNode(tmp6, tmp7, tmp8);
  }

  if (tok != Y_RETURN) {
    fallBack(IN, scanTmp);
  } else {
    advance();
    long scanTmp2 = checkPoint();
    pAst tmp9;
    if (!(tmp9 = Exp())) {
      fallBack(IN, scanTmp2);
    }
    if (tok != Y_SEMICOLON) {
      error();
      return NULL;
    }
    advance();

    return RetNode(tmp9);
  }

  long scanTmp3 = checkPoint();
  if (tok != Y_BREAK) {
    fallBack(IN, scanTmp3);
  } else {
    return BrkNode();
  }

  if (tok != Y_CONTINUE) {
    error();
    return NULL;
  } else {
    advance();
    if (tok != Y_SEMICOLON) {
      error();
      return NULL;
    }
    advance();
    return CountinueNode();
  }
}

pAst RelExp() {
  pAst tmp0;
  if (!(tmp0 = AddExp())) {
    error();
    return NULL;
  }

  long scanTmp = checkPoint();
  if (!(tok == Y_LESS || tok == Y_GREAT || tok == Y_LESSEQ ||
        tok == Y_GREATEQ)) {
    fallBack(IN, scanTmp);
    return tmp0;
  }
  char *op = getOpCopy();
  advance();

  pAst tmp1;
  if (!(tmp1 = RelExp())) {
    error();
    return NULL;
  }

  return BinaryNode(tmp0, tmp1, op);
}

pAst EqExp() {
  pAst tmp0;
  if (!(tmp0 = RelExp())) {
    error();
    return NULL;
  }

  long scanTmp = checkPoint();
  if (!(tok == Y_EQ || tok == Y_NOTEQ)) {
    fallBack(IN, scanTmp);
    return tmp0;
  }
  char *op = getOpCopy();
  advance();

  pAst tmp1;
  if (!(tmp1 = EqExp())) {
    error();
    return NULL;
  }

  return BinaryNode(tmp0, tmp1, op);
}

pAst LAndExp() {
  pAst tmp0;
  if (!(tmp0 = EqExp())) {
    error();
    return NULL;
  }

  long scanTmp = checkPoint();
  enum yytokentype op;
  if (tok != Y_AND) {
    fallBack(IN, scanTmp);
    return tmp0;
  }
  op = tok;
  advance();

  pAst tmp1;
  if (!(tmp1 = LAndExp())) {
    error();
    return NULL;
  }

  return LAndExp(tmp0, tmp1, op);
}

pAst LOrExp() {
  pAst tmp0;
  if (!(tmp0 = LAndExp())) {
    error();
    return NULL;
  }

  long scanTmp = checkPoint();
  if (tok != Y_OR) {
    fallBack(IN, scanTmp);
    return tmp0;
  }
  char *op = getOpCopy();
  advance();

  pAst tmp1;
  if (!(tmp1 = LOrExp())) {
    error();
    return NULL;
  }

  return BinaryNode(tmp0, tmp1, op);
}

pAst CallParams() {
  pAst tmp0;
  if (!(tmp0 = Exp())) {
    error();
    return NULL;
  }

  long scanTmp = checkPoint();
  if (tok != Y_COMMA) {
    fallBack(IN, scanTmp);
    return tmp0;
  }
  advance();
  pAst tmp1;
  if (!(tmp1 = CallParams())) {
    error();
    return NULL;
  }
  return ParamNode(tmp0, tmp1);
}

pAst ArraySubscripts() {
  if (tok != Y_LSQUARE) {
    error();
    return NULL;
  }
  advance();
  pAst tmp0;
  if (!(tmp0 = Exp())) {
    error();
    return NULL;
  }
  if (tok != Y_RSQUARE) {
    error();
    return NULL;
  }
  advance();

  long scanTmp = checkPoint();
  pAst tmp1;
  if (!(tmp1 = ArraySubscripts())) {
    fallBack(IN, scanTmp);
  }

  return ArraySubscriptsNode(tmp0, tmp1);
}

pAst LVal() {
  if (tok != Y_ID) {
    error();
    return NULL;
  }
  char *ID = getIDCopy(yylval);
  advance();

  long scanTmp = checkPoint();
  pAst tmp0;
  if (!(tmp0 = ArraySubscripts())) {
    fallBack(IN, scanTmp);
  }

  return LValNode(ID, tmp0);
}

pAst Exp() {
  pAst tmp0;
  if (!(tmp0 = AddExp())) {
    error();
    return NULL;
  }
  return tmp0;
}

pAst PrimaryExp() {
  long scanTmp = checkPoint();

  if (tok != Y_LPAR) {
    fallBack(IN, scanTmp);
  } else {
    // 65
    advance();
    pAst tmp0;
    if (!(tmp0 = Exp())) {
      error();
      return NULL;
    }
    if (tok != Y_RPAR) {
      error();
      return NULL;
    }
    advance();
    return tmp0;
  }

  // 66
  pAst tmp1;
  if (!(tmp1 = LVal())) {
    fallBack(IN, scanTmp);
  } else {
    return tmp1;
  }

  // 67 68
  if (!(tok == num_INT || tok == num_FLOAT)) {
    error();
    return NULL;
  }
  pAst tmp2;
  if (tok == num_INT) {
    tmp2 = IntNode(yylval.int_value);
  } else {
    tmp2 = FloatNode(yylval.float_value);
  }
  advance();
  return tmp2;
}

pAst UnaryExp() {
  // all tmp val

  long scanTmp = checkPoint();
  // 70 || 71
  if (!(tok == Y_ID)) {
    fallBack(IN, scanTmp);
  } else {
    char *ID = getIDCopy(yylval);
    advance();
    if (!(tok == Y_LPAR)) {
      fallBack(IN, scanTmp);
    } else {
      advance();

      long scanTmp1 = checkPoint();
      // 70
      if (!(tok == Y_RPAR)) {
        fallBack(IN, scanTmp1);
      } else {
        advance();
        return UnaryNode(NULL, ID);
      }

      // 71
      pAst tmp0;
      if (!(tmp0 = CallParams())) {
        error();
        return NULL;
      }

      if (tok != Y_RPAR) {
        error();
        return NULL;
      }
      advance();

      return UnaryNode(tmp0, ID);
    }
  }

  // 69
  pAst tmp1;
  if (!(tmp1 = PrimaryExp())) {
    fallBack(IN, scanTmp);
  } else {
    return tmp1;
  }

  // 72 73 74
  if (!(tok == Y_ADD || tok == Y_SUB || tok == Y_NOT)) {
    error();
    return NULL;
  }
  char *op = getOpCopy();
  advance();

  pAst tmp2;
  if (!(tmp2 = UnaryExp())) {
    error();
    return NULL;
  }
  // here sub bin exp
  // take care of the none func call sector
  // TODO heres some problme
  return BinaryNode(newNode(), tmp2, op);
}

pAst MulExp() {
  // 77
  pAst tmp0;
  if (!(tmp0 = UnaryExp())) {
    error();
    return NULL;
  }
  long scanTmp = checkPoint();
  if (!(tok == Y_MUL || tok == Y_DIV || tok == Y_MODULO)) {
    fallBack(IN, scanTmp);
    return tmp0;
  }
  char *op = getOpCopy();
  advance();

  pAst tmp1;
  if (!(tmp1 = MulExp())) {
    error();
    return NULL;
  }

  return BinaryNode(tmp0, tmp1, op);
}

pAst AddExp() {
  pAst tmp0;
  if (!(tmp0 = MulExp())) {
    error();
    return NULL;
  }

  long scanTmp = checkPoint();
  if (!(tok == Y_ADD || tok == Y_SUB)) {
    fallBack(IN, scanTmp);
    return tmp0;
  }
  char *op = getOpCopy();
  advance();

  pAst tmp1;
  if (!(tmp1 = AddExp())) {
    error();
    return NULL;
  }

  return BinaryNode(tmp0, tmp1, op);
}

pAst ConstExp() {
  pAst tmp0;
  if (!(tmp0 = AddExp())) {
    error();
    return NULL;
  }
  return tmp0;
}

enum yytokentype Type() {
  if (tok == Y_INT) {
    advance();
    return tok;
  }
  if (tok == Y_FLOAT) {
    advance();
    return tok;
  }
  if (tok == Y_VOID) {
    advance();
    return tok;
  }

  error();
  return 0;
}

int main() {
  IN = fopen("./dataIn.txt", "r");
  yyset_in(IN);
  advance();
  pAst res = Stmt();

  if (res) {
    printf("No ERROR\n");
    printf("stop at token: %s", yytext);
  } else {
    printf("ERROR");
    printf("stop at token: %s", yytext);
  }

  clean(res);
}
