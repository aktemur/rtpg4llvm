#ifndef _CODE_GEN_CONSTRUCTORS_H
#define _CODE_GEN_CONSTRUCTORS_H

#include <string>
#include <vector>
#include "expressions.h"
#include "statements.h"
#include "topLevelConstructs.h"

namespace codeGen {
  enum Operator {
    Minus, Bang, Ampersand, Star, MinusMinus, PlusPlus,
    Plus, Mod, Slash, 
    Equal, PlusEqual, MinusEqual, StarEqual, SlashEqual,
    LogicalAnd, LogicalOr,
    Less, Greater, EqEq, NotEq, LessEq, GreaterEq 
  };
  
  vector<ExprCode*>* mkExprList(int n, ...);
  StmtCode* mkSeqStmt(int n, ...);
  StmtCode* mkScopedStmt(int n, ...);
  vector<FuncParam*>* mkFormals(int n, ...);
  Program* mkProgram(int n, ...);
  ExprCode* mkUnaryOp(Operator op, ExprCode *exp);
  ExprCode* mkUnaryPostOp(Operator op, ExprCode *exp);
  ExprCode* mkBinOp(Operator op, ExprCode *exp1, ExprCode *exp2);
  template<class T>
  ConstExpr<T>* mkConst(T val);
  ListExpr* mkConstList(int elements[], int numElts);
  ListExpr* mkConstList(double elements[], int numElts);
  ListExpr* mkConstList(bool elements[], int numElts);
  
  class Util {
  public:
    template<class T>
    static ListExpr* mkList(T elements[], int numElts);
  };
  
}

#endif
