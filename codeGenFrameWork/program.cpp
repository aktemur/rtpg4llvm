#include "program.h"

using namespace codeGen;

Program::~Program() {
  for (typeof(units->begin())s = units->begin(); s != units->end(); ++s) {
    delete (*s);
  }
  delete units;
}

void Program::build() {
  for (typeof(units->begin())s = units->begin(); s != units->end(); ++s) {
    (*s)->build();
  }
}

void Program::compile() {
  CodeGeneration::compile(units);
}

void Program::print() {
  for (typeof(units->begin())s = units->begin(); s != units->end(); ++s) {
    (*s)->print();
  }
}
