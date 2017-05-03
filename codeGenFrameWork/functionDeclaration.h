#ifndef _FUNC_DECL_STMT
#define _FUNC_DECL_STMT

#include <vector>
#include "compileUnit.h"
#include "functionParameter.h"
#include "stmtCode.h"

namespace codeGen {

class FuncDeclaration : public CompileUnit {
   std::string funcName;
   Type* returnType;
   vector<FuncParam*> *params;
   StmtCode* body;
   
public:
   FuncDeclaration(Type *returnType, std::string funcName, vector<FuncParam*> *params, StmtCode *body) :
      funcName(funcName), returnType(returnType), params(params), body(body) {
   }

   ~FuncDeclaration();
  
   virtual void build();
  
   virtual void print();
};

}
#endif