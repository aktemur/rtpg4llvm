#include "continueStmt.h"

using namespace codeGen;

void ContinueStmt::build() {
  std::cerr << "Continue stmt not supported yet." << std::endl;
  exit(1);
}

void ContinueStmt::print(int level) {
  indent(level);
  std::cout << "continue;\n";
}