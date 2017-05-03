#ifndef _GLOBAL_VAR_DECL_STMT
#define _GLOBAL_VAR_DECL_STMT

#include <string>
#include "compileUnit.h"
#include "types.h"
#include "exprCode.h"
#include "constExpr.h"

namespace codeGen {

  class GlobalDecl: public CompileUnit {
    Type* variableType;
    std::string varName;
    ConstExprBase* initialValue;
    
  public:
    GlobalDecl(Type *variableType, string varName) :
    varName(varName), variableType(variableType) {
      initialValue = NULL;
    }
    
    GlobalDecl(Type *variableType, string varName, ConstExprBase *init) :
    varName(varName), variableType(variableType), initialValue(init) {
    }
    
    ~GlobalDecl();
    
    virtual void build();
    virtual void print();
  };

  class ExternDecl: public CompileUnit {
    std::string varName;
    Type* variableType;
    
  public:
    ExternDecl(Type *variableType, string varName) :
    varName(varName), variableType(variableType) {
    }
    
    ~ExternDecl();
    
    virtual void build();
    virtual void print();
  };

}

#endif