#include "functionParameter.h"

using namespace codeGen;

FuncParam::~FuncParam() {
  delete type;
}

std::string FuncParam::getName() {
  return name;
}

codeGen::Type* FuncParam::getType() {
  return type;
}

bool FuncParam::isRestrict() {
  return restricted;
}

void FuncParam::print() {
  type->print();
  if(restricted) std::cout << " __restrict__ ";
  std::cout << " " << name;
}
