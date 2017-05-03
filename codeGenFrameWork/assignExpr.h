#ifndef codeGenFrameWork_assignExpr_h
#define codeGenFrameWork_assignExpr_h

#include "exprCode.h"
#include "binaryExpr.h"

namespace codeGen {
  
  class AssignExpr : public ExprCode {
  protected:
    ExprCode* lhs;
    ExprCode* rhs;
    
  public:
    AssignExpr(ExprCode *lhs, ExprCode *rhs) :
    lhs(lhs), rhs(rhs) {        
    }
    ~AssignExpr();
    
    virtual cval build();
    virtual void print();
    virtual cval getRVal();
  };
  
  class CombinedAssignmentExprBase: public AssignExpr {
  public:
    CombinedAssignmentExprBase(ExprCode *lhs, ExprCode *rhs) :
    AssignExpr(lhs, rhs) {        
    }
    
    virtual BinaryExpr* getCombinedRhs() = 0;
    virtual cval getRVal();
  };

  class PlusEqualExpr : public CombinedAssignmentExprBase {
  public:
    PlusEqualExpr(ExprCode *lhs, ExprCode *rhs) :
    CombinedAssignmentExprBase(lhs, rhs) {        
    }
    
    virtual BinaryExpr* getCombinedRhs();
    virtual void print();
  };

  class MinusEqualExpr : public CombinedAssignmentExprBase {
  public:
    MinusEqualExpr(ExprCode *lhs, ExprCode *rhs) :
    CombinedAssignmentExprBase(lhs, rhs) {        
    }
    
    virtual BinaryExpr* getCombinedRhs();
    virtual void print();
  };
  
}

#endif
