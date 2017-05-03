#include "unaryMinusExpr.h"

using namespace codeGen;

UnaryMinusExpr::~UnaryMinusExpr() {
  delete exp;
}

cval UnaryMinusExpr::build() {
   cval val = exp->build();
   
   if (val.val->getType()->isIntegerTy()) {
      val.val = Builder->CreateNeg(val.val);
   }else if (val.val->getType()->isFloatingPointTy()) {
      val.val = Builder->CreateFNeg(val.val);
   }else {
      std::cout << "Unary minus works for int and fp types only." << endl;
      exit(1);
   }

   return val;
}

void UnaryMinusExpr::print() {
  std::cout << "-";
  exp->print();
}