#include "derefExpr.h"

using namespace codeGen;

DerefExpr::~DerefExpr() {
  delete exp;
}

cval DerefExpr::build() {    
   cval result = exp->build();
   result.val = Builder->CreateLoad(result.val);
   
   return result;
}

cval DerefExpr::buildLHS() {
   cval lval = exp->buildLHS();
   lval.val = Builder->CreateLoad(lval.val);
   return lval;
}

void DerefExpr::print() {
  std::cout << "*(";
  exp->print();
  std::cout << ')';  
}