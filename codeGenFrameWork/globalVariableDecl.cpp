#include "globalVariableDecl.h"
#include "constExpr.h"
#include "llvm/IR/GlobalVariable.h"

using namespace codeGen;

GlobalDecl::~GlobalDecl() {
  delete variableType;
  delete initialValue;
}

ExternDecl::~ExternDecl() {
  delete variableType;
}

void GlobalDecl::build(){
   llvm::GlobalVariable* pGv;
   llvm::Constant *constVal;
  
   if(initialValue != NULL) {
     llvm::Type *cType = variableType->getLLVMTypeForGlobal();
     cval initVal = initialValue->buildForGlobal(cType);
     constVal = static_cast<llvm::Constant*>(initVal.val);     
   } else {
     constVal = variableType->getLLVMDefaultValue();
   }
  
   pGv = new llvm::GlobalVariable(*TheModule,
                            variableType->getLLVMTypeForGlobal(),
                            false, // isConstant?
                            (initialValue == NULL) ? llvm::GlobalValue::CommonLinkage : llvm::GlobalValue::ExternalLinkage,
                            constVal,
                            varName);
  
   cval gVal = _make_signed(pGv);
   env->put(varName, gVal);
}

void ExternDecl::build(){
  llvm::GlobalVariable* pGv;
  
  pGv = new llvm::GlobalVariable(*TheModule,
                                 variableType->getLLVMTypeForGlobal(),
                                 false,
                                 llvm::GlobalValue::ExternalLinkage,
                                 0,
                                 varName);
  
  cval gVal = _make_signed(pGv);
  env->put(varName, gVal);
}

void GlobalDecl::print() {
  variableType->print();
  std::cout << " " << varName;
  if(initialValue != NULL) {
    std::cout << " = ";
    initialValue->print();    
  }
  std::cout << ";\n\n";
}

void ExternDecl::print() {
  std::cout << "extern ";
  variableType->print();
  std::cout << " " << varName << ";\n\n";
}
