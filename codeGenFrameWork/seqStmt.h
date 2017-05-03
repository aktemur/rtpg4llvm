#ifndef _SEQ_STMT
#define _SEQ_STMT

#include "stmtCode.h"
#include <vector>

namespace codeGen {

class SeqStmt : public StmtCode {
protected:
   vector<StmtCode*> *stmts;
public:
   SeqStmt(vector<StmtCode*> *stmts): stmts(stmts) {
   }
   ~SeqStmt();
   
   virtual void build();
   virtual void print(int indent);
};

  
class ScopedStmt : public SeqStmt {
public:
   ScopedStmt(vector<StmtCode*> *stmts): SeqStmt(stmts) {
   }
   
   virtual void build();
   virtual void print(int indent);
};

}

#endif