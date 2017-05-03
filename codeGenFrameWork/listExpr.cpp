#include "listExpr.h"

using namespace codeGen;

ListExpr::~ListExpr() {
  for (typeof(exps->begin())e = exps->begin(), end = exps->end(); e != end; ++e) {
    delete (*e);
  }
  delete exps;
}

cval ListExpr::buildForGlobal(llvm::Type *declaredType) {
  llvm::Type *elementType = declaredType->getArrayElementType();
  setTypeAndConstArray(elementType);
  
  cval result;
  result.is_signed = false;
  result.val = constArray;
  return result;
}

cval ListExpr::build() {
  setTypeAndConstArray(assignedVar->getType()->getPointerElementType()->getArrayElementType());
  
  llvm::GlobalVariable *pGv = new llvm::GlobalVariable(*TheModule,
                                 type,
                                 true, // isConstant?
                                 llvm::GlobalValue::PrivateLinkage,
                                 constArray,
                                 Builder->GetInsertBlock()->getParent()->getName() + "." + assignedVar->getName());
  pGv->setUnnamedAddr(true);
  
  int numBitsToCopy = type->getNumElements() * type->getContainedType(0)->getPrimitiveSizeInBits();
  Builder->CreateMemCpy(assignedVar, pGv, numBitsToCopy/8, 4);
  
  cval result = _make_signed(pGv);
  return result;
}

void ListExpr::setTypeAndConstArray(llvm::Type *elementType) {
  vector<llvm::Constant*> values;
  
  for (typeof(exps->begin())e = exps->begin(), end = exps->end(); e != end; ++e) {
    cval eVal = (*e)->build();
    promoteValToType(eVal, elementType, true);
    // Parser makes sure that the elements are const values
    llvm::Constant *constVal = static_cast<llvm::Constant*>(eVal.val);
    values.push_back(constVal);
  }
  
  type = llvm::ArrayType::get(values[0]->getType(), values.size());
  constArray = llvm::ConstantArray::get(type, values);
}

void ListExpr::setAssignedVar(llvm::Value *var) {
  assignedVar = var;
}

void ListExpr::print() {
  std::cout << "{";
  typeof(exps->begin()) argIt = exps->begin(), argItEnd = exps->end();
  while(true) {
    (*argIt)->print();
    ++argIt;
    if(argIt != argItEnd) 
      std::cout << ", ";
    else
      break;
  }
  std::cout << "}";  
}
