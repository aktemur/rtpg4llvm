#include "breakStmt.h"

using namespace codeGen;

void BreakStmt::build() {
  std::cerr << "Break stmt not supported yet." << std::endl;
  exit(1);
}

void BreakStmt::print(int level) {
  indent(level);
  std::cout << "break;\n";
}