#include <stdlib.h>

#include "reader.h"
#include "scanner.h"
#include "parser.h"
#include "error.h"

Token *currentToken; //token vua doc
Token *lookAhead;    //token xem truoc

void scan(void) {
  Token* tmp = currentToken;
  currentToken = lookAhead;
  lookAhead = getValidToken();
  free(tmp);
}

void eat(TokenType tokenType) {
  if (lookAhead->tokenType == tokenType) {
    printToken(lookAhead);
    scan();
  } else missingToken(tokenType, lookAhead->lineNo, lookAhead->colNo);
}

void compileProgram(void) {
  assert("Parsing a Program ....");
  eat(KW_PROGRAM);
  eat(TK_IDENT);
  eat(SB_SEMICOLON);
  compileBlock();
  eat(SB_PERIOD);
  assert("Program parsed!");
} // So do cu phap program, slide 4

void compileBlock(void) {
  assert("Parsing a Block ....");
  if (lookAhead->tokenType == KW_CONST) {
    eat(KW_CONST);
    compileConstDecl();
    compileConstDecls();
    compileBlock2();
  } 
  else compileBlock2();
  assert("Block parsed!");
} // So do cu phap block, TH constant declare; slide 4

void compileBlock2(void) {
  if (lookAhead->tokenType == KW_TYPE) {
    eat(KW_TYPE);
    compileTypeDecl();
    compileTypeDecls();
    compileBlock3();
  } 
  else compileBlock3();
}//Slide 4: TH typedef declare

void compileBlock3(void) {
  if (lookAhead->tokenType == KW_VAR) {
    eat(KW_VAR);
    compileVarDecl();
    compileVarDecls();
    compileBlock4();
  } 
  else compileBlock4();
}//Slide 4: TH var declare

void compileBlock4(void) {
  compileSubDecls();
  compileBlock5();
}//Slide 4: TH procedure + function declare

void compileBlock5(void) {
  eat(KW_BEGIN);
  compileStatements();
  eat(KW_END);
}//Slide 4: TH khoi BEGIN-END

void compileConstDecls(void) {
  // TODO
  switch(lookAhead->tokenType)
    {
    case TK_IDENT:
      compileConstDecl();
      compileConstDecls();
      break;
    case KW_TYPE:
    case KW_VAR:
    case KW_PROCEDURE:
    case KW_FUNCTION:
    case KW_BEGIN:
      break;
      //TH loi xay ra:
    default:
      error(ERR_INVALIDSTATEMENT, lookAhead->lineNo, lookAhead->colNo);
      break;
    }
} // San xuat nhan ra: D -> D; D

void compileConstDecl(void) {
  // TODO
  eat(TK_IDENT);
  eat(SB_EQ);
  compileConstant();
  eat(SB_SEMICOLON);
} //CONST <identifier>=<constant>;

void compileTypeDecls(void) {
  // TODO
  switch(lookAhead->tokenType)
    {
    case TK_IDENT:
      compileTypeDecl();
      compileTypeDecls();
      break;
    case KW_VAR:
    case KW_PROCEDURE:
    case KW_FUNCTION:
    case KW_BEGIN:
      break;
      //TH loi xay ra:
    default:
      error(ERR_INVALIDSTATEMENT, lookAhead->lineNo, lookAhead->colNo);
      break;
    }
}

void compileTypeDecl(void) {
  // TODO
  eat(TK_IDENT);
  eat(SB_EQ);
  compileType();
  eat(SB_SEMICOLON);
} //TYPE <identifier>=<type>;

void compileVarDecls(void) {
  // TODO
  switch(lookAhead->tokenType)
    {
    case TK_IDENT:
      compileVarDecl();
      compileVarDecls();
      break;
    case KW_PROCEDURE:
    case KW_FUNCTION:
    case KW_BEGIN:
      break;
      //TH loi xay ra:
    default:
      error(ERR_INVALIDSTATEMENT, lookAhead->lineNo, lookAhead->colNo);
      break;
    }
}

void compileVarDecl(void) {
  // TODO
  eat(TK_IDENT);
  eat(SB_COLON);
  compileType();
  eat(SB_SEMICOLON);
} //VAR <identifier>:<type>;

void compileSubDecls(void) {
  assert("Parsing subtoutines ....");
  // TODO
  switch(lookAhead->tokenType)
    {
    case KW_PROCEDURE:
      compileProcDecl();
      compileSubDecls();
      break;
    case KW_FUNCTION:
      compileFuncDecl();
      compileSubDecls();
      break;
    case KW_BEGIN:
      break;
      //TH loi xay ra:
    default:
      error(ERR_INVALIDSTATEMENT, lookAhead->lineNo, lookAhead->colNo);
      break;
    }
  assert("Subtoutines parsed ....");
}

void compileFuncDecl(void) {
  assert("Parsing a function ....");
  // TODO
  eat(KW_FUNCTION);
  eat(TK_IDENT);
  compileParams();
  eat(SB_COLON);
  compileBasicType();
  eat(SB_SEMICOLON);
  compileBlock();
  eat(SB_SEMICOLON);
  assert("Function parsed ....");
} //FUNCTION <identifier><paramlist>:<basictype>;

void compileProcDecl(void) {
  assert("Parsing a procedure ....");
  // TODO
  eat(KW_PROCEDURE);
  eat(TK_IDENT);
  compileParams();
  eat(SB_SEMICOLON);
  compileBlock();
  eat(SB_SEMICOLON);
  assert("Procedure parsed ....");
}

void compileUnsignedConstant(void) {
  // TODO
  switch(lookAhead->tokenType)
    {
    case TK_NUMBER:
    case TK_IDENT:
    case TK_CHAR:
      eat(lookAhead->tokenType);
      break;
      //TH loi xay ra:
    default:
      error(ERR_INVALIDSTATEMENT, lookAhead->lineNo, lookAhead->colNo);
      break;
    }
}

void compileConstant(void) {
  // TODO
  switch(lookAhead->tokenType)
    {
    case SB_PLUS:
    case SB_MINUS:
      eat(lookAhead->tokenType);
      compileConstant2(); // Truong hop hang so duong/am
      break;
    case TK_NUMBER:
      compileConstant2(); // Truong hop hang so
      break;
    case TK_IDENT:
      compileConstant2();
      break;
    case TK_CHAR: // Truong hop hang ky tu
      eat(TK_CHAR);
      break;
      //TH loi xay ra:
    default:
      error(ERR_INVALIDSTATEMENT, lookAhead->lineNo, lookAhead->colNo);
      break;
    }
}

void compileConstant2(void) {
  // TODO
  switch(lookAhead->tokenType)
    {
    case TK_NUMBER:
      eat(lookAhead->tokenType);
      break;
    case TK_IDENT:
      eat(lookAhead->tokenType);
      break;
      //TH loi xay ra:
    default:
      error(ERR_INVALIDSTATEMENT, lookAhead->lineNo, lookAhead->colNo);
      break;
    }
} // Nhanh cua Constantst, 

void compileType(void) {
  // TODO
  switch(lookAhead->tokenType)
    {
    case KW_INTEGER:
    case KW_CHAR:
    case TK_IDENT: // Kieu nguoi dung tu dinh nghia
      eat(lookAhead->tokenType);
      break;
    case KW_ARRAY:
      eat(KW_ARRAY);
      eat(SB_LSEL);
      eat(TK_NUMBER);
      eat(SB_RSEL);
      eat(KW_OF);
      compileType(); // Array of array...
      break;
      //TH loi xay ra:
    default:
      error(ERR_INVALIDSTATEMENT, lookAhead->lineNo, lookAhead->colNo);
      break;
    }
}

void compileBasicType(void) {
  // TODO
  switch(lookAhead->tokenType)
    {
    case KW_INTEGER:
    case KW_CHAR:
      eat(lookAhead->tokenType);
      break;
      //TH loi xay ra:
    default:
      error(ERR_INVALIDSTATEMENT, lookAhead->lineNo, lookAhead->colNo);
      break;
    }
}//slide 7

void compileParams(void) {
  // TODO
  switch(lookAhead->tokenType)
    {
    case SB_LPAR:
      eat(SB_LPAR);
      compileParam();
      compileParams2();
      eat(SB_RPAR);
      break;
    case SB_COLON:
    case SB_SEMICOLON:
      break;
      //TH loi xay ra:
    default:
      error(ERR_INVALIDSTATEMENT, lookAhead->lineNo, lookAhead->colNo);
      break;
    }
}

void compileParams2(void) {
  // TODO
  switch(lookAhead->tokenType)
    {
    case SB_SEMICOLON:
      eat(SB_SEMICOLON);
      compileParam();
      compileParams2();
      break;
    case SB_RPAR:
      break;
      //TH loi xay ra:
    default:
      error(ERR_INVALIDSTATEMENT, lookAhead->lineNo, lookAhead->colNo);
      break;
    }
} // Nhan san xuat >> tao ra danh sach tham so

void compileParam(void) {
  // TODO
  switch(lookAhead->tokenType)
    {
    case TK_IDENT:
      eat(TK_IDENT);
      eat(SB_COLON);
      compileBasicType();
      break;
    case KW_VAR:
      eat(KW_VAR);
      eat(TK_IDENT);
      eat(SB_COLON);
      compileBasicType();
      break;
      //TH loi xay ra:
    default:
      error(ERR_INVALIDSTATEMENT, lookAhead->lineNo, lookAhead->colNo);
      break;
    }
}

void compileStatements(void) {
  // TODO
  compileStatement();
  compileStatements2();
}//Dich Statements

void compileStatements2(void) {
  // TODO
  switch(lookAhead->tokenType)
    {
    case SB_SEMICOLON:
      eat(SB_SEMICOLON);
      compileStatement();
      compileStatements2();
      break;
    case KW_END:
      break;
      //TH loi xay ra:
    default:
      error(ERR_INVALIDSTATEMENT, lookAhead->lineNo, lookAhead->colNo);
      break;
    }
}//Xem ket thuc statement

void compileStatement(void) {
  switch (lookAhead->tokenType) {
  case TK_IDENT:
    compileAssignSt();
    break;
  case KW_CALL:
    compileCallSt();
    break;
  case KW_BEGIN:
    compileGroupSt();
    break;
  case KW_IF:
    compileIfSt();
    break;
  case KW_WHILE:
    compileWhileSt();
    break;
  case KW_FOR:
    compileForSt();
    break;
    // EmptySt needs to check FOLLOW tokens
  case SB_SEMICOLON:
  case KW_END:
  case KW_ELSE:
    break;
    // Error occurs
  default:
    error(ERR_INVALIDSTATEMENT, lookAhead->lineNo, lookAhead->colNo);
    break;
  }
}//So do cu phap Statements

void compileAssignSt(void) {
  assert("Parsing an assign statement ....");
  // TODO
  eat(TK_IDENT);
  compileIndexes(); // Chi so mang
  eat(SB_ASSIGN);
  compileExpression();
  assert("Assign statement parsed ....");
}

void compileCallSt(void) {
  assert("Parsing a call statement ....");
  // TODO
  eat(KW_CALL);
  eat(TK_IDENT);
  compileArguments();
  assert("Call statement parsed ....");
}

void compileGroupSt(void) {
  assert("Parsing a group statement ....");
  // TODO
  eat(KW_BEGIN);
  compileStatements();
  eat(KW_END);
  assert("Group statement parsed ....");
}

void compileIfSt(void) {
  assert("Parsing an if statement ....");
  eat(KW_IF);
  compileCondition();
  eat(KW_THEN);
  compileStatement();
  if (lookAhead->tokenType == KW_ELSE) 
    compileElseSt();
  assert("If statement parsed ....");
}

void compileElseSt(void) {
  eat(KW_ELSE);
  compileStatement();
}

void compileWhileSt(void) {
  assert("Parsing a while statement ....");
  // TODO
  eat(KW_WHILE);
  compileCondition();
  eat(KW_DO);
  compileStatement();
  assert("While statement pased ....");
}

void compileForSt(void) {
  assert("Parsing a for statement ....");
  // TODO
  eat(KW_FOR);
  eat(TK_IDENT);
  eat(SB_ASSIGN);
  compileExpression();
  eat(KW_TO);
  compileExpression();
  eat(KW_DO);
  compileStatement();
  assert("For statement parsed ....");
}

void compileArguments(void) {
  // TODO

  switch(lookAhead->tokenType)
    {
    case SB_LPAR:
      eat(SB_LPAR);
      compileExpression();
      compileArguments2();
      eat(SB_RPAR);
      break;

    case SB_TIMES:
    case SB_SLASH:
    case SB_PLUS:
    case SB_MINUS:
    case KW_TO:
    case KW_DO:
    case SB_RPAR:
    case SB_COMMA:
    case SB_EQ:
    case SB_NEQ:    
    case SB_LE:
    case SB_LT:
    case SB_GE:
    case SB_GT:
    case SB_RSEL:
    case SB_SEMICOLON:
    case KW_END:
    case KW_ELSE:
    case KW_THEN:
      break;
    // Error occurs
  default:
    error(ERR_INVALIDARGUMENTS, lookAhead->lineNo, lookAhead->colNo);
    }

}

void compileArguments2(void) {
  // TODO
  while(lookAhead->tokenType == SB_COMMA)
    {
      eat(SB_COMMA);
      compileExpression();
    }
}

void compileCondition(void) {
  // TODO
  compileExpression();
  compileCondition2();
}

void compileCondition2(void) {
  // TODO
  switch(lookAhead->tokenType)
    {
    case SB_EQ:
      eat(SB_EQ);
      break;
    case SB_NEQ:
      eat(SB_NEQ);
      break;
    case SB_LE:
      eat(SB_LE);
      break;
    case SB_LT:
      eat(SB_LT);
      break;
    case SB_GE:
      eat(SB_GE);
      break;
    case SB_GT:
      eat(SB_GT);
      break;
    default:
      error(ERR_INVALIDCOMPARATOR, lookAhead->lineNo, lookAhead->colNo);
    }
  compileExpression();
}

void compileExpression(void) {
  assert("Parsing an expression");
  // TODO
  switch(lookAhead->tokenType)
    {
    case SB_PLUS:
      eat(SB_PLUS);
      compileExpression2();
      break;
    case SB_MINUS:
      eat(SB_MINUS);
      compileExpression2();
      break;
    default:
      compileExpression2();
      break;
    }
  assert("Expression parsed");
}

void compileExpression2(void) {
  // TODO
  compileTerm();
  compileExpression3();
}


void compileExpression3(void) {
  // TODO
  switch(lookAhead->tokenType)
    {
    case SB_PLUS:
      eat(SB_PLUS);
      compileTerm();
      compileExpression3();
      break;
    case SB_MINUS:
      eat(SB_MINUS);
      compileTerm();
      compileExpression3();
      break;
    case KW_TO:
    case KW_DO:
    case SB_RPAR:
    case SB_COMMA:
    case SB_EQ:
    case SB_NEQ:    
    case SB_LE:
    case SB_LT:
    case SB_GE:
    case SB_GT:
    case SB_RSEL:
    case SB_SEMICOLON:
    case KW_END:
    case KW_ELSE:
    case KW_THEN:
      break;
    default:
      error(ERR_INVALIDEXPRESSION, lookAhead->lineNo, lookAhead->colNo);
      break;
    }
}

void compileTerm(void) {
  // TODO
  compileFactor();
  compileTerm2();
}

void compileTerm2(void) {
  // TODO
  switch(lookAhead->tokenType)
    {
    case SB_TIMES:
      eat(SB_TIMES);
      compileFactor();
      compileTerm2();
      break;
    case SB_SLASH:
      eat(SB_SLASH);
      compileFactor();
      compileTerm2();
      break;
    case SB_PLUS:
    case SB_MINUS:
    case KW_TO:
    case KW_DO:
    case SB_RPAR:
    case SB_COMMA:
    case SB_EQ:
    case SB_NEQ:    
    case SB_LE:
    case SB_LT:
    case SB_GE:
    case SB_GT:
    case SB_RSEL:
    case SB_SEMICOLON:
    case KW_END:
    case KW_ELSE:
    case KW_THEN:
      break;
    default:
      error(ERR_INVALIDTERM, lookAhead->lineNo, lookAhead->colNo);
      break;
    }
}

void compileFactor(void) {
  // TODO
  switch(lookAhead->tokenType)
    {
    case TK_NUMBER:
      eat(TK_NUMBER);
      break;
    case TK_CHAR:
      eat(TK_CHAR);
      break;
    case TK_IDENT:
      eat(TK_IDENT);
      switch(lookAhead->tokenType)
      	{
	       case SB_LPAR: // Uu tien phep toan E->(E)
	         compileArguments();
	         break;
	       case SB_LSEL: // Mang
	         compileIndexes();
        	  break;
	       default:
	         break;
	       }
      break;
    case SB_LPAR:
      eat(SB_LPAR);
      compileExpression();
      eat(SB_RPAR);
      break;
    default:
      error(ERR_INVALIDFACTOR, lookAhead->lineNo, lookAhead->colNo);
      break;
    }
}

void compileIndexes(void) {
  // TODO
  while(lookAhead->tokenType == SB_LSEL)
    {
      eat(SB_LSEL);
      compileExpression();
      eat(SB_RSEL);
    }
}

int compile(char *fileName) {
  if (openInputStream(fileName) == IO_ERROR)
    return IO_ERROR;

  currentToken = NULL;
  lookAhead = getValidToken();

  compileProgram();

  free(currentToken);
  free(lookAhead);
  closeInputStream();
  return IO_SUCCESS;

}
