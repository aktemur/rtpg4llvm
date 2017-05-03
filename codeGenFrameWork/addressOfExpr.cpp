#include "addressOfExpr.h"
#include <iostream>

using namespace codeGen;

AddressOfExpr::~AddressOfExpr() {
  delete exp;
}

cval AddressOfExpr::build() {
    cval result = exp->buildLHS();
    return result;
}

void AddressOfExpr::print() {
  std::cout << "&(";
  exp->print();
  std::cout << ")";
}