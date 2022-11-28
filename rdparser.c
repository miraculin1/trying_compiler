#include "rdparser.h"
#include "lex.h"
#include "token.h"
#include <stdio.h>
#include <stdlib.h>
// DEBUG
#include <dirent.h>
#include <string.h>

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

void error() {
  /* printf("%s", yytext); */
}

static void fallBack(FILE *IN, long tar) {
  // from begin;
  fseek(IN, tar, SEEK_SET);
  yyrestart(IN);
  scaned = tar;
  advance();
}

int CompUnit() {
  long scanTmp = checkPoint();
  if (Decl()) {
    fallBack(IN, scanTmp);
  } else {
    long scanTmp1 = checkPoint();
    if (CompUnit()) {
      fallBack(IN, scanTmp1);
      return 0;
    }
    return 0;
  }

  if (FuncDef()) {
    return 1;
  } else {
    long scanTmp2 = checkPoint();
    if (CompUnit()) {
      fallBack(IN, scanTmp2);
      return 0;
    }
    return 0;
  }
}

int VarDecl() {
  if (Type()) {
    return 1;
  }

  if (VarDef()) {
    return 1;
  }

  long scanTmp = checkPoint();
  if (VarDecls()) {
    fallBack(IN, scanTmp);
  } else {
    if (tok != Y_SEMICOLON) {
      return 1;
    }
    advance();
    return 0;
  }

  if (tok != Y_SEMICOLON) {
    return 1;
  }
  advance();
  return 0;
}

int VarDecls() {
  if (tok != Y_COMMA) {
    return 1;
  }
  advance();

  if (VarDef()) {
    return 1;
  }

  long scanTmp = checkPoint();
  if (VarDecls()) {
    fallBack(IN, scanTmp);
    return 0;
  }

  return 0;
}

int VarDef() {
  if (tok != Y_ID) {
    return 1;
  }
  advance();

  long scanTmp = checkPoint();
  if (tok != Y_ASSIGN) {
    fallBack(IN, scanTmp);
  } else {
    advance();
    if (InitVal()) {
      return 1;
    }
    return 0;
  }

  long scanTmp1 = checkPoint();
  if (ConstExps()) {
    fallBack(IN, scanTmp1);
    return 0;
  }

  long scanTmp2 = checkPoint();
  if (tok != Y_ASSIGN) {
    fallBack(IN, scanTmp2);
    return 0;
  } else {
    advance();
    if (InitVal()) {
      return 1;
    }
    return 0;
  }
}

int InitVal() {
  long scanTmp = checkPoint();
  if (tok != Y_LBRACKET) {
    fallBack(IN, scanTmp);
  } else {
    advance();
    long scanTmp1 = checkPoint();
    if (tok != Y_RBRACKET) {
      fallBack(IN, scanTmp1);
    } else {
      advance();
      return 0;
    }

    if (InitVal()) {
      return 1;
    }

    long scanTmp2 = checkPoint();
    if (tok != Y_RBRACKET) {
      fallBack(IN, scanTmp2);
    } else {
      advance();
      return 0;
    }

    if (InitVals()) {
      return 1;
    }

    if (tok != Y_RBRACKET) {
      return 1;
    } else {
      advance();
      return 0;
    }
  }

  if (Exp()) {
    return 1;
  }
  return 0;
}

int InitVals() {
  if (tok != Y_COMMA) {
    return 1;
  }
  advance();

  if (InitVal()) {
    return 1;
  }

  long scanTmp = checkPoint();
  if (InitVals()) {
    fallBack(IN, scanTmp);
    return 0;
  }
  return 0;
}

int FuncDef() {
  if (Type()) {
    return 1;
  }
  if (tok != Y_ID) {
    return 1;
  }
  advance();
  if (tok != Y_LPAR) {
    return 1;
  }
  advance();

  long scanTmp = checkPoint();
  if (tok != Y_RPAR) {
    fallBack(IN, scanTmp);
  } else {
    advance();
    if (Block()) {
      return 1;
    }
    return 0;
  }

  if (FuncParams()) {
    return 1;
  }

  if (tok != Y_RPAR) {
    return 1;
  } else {
    advance();
    if (Block()) {
      return 1;
    }
    return 0;
  }
}

int FuncParams() {
  if (FuncParam()) {
    return 1;
  }

  long scanTmp = checkPoint();
  if (tok != Y_COMMA) {
    fallBack(IN, scanTmp);
    return 0;
  } else {
    advance();
    if (FuncParams()) {
      return 1;
    }
    return 0;
  }
}

int FuncParam() {
  if (Type()) {
    return 1;
  }
  if (tok != Y_ID) {
    return 1;
  }
  advance();

  long scanTmp = checkPoint();
  if (tok != Y_LSQUARE) {
    fallBack(IN, scanTmp);
    return 0;
  } else {
    advance();
    if (tok != Y_RSQUARE) {
      return 1;
    } else {
      advance();
      long scanTmp1 = checkPoint();
      if (ArraySubscripts()) {
        fallBack(IN, scanTmp1);
        return 0;
      }
      return 0;
    }
  }

  long scanTmp2 = checkPoint();
  if (ArraySubscripts()) {
    fallBack(IN, scanTmp2);
    return 0;
  }
  return 0;
}

int ConstInitvals() {
  if (tok != Y_COMMA) {
    return 1;
  }
  advance();

  if (ConstInitval()) {
    return 1;
  }

  long scanTmp = checkPoint();
  if (ConstInitvals()) {
    fallBack(IN, scanTmp);
    return 0;
  } else {
    return 0;
  }
}

int ConstInitval() {
  long scanTmp = checkPoint();
  if (ConstExp()) {
    fallBack(IN, scanTmp);
  } else {
    return 0;
  }

  if (tok != Y_LBRACKET) {
    return 1;
  } else {
    advance();
    long scanTmp1 = checkPoint();
    if (tok != Y_RBRACKET) {
      fallBack(IN, scanTmp1);
      if (ConstInitval()) {
        return 1;
      } else {
        long scanTmp2 = checkPoint();
        if (tok != Y_RBRACKET) {
          fallBack(IN, scanTmp2);
          if (ConstInitvals()) {
            return 1;
          } else {
            if (tok != Y_RBRACKET) {
              return 1;
            } else {
              advance();
              return 0;
            }
          }
        } else {
          advance();
          return 0;
        }
      }
    } else {
      advance();
      return 0;
    }
  }
}

int ConstExps() {
  if (tok != Y_LSQUARE) {
    return 1;
  }
  advance();

  if (ConstExp()) {
    return 1;
  }

  if (tok != Y_RSQUARE) {
    return 1;
  }
  advance();

  long scanTmp = checkPoint();
  if (ConstExps()) {
    fallBack(IN, scanTmp);
    return 0;
  }
  return 0;
}

int ConstDef() {
  if (tok != Y_ID) {
    return 1;
  }
  advance();

  long scanTmp = checkPoint();
  if (tok != Y_ASSIGN) {
    fallBack(IN, scanTmp);
  } else {
    advance();
    if (ConstInitval()) {
      return 1;
    }

    return 0;
  }

  if (ConstExps()) {
    return 1;
  } else {
    if (tok != Y_ASSIGN) {
      return 1;
    }
    advance();
    if (ConstInitval()) {
      return 1;
    }

    return 0;
  }
}

int ConstDefs() {
  if (ConstDef()) {
    return 1;
  }

  if (tok != Y_COMMA) {
    return 1;
  }
  advance();

  long scanTmp = checkPoint();
  if (ConstDefs()) {
    fallBack(IN, scanTmp);
  } else {
    return 0;
  }

  if (ConstDef()) {
    return 1;
  } else {
    return 0;
  }
}
int ConstDecl() {
  if (tok != Y_CONST) {
    return 1;
  }
  advance();

  if (Type()) {
    return 1;
  }

  long scanTmp = checkPoint();
  if (ConstDefs()) {
    fallBack(IN, scanTmp);
  } else {
    if (tok != Y_SEMICOLON) {
      return 1;
    }
    advance();
    return 0;
  }

  if (ConstDef()) {
    return 1;
  } else {
    if (tok != Y_SEMICOLON) {
      return 1;
    }
    advance();
    return 0;
  }
}

int Decl() {
  long scanTmp = checkPoint();
  if (ConstDecl()) {
    fallBack(IN, scanTmp);
  } else {
    return 0;
  }

  if (VarDecl()) {
    return 1;
  } else {
    return 0;
  }
}

int BlockItem() {
  long scanTmp = checkPoint();
  if (Decl()) {
    fallBack(IN, scanTmp);
  } else {
    return 0;
  }

  if (Stmt()) {
    error();
    return 1;
  }

  return 0;
}

int BlockItems() {
  if (BlockItem()) {
    error();
    return 1;
  }

  long scanTmp = checkPoint();
  if (BlockItems()) {
    fallBack(IN, scanTmp);
    return 0;
  }
  return 0;
}

int Block() {
  if (tok != Y_LBRACKET) {
    error();
    return 1;
  }
  advance();

  long scanTmp = checkPoint();
  if (BlockItems()) {
    fallBack(IN, scanTmp);
  }
  if (tok != Y_RBRACKET) {
    error();
    return 1;
  }
  advance();

  return 0;
}

int Stmt() {
  long scanTmp = checkPoint();
  if (LVal()) {
    fallBack(IN, scanTmp);
  } else {
    if (tok != Y_ASSIGN) {
      fallBack(IN, scanTmp);
    } else {
      advance();
      if (Exp()) {
        error();
        return 1;
      }

      if (tok != Y_SEMICOLON) {
        error();
        return 1;
      }
      advance();
      return 0;
    }
  }

  if (Exp()) {
    fallBack(IN, scanTmp);
  } else {
    if (tok != Y_SEMICOLON) {
      error();
      return 1;
    }
    advance();
    return 0;
  }

  if (tok != Y_SEMICOLON) {
    fallBack(IN, scanTmp);
  } else {
    advance();
    return 0;
  }

  if (Block()) {
    fallBack(IN, scanTmp);
  } else {
    return 0;
  }

  if (tok != Y_WHILE) {
    fallBack(IN, scanTmp);
  } else {
    advance();
    if (tok != Y_LPAR) {
      error();
      return 1;
    }
    advance();

    if (LOrExp()) {
      error();
      return 1;
    }

    if (tok != Y_RPAR) {
      error();
      return 1;
    }
    advance();

    if (Stmt()) {
      error();
      return 1;
    }

    return 0;
  }

  if (tok != Y_IF) {
    fallBack(IN, scanTmp);
  } else {
    advance();
    if (tok != Y_LPAR) {
      error();
      return 1;
    }
    advance();

    if (LOrExp()) {
      error();
      return 1;
    }

    if (tok != Y_RPAR) {
      error();
      return 1;
    }
    advance();

    if (Stmt()) {
      error();
      return 1;
    }

    long scanTmp1 = checkPoint();
    if (tok != Y_ELSE) {
      fallBack(IN, scanTmp1);
    } else {
      advance();
      if (Stmt()) {
        error();
        return 1;
      }
    }

    return 0;
  }

  if (tok != Y_RETURN) {
    fallBack(IN, scanTmp);
  } else {
    advance();
    long scanTmp2 = checkPoint();
    if (Exp()) {
      fallBack(IN, scanTmp2);
    }
    if (tok != Y_SEMICOLON) {
      error();
      return 1;
    }
    advance();

    return 0;
  }

  if (!(tok == Y_BREAK || tok == Y_CONTINUE)) {
    error();
    return 1;
  } else {
    advance();
    if (tok != Y_SEMICOLON) {
      error();
      return 1;
    }
    advance();
    return 0;
  }
}

int RelExp() {
  if (AddExp()) {
    error();
    return 1;
  }

  long scanTmp = checkPoint();
  if (!(tok == Y_LESS || tok == Y_GREAT || tok == Y_LESSEQ ||
        tok == Y_GREATEQ)) {
    fallBack(IN, scanTmp);
    return 0;
  }
  advance();

  if (RelExp()) {
    error();
    return 1;
  }

  return 0;
}

int EqExp() {
  if (RelExp()) {
    error();
    return 1;
  }

  long scanTmp = checkPoint();
  if (!(tok == Y_EQ || tok == Y_NOTEQ)) {
    fallBack(IN, scanTmp);
    return 0;
  }
  advance();

  if (EqExp()) {
    error();
    return 1;
  }

  return 0;
}

int LAndExp() {
  if (EqExp()) {
    error();
    return 1;
  }

  long scanTmp = checkPoint();
  if (tok != Y_AND) {
    fallBack(IN, scanTmp);
    return 0;
  }
  advance();

  if (LAndExp()) {
    error();
    return 1;
  }

  return 0;
}

int LOrExp() {
  if (LAndExp()) {
    error();
    return 1;
  }

  long scanTmp = checkPoint();
  if (tok != Y_OR) {
    fallBack(IN, scanTmp);
    return 0;
  }
  advance();

  if (LOrExp()) {
    error();
    return 1;
  }

  return 0;
}

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

int updFile(char *file, struct dirent *dir, DIR *d) {

  while (1) {
    if (!(dir = readdir(d))) {
      return 0;
    }
    if (!strcmp(dir->d_name, ".")) {
      continue;
    }
    if (!strcmp(dir->d_name, "..")) {
      continue;
    }
    if (!(strcmp(dir->d_name + strlen(dir->d_name) - 3, ".in"))) {
      continue;
    }
    break;
  }

  sprintf(file, "./ref/test_cases/%s", dir->d_name);
  return 1;
}

/* #define ONE */

#ifndef ONE
int main() {
  DIR *d = opendir("ref/test_cases");
  struct dirent *dir = readdir(d);
  char file[100];

  while (updFile(file, dir, d)) {
    scaned = 0;
    printf("===============\n");
    printf("%s:\n", file);
    IN = fopen(file, "r");
    yyset_in(IN);
    advance();
    int res = CompUnit();
    if (res) {
      printf("ERROR\n");
      printf("stop at token: %s\n", yytext);
      printf("stop at pos: %ld\n", scaned);
    } else {
      printf("Ok\n");
      printf("stop at token: %s\n", yytext);
      printf("stop at pos: %ld\n", scaned);
    }
  }

  closedir(d);
}
#endif

#ifdef ONE
int main() {
  IN = fopen("./dataIn.sy", "r");
  /* IN = fopen("./ref/test_cases/66_exgcd.sy", "r"); */
  yyset_in(IN);
  advance();
  int res = CompUnit();

  if (res) {
    printf("ERROR\n");
    printf("stop at: ");
    for (int i = 0; i < 10; i++) {
      printf("%s", yytext);
      advance();
    }
    printf("stop at pos: %ld", scaned);
  } else {
    printf("OK");
    printf("stop at: ");
    for (int i = 0; i < 10; i++) {
      printf("%s", yytext);
      advance();
    }
    printf("stop at pos: %ld", scaned);
  }
}
#endif
