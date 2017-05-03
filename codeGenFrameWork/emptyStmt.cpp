#include "emptyStmt.h"

using namespace codeGen;

void EmptyStmt::build() { 
}

void EmptyStmt::print(int level) { 
  indent(level);
  std::cout << " // EMPTY\n";
}

