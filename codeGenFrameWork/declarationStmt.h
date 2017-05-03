#ifndef _DECLARATION_STMT
#define _DECLARATION_STMT

#include "expressions.h"
#include "stmtCode.h"
#include "types.h"

namespace codeGen {
  
  class VarDecl : public StmtCode {
    string varName;
    Type* type;
    StmtCode *init;
    bool restricted;
  public:
    VarDecl(Type* type, string varName, bool isRestricted=false);
    
    VarDecl(Type* type, string varName, ExprCode *init, bool isRestricted=false);
    
    ~VarDecl();
    
    virtual void build();
    virtual void print(int indent);
  };

  // A local var declaration like 
  // int x[3] = {1, 2, 3}; 
  class ListDecl : public StmtCode {
    string varName;
    ArrayType* type;
    ListExpr *init;
  public:
    ListDecl(ArrayType* type, string varName, ListExpr *init) :
    varName(varName), type(type), init(init) {        
    }
    
    ~ListDecl();
    
    virtual void build();
    virtual void print(int indent);
  };

}

#endif
