#include "arithmeticExpr.h"

using namespace codeGen;

cval ArithmeticExpr::op(cval cv1, cval cv2)
{
    const llvm::Type *t1 = cv1.val->getType();
    const llvm::Type *t2 = cv2.val->getType();
    cval r;
    
    r.is_signed = false;
    if (!t1->isPointerTy() && !t2->isPointerTy()) {
        promote(cv1, cv2);
        t1 = cv1.val->getType();
        r.is_signed = cv1.is_signed;
        if (t1->isIntegerTy()) {
            r.val = this->integerOp(cv1.val, cv2.val, r.is_signed);
        } else if (t1->isFloatingPointTy()) {
            r.val = this->fpOp(cv1.val, cv2.val);
        }  else{
            std::cerr << "Unexpected type after promotion" << endl;
            exit(1);
        }
    } else if (t1->isIntegerTy() && t2->isPointerTy()) {
        r.is_signed = cv2.is_signed;
        r.val = this->pointerOp(cv2.val, cv1.val);
    } else if (t1->isPointerTy() && t2->isIntegerTy()) {       
        r.is_signed = cv1.is_signed;
        r.val = this->pointerOp(cv1.val, cv2.val);
    }else {
        cv1.val->dump();
        cv2.val->dump();
        std::cerr << "Incompatible types in arithmetic expr." << endl;
        exit(1);
    }
    return r;
}
