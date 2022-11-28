#ifndef __RDPARSER_H
#define __RDPARSER_H

#include "ast.h"
#include "lex.h"
#include "token.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

pAst BlockItem();
pAst BlockItems();
pAst Block();
pAst Stmt();
pAst RelExp();
pAst EqExp();
pAst LAndExp();
pAst LOrExp();
pAst CallParams();
pAst ArraySubscripts();
pAst LVal();
pAst Exp();
pAst PrimaryExp();
pAst UnaryExp();
pAst MulExp();
pAst AddExp();
pAst ConstExp();
enum yytokentype Type();
#endif
