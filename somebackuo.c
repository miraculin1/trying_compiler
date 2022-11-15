
/* int Block() { */
  /* if (tok == Y_LBRACKET) { */
    /* advance(); */
  /* } else { */
    /* error(); */
  /* } */
/*  */
  /* long scanTmp = scaned; */
  /* if (BlockItems()) { */
    /* fallBack(IN, scanTmp); */
  /* } */
/*  */
  /* if (tok == Y_RBRACKET) { */
    /* advance(); */
    /* return 0; */
  /* } else { */
    /* error(); */
  /* } */
/*  */
  /* return 1; */
/* } */
/*  */
/* // we need a way to go back */
/* int BlockItems() { */
  /* if (BlockItem()) { */
    /* error(); */
    /* return 1; */
  /* } */
/*  */
  /* long scanTmp = scaned; */
  /* if (BlockItems()) { */
    /* fallBack(IN, scanTmp); */
  /* } */
  /* return 0; */
/* } */
/*  */
/* int BlockItem() { */
  /* if (Decl()) { */
    /* if (Stmt()) { */
      /* return 1; */
    /* } */
  /* } */
  /* return 0; */
/* } */

