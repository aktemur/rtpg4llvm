#ifndef SPMVGEN_H
#define SPMVGEN_H

#include "codeGenConstructors.h"
#include "matrix.h"
#include "profiler.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <queue>
#include "llvm/Support/ToolOutputFile.h"
#include "llvm/MC/MCStreamer.h"
#include "llvm/ExecutionEngine/ObjectBuffer.h"

using namespace codeGen;

namespace spMVgen {
  typedef void(*multByMFun)(double*, double*);

  class WeightedStmt {
  private:
    StmtCode *stmt;
    int weight;
  public:
    WeightedStmt(StmtCode *s, int w): stmt(s), weight(w) {}
    ~WeightedStmt() {
      // do nothing
      // We put this destructor here explicitly to emphasize that 
      // this object is not the owner of stmt.
      // The stmt object is destructed by someone else.
    }

    StmtCode *getStmt();
    int getWeight();
  };

  
  typedef struct {
    void* address1;
    void* address2;
  } PointerPair;

  /// A note on memory management:
  /// Destructing SpMVSpecializer objects does not destruct
  /// their mmMatrix objects, which are created separately. 
  /// It is the client's responsibility to destruct every 
  /// SpMVSpecializer and mmMatrix object separately.

  class SpMVSpecializer {
  public:
    // gen method returns a pointer to a multByM method with the
    // signature below:
    // void multByM(double *v, double *w);
    multByMFun gen();
    
    static MMMatrix* readMatrixFromFile(string fileName);

    virtual CompileUnit* getAllDecls();
    
    MMMatrix* getMMMatrix();

    Matrix* getMatrix();

    static int numFunctionsGenerated;

    virtual void setOptimizationPasses();

    // Methods to directly emit assembly code without any quote/unquote stuff, 
    // by-passing the rtpg4llvm compiler
    multByMFun genByEmittingAssembly();
    virtual void dumpAssemblyText(llvm::MCStreamer *Str);
    virtual void dumpAssemblyConstData(llvm::MCStreamer *Str);
    virtual void dumpAssemblyData(llvm::MCStreamer *Str);
    virtual void dumpMatrixValueSetterFunction(llvm::MCStreamer *Str);
    virtual void setMatrixValuesInGeneratedAssembly();
  protected:
    void dumpAssemblyTextHeader(llvm::MCStreamer *Str);
    void dumpAssemblyTextFooter(llvm::MCStreamer *Str);
    void dumpAssemblyConstDataHeader(llvm::MCStreamer *Str);
    void dumpAssemblyDataHeader(llvm::MCStreamer *Str);

    static std::string mainFunctionName;
    static std::string matrixValueSetterFunctionName;
    static std::vector<void*> generatedFunctions;

    MMMatrix *mmMatrix;

    Matrix *matrix;
    
    static int stage;

    SpMVSpecializer() { 
      mmMatrix = NULL;
      matrix = NULL;
    }

    SpMVSpecializer(MMMatrix *mmMatrix): mmMatrix(mmMatrix) { 
      matrix = NULL;
    }

    // Global declarations that will be put outside the generated functions.
    virtual CompileUnit* getGlobalDecls(Matrix *matrix);

    // Declarations to be put at the beginning of each generated
    // function after splitting a long list of generated statements
    virtual StmtCode* getLocalDecls() = 0;

    // The "meat" of the generated multiplication code.
    // The collection of statements returned from this method are split
    // into multiple functions according to the weight of each statement
    // and the WEIGHT_PER_FUNCTION value.
    virtual std::queue<WeightedStmt>* getBody(Matrix *matrix) = 0;
    
    // Convert the MMMatrix to matrix format assumed/required 
    // by the generated code. E.g. a CSR-like format.
    virtual Matrix* getMatrixForGeneration() = 0;

    static string newName(const string &str, int i);
    static string newName(const string &str, int i, int j);

    static llvm::tool_output_file *getOutputStream(std::string OutputFilename);
  private:
    CompileUnit* splitIntoFunctions(std::queue<WeightedStmt> *stmts);

    void loadBuffer(llvm::ObjectBuffer *Buffer);
  };


  class CombinedSpecializer: public SpMVSpecializer {
  private:
    SpMVSpecializer *specOne, *specTwo;

  public:
    CombinedSpecializer(SpMVSpecializer *specOne, 
                        SpMVSpecializer *specTwo): 
      specOne(specOne), specTwo(specTwo) {
      mmMatrix = specOne->getMMMatrix();
    }
    
    virtual CompileUnit* getAllDecls();

    void setOptimizationPasses();

    virtual void dumpAssemblyText(llvm::MCStreamer *Str);
    virtual void dumpAssemblyConstData(llvm::MCStreamer *Str);
    virtual void dumpAssemblyData(llvm::MCStreamer *Str);
    virtual void dumpMatrixValueSetterFunction(llvm::MCStreamer *Str);
    virtual void setMatrixValuesInGeneratedAssembly();
  protected:
    virtual StmtCode* getLocalDecls();

    virtual std::queue<WeightedStmt>* getBody(Matrix *matrix);
    
    virtual Matrix* getMatrixForGeneration();
  };


  class Unrolling: public SpMVSpecializer {
  protected:
    int u;
  public:
    Unrolling(MMMatrix *mmMatrix, int u):
      SpMVSpecializer(mmMatrix), u(u) {}

    void setOptimizationPasses();
  protected:
    Matrix* getMatrixForGeneration();

    StmtCode* getLocalDecls();

    virtual StmtCode* getInnerLoopBody();

    std::queue<WeightedStmt>* getBody(Matrix *matrix);
  };
  

  class OSKI: public SpMVSpecializer {
  private:
    int b_r, b_c;
  public:
    OSKI(MMMatrix *mmMatrix, int b_r, int b_c);

    void setOptimizationPasses();

    virtual void dumpAssemblyText(llvm::MCStreamer *Str);
    virtual void dumpAssemblyConstData(llvm::MCStreamer *Str);
    virtual void dumpAssemblyData(llvm::MCStreamer *Str);

  protected:
    Matrix* getMatrixForGeneration();

    StmtCode* getLocalDecls();

    StmtCode* getWhileLoop();

    std::queue<WeightedStmt>* getBody(Matrix *matrix);

    void dumpForLoop(llvm::MCStreamer *Str);
  };


  class CSRbyNZ: public SpMVSpecializer {
  public:
    CSRbyNZ(MMMatrix *mmMatrix):
      SpMVSpecializer(mmMatrix){}

    void setOptimizationPasses();

    virtual void dumpAssemblyText(llvm::MCStreamer *Str);
    virtual void dumpAssemblyConstData(llvm::MCStreamer *Str);
    virtual void dumpAssemblyData(llvm::MCStreamer *Str);    
  protected:
    Matrix* getMatrixForGeneration();

    CompileUnit* getGlobalDecls(Matrix *matrix);

    StmtCode* getLocalDecls();

    std::queue<WeightedStmt>* getBody(Matrix *matrix);

    StmtCode* getSingleLoop(int numRows, int rowLength);

    void dumpSingleLoop(llvm::MCStreamer *Str, int numRows, int rowLength, int loopIndex);
  };
  

  class Stencil: public SpMVSpecializer {
  public:
    Stencil(MMMatrix *mmMatrix):
      SpMVSpecializer(mmMatrix){}
    
    void setOptimizationPasses();

    virtual void dumpAssemblyText(llvm::MCStreamer *Str);
    virtual void dumpAssemblyConstData(llvm::MCStreamer *Str);
    virtual void dumpAssemblyData(llvm::MCStreamer *Str);    
  protected:
    Matrix* getMatrixForGeneration();

    CompileUnit* getGlobalDecls(Matrix *matrix);

    StmtCode* getLocalDecls();

    std::queue<WeightedStmt>* getBody(Matrix *matrix);

    StmtCode* getStencilLoop(const std::vector<int> &stencil, 
                             std::vector<int> &rows);

  private:
    void dumpStencilAssemblyText(llvm::MCStreamer *Str,
                         const std::vector<int> &stencil, 
                         std::vector<int> &rows, 
                         int baseValsIndex);
  };
  

  class Unfolding: public SpMVSpecializer {
  private:
    int b_r, b_c;
  public:
    // Infinite X infinite unfolding
    Unfolding(MMMatrix *mmMatrix);

    void setOptimizationPasses();

    virtual void dumpAssemblyText(llvm::MCStreamer *Str);
    virtual void dumpAssemblyConstData(llvm::MCStreamer *Str);
    virtual void dumpAssemblyData(llvm::MCStreamer *Str);    
  protected:
    Matrix* getMatrixForGeneration();

    CompileUnit* getGlobalDecls(Matrix *matrix);

    StmtCode* getLocalDecls();

    std::queue<WeightedStmt>* getBody(Matrix *matrix);
  private:
    void addBlockStmts(std::queue<WeightedStmt> &destination,
                       std::vector<MMElement*> &elts);
    void dumpElements(llvm::MCStreamer *Str, std::vector<MMElement*> &elts);
    void doMathOperations(llvm::MCStreamer *Str, int lastRegIndex, vector<int> &colIndices, vector<int> &valIndices);
    void dumpRowConclusion(llvm::MCStreamer *Str, int rowIndex, int lastRegIndex);
    void dumpPartialRowConclusion(llvm::MCStreamer *Str, int rowIndex, int lastRegIndex);
    void dumpRegisterReduce(llvm::MCStreamer *Str, int lastRegIndex);
    bool existsInBlock(int colIndex, GroupByBlockView::iterator &blockViewIterator);
  };
  

  class Diagonal: public SpMVSpecializer {
  public:
    Diagonal(MMMatrix *mmMatrix);

    void setOptimizationPasses();
  protected:
    Matrix* getMatrixForGeneration();

    CompileUnit* getGlobalDecls(Matrix *matrix);

    StmtCode* getLocalDecls();

    std::queue<WeightedStmt>* getBody(Matrix *matrix);
  };


  class Splitter {
  protected:
    MMMatrix *mmMatrix, *mmMatrixOne, *mmMatrixTwo;

  public:
    Splitter(MMMatrix *mmMatrix): mmMatrix(mmMatrix) {}

    // Put the matrix elements into mmMatrixOne and mmMatrixTwo.
    virtual void split() = 0;
    
    MMMatrix* getMatrixOne() {
      return mmMatrixOne;
    }

    MMMatrix* getMatrixTwo() {
      return mmMatrixTwo;
    }
  };


  // This splitter assigns the first matrix to the original,
  // and the second matrix to an empty matrix.
  // The main purpose of this splitter is to run single
  // specializers using a CombineSpecializer. 
  // Because the second matrix is empty, the second specializer
  // in the CombinedSpecializer will essentially be ignored.
  class TakeAllSplitter: public Splitter {
  public:
    TakeAllSplitter(MMMatrix *mmMatrix):
      Splitter(mmMatrix) {}
    
    void split();
  };


  // Include the first "count" elements to matrixOne,
  // and the rest to matrixTwo.
  class ByCountSplitter: public Splitter {
  private:
    int count;

  public:
    ByCountSplitter(MMMatrix *mmMatrix, int count):
      Splitter(mmMatrix), count(count) {}
    
    void split();
  };


  // Divide matrix into elts within a band around the main diagonal and the rest.
  // lo and hi: typically positive numbers. 
  // E.g. to take the bands that are at most 5 elements away from the 
  // diagonal, set lo=5 and hi=5. (*not* lo=-5). 
  class ByBandSplitter: public Splitter {
  private:
    int lo, hi;

  public:
    ByBandSplitter(MMMatrix *mmMatrix, int lo, int hi):
      Splitter(mmMatrix), lo(lo), hi(hi) {}
    
    void split();
  };


  // Divide matrix into elts along certain diagonals and the rest.
  // The diagonals chosen are those that are within a certain distance
  // of the main diagonal (just as for splitByBand), *and*  as dense as
  // the percentage given as an argument.
  // percent: a value between 0 and 100.
  class ByDenseBandSplitter: public Splitter {
  private:
    int lo, hi;
    double percent;

  public:
    ByDenseBandSplitter(MMMatrix *mmMatrix, int lo, int hi, double percent):
      Splitter(mmMatrix), lo(lo), hi(hi), percent(percent) {}
    
    void split();
  };


  // For OSKI, get all elements that fit perfectly
  // in the given block size
  class ByBlockSplitter: public Splitter {
  private:
    int b_r, b_c;

  public:
    ByBlockSplitter(MMMatrix *mmMatrix, int b_r, int b_c):
      Splitter(mmMatrix), b_r(b_r), b_c(b_c) {}
    
    void split();
  };

}

#endif
