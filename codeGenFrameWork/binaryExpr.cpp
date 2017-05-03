#include "binaryExpr.h"
#include <iostream>

using namespace codeGen;

BinaryExpr::~BinaryExpr() {
  delete left;
  delete right;
}

cval BinaryExpr::build() {
   cval lval = left->build();
   cval rval = right->build();
   return this->op(lval, rval);
}

void BinaryExpr::print() {
  std::cout << "(";
  left->print();
  printOp();
  right->print();
  std::cout << ")";
}

void BinaryExpr::setLeftExpr(ExprCode *e) {
  left = e;
}

void BinaryExpr::setRightExpr(ExprCode *e) {
  right = e;
}