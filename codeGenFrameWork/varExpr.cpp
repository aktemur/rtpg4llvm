#include "varExpr.h"
#include "constExpr.h"

using namespace codeGen;

cval VarExpr::build() {
  cval result = (*env)[name];
  if (result.val->getType()->getContainedType(0)->isArrayTy()) {
    vector<llvm::Value*> idxList;
    cval zero = (ConstExpr<int>(0)).build();
    idxList.push_back(zero.val);
    idxList.push_back(zero.val);
    result.val = Builder->CreateGEP(result.val, idxList);
  } else {
    result.val = Builder->CreateLoad(result.val);
  }
  return result;
}

cval VarExpr::buildLHS() {
  return (*env)[name];
}

string VarExpr::getName() {
  return name;
}

void VarExpr::print() {
  std::cout << name;
}