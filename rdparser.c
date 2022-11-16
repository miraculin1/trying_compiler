#include "rdparser.h"
#include "lex.h"
#include "token.h"
#include <stdio.h>

/* we define return 1 as problem
 * explicit "return;"
 */
// tok alway point to next "to analyse token"
int tok;
FILE *IN;

// TODO think up a suitable ast structure
struct _ast {

  struct _ast *lnode;
  struct _ast *rnode;
  struct _ast *next;
};

static long checkPoint() { return scaned - yyleng; }
static void advance() { tok = yylex(); }

void error() { /* printf("error"); */
}

// TODO this module not effective
static void fallBack(FILE *IN, long tar) {
  // from begin;
  fseek(IN, tar, SEEK_SET);
  yyrestart(IN);
  scaned = tar;
  advance();
}

// TODO change fallback logic

int CallParams() {
  if (Exp()) {
    error();
    return 1;
  }

  long scanTmp = checkPoint();
  if (tok != Y_COMMA) {
    fallBack(IN, scanTmp);
    return 0;
  }
  advance();
  if (CallParams()) {
    error();
    return 1;
  }
  return 0;
}

int ArraySubscripts() {
  if (tok != Y_LSQUARE) {
    error();
    return 1;
  }
  advance();
  if (Exp()) {
    error();
    return 1;
  }
  if (tok != Y_RSQUARE) {
    error();
    return 1;
  }
  advance();

  long scanTmp = checkPoint();
  if (ArraySubscripts()) {
    fallBack(IN, scanTmp);
  }

  return 0;
}

int LVal() {
  if (tok != Y_ID) {
    error();
    return 1;
  }
  advance();

  long scanTmp = checkPoint();
  if (ArraySubscripts()) {
    fallBack(IN, scanTmp);
  }
  return 0;
}

int Exp() {
  if (AddExp()) {
    error();
    return 1;
  }
  return 0;
}

int PrimaryExp() {
  long scanTmp = checkPoint();

  if (tok != Y_LPAR) {
    fallBack(IN, scanTmp);
  } else {
    // 65
    advance();
    if (Exp()) {
      error();
      return 1;
    }
    if (tok != Y_RPAR) {
      error();
      return 1;
    }
    advance();
    return 0;
  }

  // 66
  if (LVal()) {
    fallBack(IN, scanTmp);
  } else {
    return 0;
  }

  // 67 68
  if (!(tok == num_INT || tok == num_FLOAT)) {
    error();
    return 1;
  }
  advance();

  return 0;
}

int UnaryExp() {
  long scanTmp = checkPoint();

  // 70 || 71
  if (!(tok == Y_ID)) {
    fallBack(IN, scanTmp);
  } else {
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
        return 0;
      }

      // 71
      if (CallParams()) {
        error();
        return 1;
      }

      if (tok != Y_RPAR) {
        error();
        return 1;
      }
      advance();

      return 0;
    }
  }

  // 69
  if (PrimaryExp()) {
    fallBack(IN, scanTmp);
  } else {
    return 0;
  }

  // 72 73 74
  if (!(tok == Y_ADD || tok == Y_SUB || tok == Y_NOT)) {
    error();
    return 1;
  }
  advance();

  if (UnaryExp()) {
    error();
    return 1;
  }
  return 0;
}

int MulExp() {
  // 77
  if (UnaryExp()) {
    error();
    return 1;
  }

  long scanTmp = checkPoint();
  if (!(tok == Y_MUL || tok == Y_DIV || tok == Y_MODULO)) {
    fallBack(IN, scanTmp);
    return 0;
  }
  advance();

  if (MulExp()) {
    error();
    return 1;
  }

  return 0;
}

int AddExp() {
  if (MulExp()) {
    error();
    return 1;
  }

  long scanTmp = checkPoint();
  if (!(tok == Y_ADD || tok == Y_SUB)) {
    fallBack(IN, scanTmp);
    return 0;
  }
  advance();

  if (AddExp()) {
    error();
    return 1;
  }

  return 0;
}

int ConstExp() {
  if (AddExp()) {
    error();
    return 1;
  }
  return 0;
}

int Type() {
  if (tok == Y_INT) {
    advance();
    return 0;
  }
  if (tok == Y_FLOAT) {
    advance();
    return 0;
  }
  if (tok == Y_VOID) {
    advance();
    return 0;
  }

  error();
  return 1;
}

int main() {
  IN = fopen("./dataIn.txt", "r");
  yyset_in(IN);
  advance();
  int res = UnaryExp();

  if (res) {
    printf("ERROR");
  } else {
    printf("No ERROR\n");
    printf("stop at token: %s", yytext);
  }
}
