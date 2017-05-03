#include <iostream>
#include "assignExpr.h"
#include "addExpr.h"
#include "subtractionExpr.h"

using namespace codeGen;

AssignExpr::~AssignExpr() {
  delete lhs;
  delete rhs;
}

cval AssignExpr::build() {
  cval rval = getRVal();
  cval lval = lhs->buildLHS();
  
  if (!lval.val->getType()->isPointerTy()) {
    cerr << "Var assignment possible for pointer types only. Obtained value: ";
    lval.val->dump();
    cerr << endl;
    exit(1);
  } else {
    promoteValToType(rval, lval.val->getType()->getPointerElementType(), lval.is_signed);
    Builder->CreateStore(rval.val, lval.val);
  }
  return rval;
}

cval AssignExpr::getRVal() {
  return rhs->build();
}

cval CombinedAssignmentExprBase::getRVal() {
  BinaryExpr *combinedRhs = getCombinedRhs();  
  cval rval = combinedRhs->build();
  combinedRhs->setLeftExpr(NULL); // to avoid double delete
  combinedRhs->setRightExpr(NULL); // to avoid double delete
  delete combinedRhs;
  return rval;
}

BinaryExpr* PlusEqualExpr::getCombinedRhs() {
  return new AddExpr(rhs, lhs);  
}

BinaryExpr* MinusEqualExpr::getCombinedRhs() {
  return new SubtractionExpr(lhs, rhs);  
}

void AssignExpr::print() {
  lhs->print();
  std::cout << " = ";
  rhs->print();
}

void PlusEqualExpr::print() {
  lhs->print();
  std::cout << " += ";
  rhs->print();
}

void MinusEqualExpr::print() {
  lhs->print();
  std::cout << " -= ";
  rhs->print();
}

