#include "logicalExpr.h"
#include <iostream>

using namespace codeGen;

cval LogicalExpr::op(cval lhs, cval rhs){
   cval ret;
   ret.is_signed = 0;
   
   promote(lhs, rhs);
   
   const llvm::Type* Ty1 = lhs.val->getType();
   
   if (Ty1->isIntegerTy()) {
      ret.val = this->integerOp(lhs.val, rhs.val);
   } else {
      std::cerr << "Incompatible types in logical expr: "
      << Ty1->getTypeID() << ", " << rhs.val->getType()->getTypeID() << endl;
      exit(1);
   }
   return ret;
}
