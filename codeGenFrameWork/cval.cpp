#include "cval.h"

using namespace codeGen;

cval codeGen::_make_signed(llvm::Value *val) {
   cval r;
   r.val = val;
   r.is_signed = true;
   return r;
}

cval codeGen::_make_unsigned(llvm::Value *val) {
   cval r;
   r.val = val;
   r.is_signed = false;
   return r;
}

cval codeGen::make_cval(bool is_signed, llvm::Value *val) {
   cval r;
   r.val = val;
   r.is_signed = is_signed;
   return r;
}