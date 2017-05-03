#ifndef _IFELSE_STMT
#define _IFELSE_STMT

#include "expressions.h"
#include "stmtCode.h"

namespace codeGen {
  
class IfElseStmt : public StmtCode {
   ExprCode* condition;
   StmtCode* thenBlock;
   StmtCode* elseBlock;

public:
   IfElseStmt(ExprCode* condition, StmtCode* thenBlock, StmtCode* elseBlock) :
   condition(condition), thenBlock(thenBlock), elseBlock(elseBlock) {
   }
   
   IfElseStmt(ExprCode* condition, StmtCode* thenBlock) :
   condition(condition), thenBlock(thenBlock) {
      elseBlock = NULL;
   }

   ~IfElseStmt();
  
   virtual void build();
   virtual void print(int indent);
};

}

#endif