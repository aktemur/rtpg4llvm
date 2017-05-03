import java.util.ArrayList;

class Codegen {

   // Defines function codegen(AST), which returns the LLVM code
   // for this AST.  This does not cover all AST operators,
   // only those for which code is to be generated.  The main AST operator
   // of arguments to codegen is method or one of the expr or stmt
   // operators (see abssynops.java), excluding: assertstmt, trystmt,
   // foriterator, listexpr, dictexpr, mappair, and rangeexpr.  In
   // addition, nameorfunlist is restricted to simple variables or
   // function calls; dot notation is not supported at present because
   // we have not implemented structs.

   static int indent = 3;
   static String spaces =
      "                                                    "
      + "                                                    ";

   static String spaces (int s) {
      return spaces.substring(0, s);
   }

   static String codegen (ArrayList<AST> tl, int indent) {
      String r = "";
      if (tl.size() == 0)
         return r;
      r = codegen(tl.get(0));
      for (int i=1; i<tl.size(); i++)
         r += "," + "\n" + spaces(indent)
         + codegen(tl.get(i));
      return r;
   }

   private static String tokenToTypeString(AST tok) {
      switch(tok.getToken()) {
      case Int: return "new IntType()";
      case Double: return "new DoubleType()";
      case Void: return "new VoidType()";
      case Char: return "new CharType()";
      case Bool: return "new BoolType()";
      case Identifier: return "new NamedType(\"" + ((IdentInfo)tok.thetokenvalue).thename + "\")";
      default: throw new Error("Unexpected type token.");
      }
   }

   public static String codegenType (AST t, AST stars) {
      String r = "";
      ArrayList<AST> children = stars.getChildren();
      for (int i=0; i<children.size(); i++)
         r += "new PtrType(";

      r += tokenToTypeString(t);

      for (int i=0; i<children.size(); i++)
         r += ")";

      return r;
   }

   public static String codegenArrayType(AST t, String base) {
      if(t == null)
         return base;
      
      ArrayList<AST> children = t.getChildren();
      if (children.size() == 0) {
         return "new ArrayType(" + base + ")";
      } else {
         return "new ArrayType(" + base + "," + children.get(0).getValue() +")";
      }
   }
   
   public static String codegen (AST t) {
      return codegen(t, indent);
   }

   public static String codegen (AST t, int s) {
      if (t.isToken())
         return printTokenAST(t);

      ArrayList<AST> children = t.getChildren();
      abssynops op = t.getAbssynop();
      switch (op) {

      case compUnit:
         // start with the length of the arguments so that 
         // vararg function knows when to stop
         if(children.size() == 1) 
             return codegen(children.get(0), s); 
         String units = children.size() + (children.size() == 0 ? "" : ", "); 
         units += codegen(children, indent);
         return make(op, units);

      case function:
         return make(op, codegen(children, s+indent));

      case formals:
         // start with the length of the arguments so that 
         // vararg function knows when to stop
         String formals = children.size() + (children.size() == 0 ? "" : ", "); 
         formals += codegen(children, s+indent+indent);
         return make(op, formals);

      case atype:
         return codegenType(children.get(0), children.get(1));

      case var:
         String tt = codegen(children.get(0));
         boolean isRestricted = children.get(1) != null;
         String id = codegen(children.get(2));
         tt = codegenArrayType(children.get(3), tt);
         if(isRestricted)
            return make(op, tt + ", " + id);
         else
            return make(op, tt + ", " + id + ", true");

      case funcParam:
         int argIdx = 0;
         tt = codegen(children.get(argIdx++));
         isRestricted = children.get(argIdx++) != null;
         id = codegen(children.get(argIdx++));
         if(children.get(argIdx) != null)
            tt = codegenArrayType(children.get(argIdx++), tt);
         
         if(!isRestricted)
            return make(op, tt + ", " + id);
         else
            return make(op, tt + ", " + id + ", true");

      case stmtlist:
         // start with the length of the arguments so that 
         // vararg function knows when to stop
         if(children.size() == 1) 
            return codegen(children.get(0), s); 
         String stmts = children.size() + (children.size() == 0 ? "" : ", "); 
         stmts += codegen(children, s+indent);
         return make(op, stmts);
         
      case scopedStmtList:
         // start with the length of the arguments so that 
         // vararg function knows when to stop
         children = children.get(0).children;
         stmts = children.size() + (children.size() == 0 ? "" : ", "); 
         stmts += codegen(children, s+indent);
         return make(op, stmts);

      case exprstmt:
         return make(op, codegen(children, s+indent));

      case vardecl:
         AST varAST = children.get(0);
         tt = codegen(varAST.getChild(0), 0);
         isRestricted = varAST.getChild(1) != null;
         String varName = makeVarName(varAST.getChild(2));
         tt = codegenArrayType(varAST.getChild(3), tt);
         
         if(children.size() == 1) {
            return "new VarDecl(" + tt + ", " + varName + (isRestricted ? ", true" : "") + ")";
         } else {
            String init = codegen(children.get(1), 0);
            if(children.get(1).A == abssynops.listexpr || init.startsWith("mkConstList(")) { // TODO: This is an ugly hack
               return "new ListDecl(" + tt + ", " + varName + ", " + init + ")";
            } else {
               return "new VarDecl(" + tt + ", " + varName + ", " + init + (isRestricted ? ", true" : "") + ")";
            }
         }

      case ifstmt:
         return make(op, codegen(children, s+indent));

      case whilestmt:
         return make(op, codegen(children, s+indent));

      case returnstmt:
         AST a = t.getReturnexpr();
         if(a != null)
            return make(op, codegen(a, s));
         else
            return make(op, "");
         
      case breakstmt:
      case continuestmt:
         return make(op);

      case  litral:
         a = children.get(0);
         tokens tok = a.getToken();
         Object v = a.getValue();
         switch (tok) {
         case Charlit:
         case Intlit:
            return make((MyInteger)(v));
         case Doublelit:
            return make((MyDouble)(v));
         case Stringlit:
            return make((String)v);
         case True:
            return make(true);
         case False:
            return make(false);
         case Null:
            return "new NullExpr()";
         default:  // Can't happen!
         return "";
         }

      case ident:
         return make(op, makeVarName(t.getChild(0)));

      case funcall:
         return make(op, codegen(children, s+indent));

      case nameorargslist:
         return make(op, codegen(children, s+indent));

      case  unarypreoptr:
         return make(op, t.getOptr().toString(),
               codegen(children.get(1), s+indent));

      case  unarypostoptr:
         return make(op, t.getOptr().toString(),
               codegen(children.get(1), s+indent));

      case  binaryop:
         return make(op, t.getOptr().toString(), 
               codegen(children.get(1), s+indent),
               codegen(children.get(2), s+indent),
               s+indent);
      case listexpr:
          // start with the length of the arguments so that 
          // vararg function knows when to stop
          String exps = children.size() + (children.size() == 0 ? "" : ", "); 
          exps += codegen(children, s+indent);
          return make(op,  "mkExprList("+exps+")");
      case exprlist:
         // start with the length of the arguments so that 
         // vararg function knows when to stop
         exps = children.size() + (children.size() == 0 ? "" : ", "); 
         exps += codegen(children, s+indent);
         return make(op,  exps);

      case aquote:
         a = children.get(0);
         //tokens t = a.getToken();
         v = a.getValue();
         return (String)v;

      case arrayexpr:
         String e1 = codegen(children.get(0), 0);
         String e2 = codegen(children.get(1), 0);
         return "new ArrayExpr(" + e1 + ", " + e2 + ")";
      
      case externDecl:
         String type = codegen(children.get(0), 0);
         id = codegen(children.get(1), 0);  
         if(children.get(2) != null) {
             type = codegenArrayType(children.get(2), type);
         }
         return "new ExternDecl(" + type + ", " + id + ")";
         
      case globalDecl:
         type = codegen(children.get(0), 0);
         id = codegen(children.get(1), 0);
         String init = null;
         if(children.get(2) != null)
             type = codegenArrayType(children.get(2), type);
         if(children.get(3) != null)
             init = codegen(children.get(3), 0);

         if(init == null) {
            return "new GlobalDecl(" + type + ", " + id + ")";
         } else {
            return "new GlobalDecl(" + type + ", " + id + ", " + init + ")";
         }
         
      }
      return null;
   }

   static String makeVarName(AST t) {
      if(t.A == abssynops.aquote) {
         AST child = t.getChild(0);
         return child.thetokenvalue.toString();
      } else if(t.isToken()) {
         if(t.thetokenvalue instanceof IdentInfo) {
            return "\"" + ((IdentInfo)t.thetokenvalue).name() + "\"";
         } else {
            return "\"" + t.thetokenvalue + "\"";
         }
      }
      return "";
   }
   
   static String make (abssynops op, String args) {
      return stringname(op) + "(" + args + ")";
   }

   static String make (abssynops op, String args, String args2,
         String args3, int s) {
      return stringname(op) + "(" + args + ",\n" + spaces(s+indent)
      + args2 + ",\n" + spaces(s+indent) + args3 + ")";
   }

   static String make (abssynops op, String args, String args2) {
      return stringname(op) + "(" + args + ", " + args2 + ")";
   }

   static String make (abssynops op) {
      return stringname(op) + "()";
   }

   static String make (String s) {
      return "mkStringlit(\"" + s + "\")";
   }

   static String make (MyInteger i) {
      if (i.isChar)
         return "mkCharlit(" + (i+"") + ")"; 
      else
         return "mkConst(" + i + ")"; 
   }

   static String make (MyDouble i) {
      return "mkConst(" + i + ")"; 
   }

   static String make (boolean b) {
      return "mkConst(" + b + ")"; 
   }

   static String stringname (abssynops op) {
      switch (op) {
      case compUnit: return "mkProgram";
      case stmtlist: return "mkSeqStmt";
      case scopedStmtList: return "mkScopedStmt";
      case exprstmt: return "new ExprStmt";
      case vardecl: return "new vardecl";
      case ifstmt: return "new IfElseStmt";
      case whilestmt: return "new WhileStmt";
      case returnstmt: return "new ReturnStmt";
      case breakstmt: return "new BreakStmt";
      case continuestmt: return "new ContinueStmt";
      case litral: return "new litral";
      case ident: return "new VarExpr";
      case funcall: return "new FuncCallExpr";
      case nameorargslist: return "new FieldAccess";
      case unarypreoptr: return "mkUnaryOp";
      case unarypostoptr: return "mkUnaryPostOp";
      case binaryop: return "mkBinOp";
      case aquote: return "new aquote";
      case exprlist: return "mkExprList";
      case listexpr: return "new ListExpr";
      case atype: return "new atype";
      case formals: return "mkFormals";
      case function: return "new FuncDeclaration";
      case funcParam: return "new FuncParam";
      default:
         System.out.println("Returning null for: "+op);
         return null;
      }
   }

   public static IdentInfo getidentinfo (AST a) {
      return (IdentInfo)(a.getValue());
   }

   static String printTokenAST (AST a) {
      // a known to be a token
      tokens tok = a.getToken();
      Object o = a.getValue();
      System.out.println("printtoken: " + tok +", "+ o);
      switch (tok) {
      case Identifier: return "\"" + ((IdentInfo)o).name() + "\"";
      case Intlit: return "Intlit - shouldn't happen";
      case Doublelit: return "Doublelit - shouldn't happen";
      case Charlit: return "Charlit - shouldn't happen";
      case Stringlit: return "Stringlit - shouldn't happen";
      default:  return "\"" + tok.toString() + "\"";
      }
   }

}
