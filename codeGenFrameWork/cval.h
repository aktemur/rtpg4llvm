#ifndef _CVAL
#define _CVAL

#include "llvm/IR/DerivedTypes.h"

namespace codeGen {

// In C we need to distinguish signed and unsigned stuff
struct cval {
   bool is_signed;
   llvm::Value* val;
};

cval _make_signed(llvm::Value *val);

cval _make_unsigned(llvm::Value *val);

cval make_cval(bool is_signed, llvm::Value *val);

}

#endif
