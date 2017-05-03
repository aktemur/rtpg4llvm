#include "prePostOpExpr.h"
#include "assignExpr.h"
#include "varExpr.h"
#include "addExpr.h"
#include "subtractionExpr.h"
#include "constExpr.h"

using namespace codeGen;

PrePostOpBase::~PrePostOpBase() {
  delete exp;
}

cval PrePostOpBase::build() {
  // Read the current value
  cval lval = exp->buildLHS();
  cval preOpVal;
  preOpVal.val = Builder->CreateLoad(lval.val);
  
  // Perform the addition
  BinaryExpr *rhs = getBinaryExpr();
  cval rval = rhs->build();
  rhs->setLeftExpr(NULL); // To prevent exp from being doubly freed
  delete rhs;
  
  // Write the new value
  Builder->CreateStore(rval.val, lval.val);
  
  // Return the original or the new value
  return choosePreOrPostVal(preOpVal, rval);
}

BinaryExpr* PostIncrExpr::getBinaryExpr() {
  return new AddExpr(exp, new ConstExpr<int>(1));
}

BinaryExpr* PreIncrExpr::getBinaryExpr() {
  return new AddExpr(exp, new ConstExpr<int>(1));
}

BinaryExpr* PostDecrExpr::getBinaryExpr() {
  return new SubtractionExpr(exp, new ConstExpr<int>(1));
}

BinaryExpr* PreDecrExpr::getBinaryExpr() {
  return new SubtractionExpr(exp, new ConstExpr<int>(1));
}

cval& PostIncrExpr::choosePreOrPostVal(cval& preVal, cval& postVal) {
  return preVal;
}

cval& PostDecrExpr::choosePreOrPostVal(cval& preVal, cval& postVal) {
  return preVal;
}

cval& PreIncrExpr::choosePreOrPostVal(cval& preVal, cval& postVal) {
  return postVal;
}

cval& PreDecrExpr::choosePreOrPostVal(cval& preVal, cval& postVal) {
  return postVal;
}

void PostIncrExpr::print() {
  exp->print();
  std::cout << "++";
}

void PostDecrExpr::print() {
  exp->print();
  std::cout << "--";
}

void PreIncrExpr::print() {
  std::cout << "++";
  exp->print();
}

void PreDecrExpr::print() {
  std::cout << "--";
  exp->print();
}
