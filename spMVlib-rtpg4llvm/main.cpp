#include "profiler.h"
#include "spMVgen.h"
#include <iostream>

#define ITERS 10000

using namespace spMVgen;

bool DEBUG = false;
bool DUMP_ASM = false;
bool DUMP_OBJECT = false;
bool DUMP_MATRIX = false;

int main(int argc, const char *argv[]) {
  // Usage: spMVgen <matrixName> <splitter> <splitterArgs> 
  //                <specializerOne> <specializerArgs>  
  //                <specializerTwo> <specializerArgs> {-debug|-dump_asm|-dump_object|-dump_matrix}
  // E.g: spMVgen matrices/fidap037 splitAll unfolding stencil 
  // E.g: spMVgen matrices/fidap037 splitAll unfolding stencil -debug
  // E.g: spMVgen matrices/fidap037 splitAll unfolding stencil -dump_asm
  string unrolling("unrolling");
  string oski("OSKI");
  string csrByNZ("CSRbyNZ");
  string unfolding("unfolding");
  string stencil("stencil");
  string diagonal("diagonal");

  string splitAll("splitAll");
  string splitByCount("split_by_count");
  string splitByBand("split_by_band");
  string splitByDenseBand("split_by_dense_band");
  string splitByBlock("split_by_block");

  string debugFlag("-debug");
  string dumpAsmFlag("-dump_asm");
  string dumpObjFlag("-dump_object");
  string dumpMatrixFlag("-dump_matrix");

  string matrixName(argv[1]);
  MMMatrix* mmMatrix = 
    SpMVSpecializer::readMatrixFromFile(matrixName + ".mtx");
  
  char **argptr = (char**)&argv[2];

  // Read the splitter
  Splitter *splitter = NULL;
  if(splitAll.compare(*argptr) == 0) {
    splitter = new TakeAllSplitter(mmMatrix);
  } else if(splitByCount.compare(*argptr) == 0) {
    splitter = new ByCountSplitter(mmMatrix, atoi(*(++argptr)));
  } else if(splitByBand.compare(*argptr) == 0) {
    int lo = atoi(*(++argptr));
    int hi = atoi(*(++argptr));
    splitter = new ByBandSplitter(mmMatrix, lo, hi);
  } else if(splitByDenseBand.compare(*argptr) == 0) {
    int lo = atoi(*(++argptr));
    int hi = atoi(*(++argptr));
    double percent = atof(*(++argptr));
    splitter = new ByDenseBandSplitter(mmMatrix, lo, hi, percent);
  } else if(splitByBlock.compare(*argptr) == 0) {
    int b_r = atoi(*(++argptr));
    int b_c = atoi(*(++argptr));
    splitter = new ByBlockSplitter(mmMatrix, b_r, b_c);
  } else {
    std::cout << "Splitter " << *argptr << " not found.\n";
    exit(1);
  }
  splitter->split();
  argptr++;

  // Read the first specializer
  mmMatrix = splitter->getMatrixOne();
  SpMVSpecializer *methodOne = NULL;
  if(unrolling.compare(*argptr) == 0) {
    methodOne = new Unrolling(mmMatrix, atoi(*(++argptr)));
  } else if(oski.compare(*argptr) == 0) {
    int b_r = atoi(*(++argptr));
    int b_c = atoi(*(++argptr));
    methodOne = new OSKI(mmMatrix, b_r, b_c);
  } else if(unfolding.compare(*argptr) == 0) {
    methodOne = new Unfolding(mmMatrix);
  } else if(csrByNZ.compare(*argptr) == 0) {
    methodOne = new CSRbyNZ(mmMatrix);
  } else if(stencil.compare(*argptr) == 0) {
    methodOne = new Stencil(mmMatrix);
  } else if(diagonal.compare(*argptr) == 0) {
    methodOne = new Diagonal(mmMatrix);
  } else {
    std::cout << "Method " << *argptr << " not found.\n";
    exit(1);
  }
  argptr++;

  // Read the second specializer
  mmMatrix = splitter->getMatrixTwo();
  SpMVSpecializer *methodTwo = NULL;
  if(unrolling.compare(*argptr) == 0) {
    methodTwo = new Unrolling(mmMatrix, atoi(*(++argptr)));
  } else if(oski.compare(*argptr) == 0) {
    int b_r = atoi(*(++argptr));
    int b_c = atoi(*(++argptr));
    methodTwo = new OSKI(mmMatrix, b_r, b_c);
  } else if(unfolding.compare(*argptr) == 0) {
    methodTwo = new Unfolding(mmMatrix);
  } else if(csrByNZ.compare(*argptr) == 0) {
    methodTwo = new CSRbyNZ(mmMatrix);
  } else if(stencil.compare(*argptr) == 0) {
    methodTwo = new Stencil(mmMatrix);
  } else if(diagonal.compare(*argptr) == 0) {
    methodTwo = new Diagonal(mmMatrix);
  } else {
    std::cout << "Method " << *argptr << " not found.\n";
    exit(1);
  }
  argptr++;

  if (argc > argptr - (char**)&argv[0]) { // the optional flag exists
    if (debugFlag.compare(*argptr) == 0)
      DEBUG = true;
    else if (dumpAsmFlag.compare(*argptr) == 0)
      DUMP_ASM = true;
    else if (dumpObjFlag.compare(*argptr) == 0)
      DUMP_OBJECT = true;
    else if (dumpMatrixFlag.compare(*argptr) == 0)
      DUMP_MATRIX = true;
    else {
      std::cout << "Unrecognized flag: " << *argptr << "\n";
      exit(1);
    } 
  }

  // Generate and run the specialized code
  SpMVSpecializer specializer;
  specializer.addMethod(methodOne);
  specializer.addMethod(methodTwo);

  START_TIME_PROFILE(codeGeneration);
  multByMFun fptr = specializer.multByMFunction();
  END_TIME_PROFILE(codeGeneration);

  int n = mmMatrix->n;
  double *v = new double[n];
  double *w = new double[n];
  for(int i = 0; i < n; ++i) {
    v[i] = i;
  }

  if (DUMP_MATRIX) {
    Matrix *matrix = methodOne->getMatrix();
    matrix->print();
    exit(1);
  }

  if (!DEBUG) {
    START_TIME_PROFILE(multByM);
    for (int i=0; i < ITERS; i++) {
      // initialization of w to zero
      for (int j=0; j < n; j++) w[j] = 0;
      fptr(v, w);
    }
    END_TIME_PROFILE(multByM);
    Profiler::print();
  } else {
    for (int j=0; j < n; j++) 
      w[j] = 0;
    fptr(v, w);
    for(int i = 0; i < n; ++i) 
      printf("%g\n", w[i]);
  }

  return 0;
}

