#include "relationalExpr.h"
#include <iostream>

using namespace codeGen;

cval RelationalExpr::op(cval lhs, cval rhs)
{
    cval ret;
    ret.is_signed = 0;
    
    promote(lhs, rhs);
    
    const llvm::Type* Ty1 = lhs.val->getType();
    
    if (Ty1->isFloatingPointTy()) {        
        ret.val = this->fpOp(lhs.val, rhs.val);        
    } else if (Ty1->isIntegerTy()) {        
        ret.val = this->integerOp(lhs.val, rhs.val, lhs.is_signed);
    } else {
        std::cerr << "Incompatible types in relational expr: " 
             << Ty1->getTypeID() << ", " << rhs.val->getType()->getTypeID() << endl;
        exit(1);
    }
    return ret;
}