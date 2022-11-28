%{
  #include "token.h"
  #include <stdio.h>
  #include <regex.h>
  /* enum yytokentype { */ /* VOID      = 258, */
    /* NUM       = 259, */
    /* HEX       = 260, */
    /* FLOAT     = 261, */
    /* INT       = 262, */
    /* ID        = 263, */
    /* RET       = 264, */
    /* CNTNIUE   = 265, */
    /* WHILE     = 266, */
    /* IF        = 267, */
    /* APLL      = 268, */
    /* ADD       = 269, */
    /* SUB       = 270, */
    /* MUTI      = 271, */
    /* DIVI      = 272, */
    /* MOD       = 273, */
    /* EQUE      = 274, */
    /* NEQUE     = 275, */
    /* BIG       = 276, */
    /* SMALL     = 277, */
    /* EBIG      = 277, */
    /* ESMALL    = 278, */
    /* COM       = 279, */
    /* AND       = 280, */
    /* OR        = 281, */
    /* LPAR      = 282, */
    /* RPAR      = 283, */
    /* BLPAR     = 284, */
    /* BRPAR     = 285, */
    /* EOS       = 286, */
    /* EOL       = 287, */
  /* }; */

  _YYLVAL yylval;
  extern FILE *yyin;

  // refer to the macthed ones
  long scaned = 0;
  #define YY_USER_ACTION scaned += yyleng;
%}
%x C_COMMENT


%%
"int" { return Y_INT; }
"float" { return Y_FLOAT; }
"void" { return Y_VOID; }
"const" { return Y_CONST; }
"if" { return Y_IF; }
"else" { return Y_ELSE; }
"while" { return Y_WHILE; }
"break" { return Y_BREAK; }
"continue" { return Y_CONTINUE; }
"return" { return Y_RETURN; }
"+" { return Y_ADD; }
"-" { return Y_SUB; }
"*" { return Y_MUL; }
"/" { return Y_DIV; }
"%" { return Y_MODULO; }
"<" { return Y_LESS; }
"<=" { return Y_LESSEQ; }
">" { return Y_GREAT; }
">=" { return Y_GREATEQ; }
"!=" { return Y_NOTEQ; }
"==" { return Y_EQ; }
"!" { return Y_NOT; }
"&&" { return Y_AND; }
"||" { return Y_OR; }
"=" { return Y_ASSIGN; }
"(" { return Y_LPAR; }
")" { return Y_RPAR; }
"{" { return Y_LBRACKET; }
"}" { return Y_RBRACKET; }
"[" { return Y_LSQUARE; }
"]" { return Y_RSQUARE; }
"," { return Y_COMMA; }
";" { return Y_SEMICOLON; }
\/\/.*\n { /* coment */ }
0x[0-9a-fA-F]* { yylval.int_value = strtol(yytext, NULL, 16);
             return num_INT; }
[0-9]+ { yylval.int_value = atoi(yytext);
             return num_INT; }
[0-9]+\.[0-9]* { yylval.float_value = atof(yytext);
                      return num_FLOAT; }
[0-9]*\.[0-9]+ { yylval.float_value = atof(yytext);
                      return num_FLOAT; }
[0-9]+[eE][-+]*[0-9]+ { yylval.float_value = strtod(yytext, NULL);
                      return num_FLOAT; }
[0-9]*\.[0-9]+[eE][-+]*[0-9]+ { yylval.float_value = strtod(yytext, NULL);
                      return num_FLOAT; }
0x[0-9]*\.[0-9a-fA-F]+[pP][+-]*[0-9]* { yylval.float_value = strtod(yytext, NULL);
                      return num_FLOAT; }
[_a-zA-Z][0-9a-zA-Z_]* { yylval.id_name = yytext;
                       return Y_ID; }
[ \t] { /* white space */ }
\n {  }
\r\n {  }


"/*"            { BEGIN(C_COMMENT); }
<C_COMMENT>"*/" { BEGIN(INITIAL); }
<C_COMMENT>\n   { }
<C_COMMENT>.    { }
%%

// user func

/* int main() { */
  /* int tok; */
  /* while (tok = yylex()) { */
    /* printf("<"); */
    /* printf("%d", tok); */
    /* printf(","); */
    /* if (tok == Y_ID) { */
      /* printf("%s", yylval.id_name); */
    /* } else if (tok == num_INT) { */
      /* printf("%d", yylval.int_value); */
    /* } else if (tok == num_FLOAT) { */
      /* printf("%f", yylval.float_value); */
      /* printf("%f", yylval.float_value); */
    /* } */
    /* printf(">"); */
    /* printf("\n"); */
  /* } */
/* } */
