#include "rdparser.h"
#include "lex.h"
#include "token.h"
#include <asm-generic/errno-base.h>
#include <stdio.h>

/* we define return 1 as problem
 * explicit "return;"
 */
// tok alway point to next "to analyse token"
int tok;
FILE *IN;

static void advance() { tok = yylex(); }

void error() { printf("error"); }

// TODO this module not effective
static void fallBack(FILE *IN, long tar) {
  // from begin;
  fseek(IN, tar, SEEK_SET);
  yyrestart(IN);
  scaned = tar;
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
  FILE *IN = fopen("./dataIn.txt", "r");
  yyset_in(IN);
  advance();

  Type();
}
