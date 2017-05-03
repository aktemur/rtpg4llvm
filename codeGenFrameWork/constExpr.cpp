#include "constExpr.h"

using namespace codeGen;

cval ConstExprBase::buildForGlobal(llvm::Type *toType) {
  cval val = build();
  promoteValToType(val, toType, true);
  return val;
}    

namespace codeGen {

template<>
cval ConstExpr<int>::con() {
   return _make_signed(llvm::ConstantInt::get(*C, llvm::APInt(8 * sizeof(int), val, true)));
}

template<>
cval ConstExpr<bool>::con() {
  return _make_unsigned(llvm::ConstantInt::get(*C, llvm::APInt(1, val ? 1 : 0, true)));
}

template<>
cval ConstExpr<double>::con() {
   return _make_signed(llvm::ConstantFP::get(*C, llvm::APFloat(val)));
}

template<>
void ConstExpr<int>::print() {
  std::cout << val;
}

template<>
void ConstExpr<double>::print() {
  std::cout << val;
}

template<>
void ConstExpr<bool>::print() {
  std::cout << val;
}

cval NullExpr::build() {
  return _make_unsigned(llvm::Constant::getNullValue(llvm::Type::getIntNTy(*C, 8 * sizeof(int))));
}

}
