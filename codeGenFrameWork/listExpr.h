#ifndef codeGenFrameWork_listExpr_h
#define codeGenFrameWork_listExpr_h

#include <vector>
#include "constExpr.h"

namespace codeGen {
  
  class ListExpr : public ConstExprBase {
  private:
    vector<ExprCode*> *exps;
    llvm::ArrayType *type;
    llvm::Constant *constArray;
    llvm::Value *assignedVar;
    
  public:
    ListExpr(vector<ExprCode*> *exps) :
     exps(exps) { }
    ~ListExpr();
    
    virtual cval build();
    virtual cval buildForGlobal(llvm::Type *toType);
    void setAssignedVar(llvm::Value *var);
    virtual void print();
  private:
    void setTypeAndConstArray(llvm::Type *elementType);
  };
  
}


#endif
