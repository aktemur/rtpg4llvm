#include "declarationStmt.h"
#include "exprStmt.h"
#include "varExpr.h"

using namespace codeGen;

VarDecl::VarDecl(Type* type, string varName, bool isRestricted) :
varName(varName), type(type), restricted(isRestricted) {        
  init = NULL;
}

VarDecl::VarDecl(Type* type, string varName, ExprCode *initExpr, bool isRestricted):
varName(varName), type(type), restricted(isRestricted) {
  init = new ExprStmt(new AssignExpr(new VarExpr(varName),
                                     initExpr));
}

VarDecl::~VarDecl() {
  delete type;
  delete init;
}

ListDecl::~ListDecl() {
  delete type;
  delete init;
}

void VarDecl::build() 
{
    cval ret = _make_signed(Builder->CreateAlloca(type->getLLVMType()));
    env->put(varName, ret);
    ret.val->setName(varName);
  
    if (init != NULL) {
      init->build();
    }
}

void VarDecl::print(int level) {
  indent(level);
  type->print();
  if(restricted) std::cout << " __restrict__";
  std::cout << " " << varName << ";\n";
  if(init != NULL) {
    init->print();
  }
}

void ListDecl::build() {
  cval ret = _make_signed(Builder->CreateAlloca(type->getLLVMType()));
  env->put(varName, ret);
  ret.val->setName(varName);
  
  init->setAssignedVar(ret.val);
  init->build(); // this will do the assignment
}

void ListDecl::print(int level) {
  indent(level);
  type->print();
  std::cout << " " << varName;
  std::cout << " = ";
  init->print();
  std::cout << ";\n";
}

