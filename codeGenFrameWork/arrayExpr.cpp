#include "arrayExpr.h"
#include "addExpr.h"
#include "constExpr.h"

using namespace codeGen;

ArrayExpr::~ArrayExpr() {
  delete index;
  delete base;
}

cval ArrayExpr::buildLHS() {
  cval rval = index->build();
  cval lval = base->buildLHS();

  // Handle pointer to pointers and pointer to arrays differently, 
  // because the base of a GEP must be a pointer.
  if (lval.val->getType()->getContainedType(0)->isPointerTy()) {
    lval.val = Builder->CreateLoad(lval.val);
  }

  const llvm::Type *t1 = lval.val->getType();
  const llvm::Type *t2 = rval.val->getType();
  cval r;  
  r.is_signed = false;
  if (t1->isPointerTy() && t2->isIntegerTy()) {   
    r.is_signed = lval.is_signed;
    vector<llvm::Value*> idxList;
    if (t1->getContainedType(0)->isArrayTy()) {
      cval zero = (ConstExpr<int>(0)).build();
      idxList.push_back(zero.val);
    }
    idxList.push_back(rval.val);    
    r.val = Builder->CreateInBoundsGEP(lval.val, idxList);
    r.val->setName("arrayidx");
  } else {
    std::cerr << "Incompatible types in array expr: ";
    lval.val->dump();
    std::cerr << " vs ";
    rval.val->dump();
    std::cerr << endl;
    exit(1);
  }
  return r;
}

cval ArrayExpr::build() {
  cval r = buildLHS();
  r.val = Builder->CreateLoad(r.val);
  return r;
}

void ArrayExpr::print() {
  base->print();
  std::cout << "[";
  index->print();
  std::cout << "]";
}
