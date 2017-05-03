#include "exprCode.h"

using namespace codeGen;

ExprCode::~ExprCode() {}

cval ExprCode::buildLHS() {
  std::cerr << "This expr is not allowed at LHS." << std::endl;
  exit(1);
}

