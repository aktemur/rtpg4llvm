#include "logicalNotExpr.h"

using namespace codeGen;

LogicalNotExpr::~LogicalNotExpr() {
  delete exp;
}

cval LogicalNotExpr::build() {
  cval val = exp->build();
  
  if (val.val->getType()->isIntegerTy()) {
    val.val = Builder->CreateNot(val.val);
  } else {
    std::cout << "LogicalNot works for int types only." << endl;
    exit(1);
  }
  
  return val;
}

void LogicalNotExpr::print() {
  std::cout << "!(";
  exp->print();
  std::cout << ")";
}