import java.util.ArrayList;
//using System.Collections.Generic;

// Many of the comments are simply productions from the
// context-free grammar of XL.  This is to make it simpler to
// connect the expressions in the code (usually, children of some
// CST node) with the kind of node it is.  (The first comment below,
// on compilationUnit, gives the idea.)

public class CSTtoAST {
   // Translate CST to AST

   // Convenience method to just copy CST when it is a token
   public static AST token (CST c) {
      return new AST(c.thetoken, c.thetokenvalue,
                     c.startpos, c.endpos);
   }

   public static AST translate (CST c)
               throws SyntaxError
   {
      initializeOptrPrecs();
      return trans(c);
   }

   public static AST trans (CST c)
             throws SyntaxError
   {
      ArrayList<CST> children;
      ArrayList<AST> newchildren, templist, templist2, templist3;
      AST newchild, newchild1, newchild2, newchild3, newchild4, newchild5, newchild6;
      CST firstsym;
      tokens t, tok;
      
      if (c.isToken())
         return new AST(c.thetoken, c.thetokenvalue,
                        c.startpos, c.endpos);

      // Almost all cases make the following assignment,
      // so may as well do it here.
      children = c.getChildren();

      switch (c.A) {

      case compilationUnit:
         // compilationUnit -> 
         //   declarationList
         newchildren = translist(children.get(0));
         return new AST(abssynops.compUnit, newchildren,
                        c.startpos, c.endpos);

      case declaration:
//        [Empty],
//        # Function or global var
//        [declBegin, declCont],
//        # Function decl
//        [UnitSpliced, Antiquote],
//        # global var decl
//        [Extern, declBegin, arrayTypeOpt, Semicolon, declarationList], 
        if (children.size() == 0)
            return null;          
//        declBegin -> type, name
//        declCont  -> formalParameters, block
//                  |  arrayTypeOpt, constInitializerOpt, Semicolon, declarationList
//        arrayTypeOpt -> Empty
//                  | Lbracket, intOpt, Rbracket
//        intOpt    -> Empty
//                  |  Intlit
        
        tok = children.get(0).getToken();
        if (tok == tokens.__unitSpliced__) {
            return new AST(abssynops.aquote, 
                    token(children.get(1)),
                    c.startpos, c.endpos);
        } else {
            boolean isExtern = false;
            if (children.get(0).getToken() == tokens.Extern) {
                isExtern = true;
                children.remove(0);
            }
            AST typeAST = trans(children.get(0).children.get(0));
            AST nameAST = trans(children.get(0).children.get(1));
            templist = new ArrayList<AST>();
            templist.add(typeAST);
            templist.add(nameAST);
            if(!isExtern && children.get(1).children.get(0).getNonterm() == nonterms.formalParameters) {
                templist2 = translist(children.get(1).children.get(0)); // formalParameters
                templist.add(new AST(abssynops.formals, templist2,
                                        c.startpos, c.endpos));
                templist.add(trans(children.get(1).children.get(1))); // block
                return new AST(abssynops.function, templist,
                        c.startpos, c.endpos); 
            } else {
                if (isExtern) {
                    // declBegin, emptyArrayTypeOpt, Semicolon
                    templist.add(trans(children.get(1)));
                    return new AST(abssynops.externDecl, templist,
                            c.startpos, c.endpos);                      
                } else {
                    // declBegin, declCont 
                    //            -> arrayTypeOpt, variableInitializerOpt, Semicolon
                    children = children.get(1).children;
                    templist.add(trans(children.get(0)));
                    templist.add(trans(children.get(1)));
                    return new AST(abssynops.globalDecl, templist,
                            c.startpos, c.endpos);                     
                }
            }
        }
        
      case arrayTypeOpt:
          // Empty | Lbracket arraySize Rbracket
          if (children.size() == 0)
             return null;
          else
             return new AST(abssynops.arrayType, trans(children.get(1)),
                     c.startpos, c.endpos);
          
      case arraySize:
         // Intlit | ExprSpliced Antiquote
         if (children.size() == 1)
            return token(children.get(0));
         else
            return token(children.get(1));
         
      case emptyArrayTypeOpt:
          // Empty | Lbracket Rbracket
          if (children.size() == 0)
             return null;
          else
             return new AST(abssynops.arrayType, (new ArrayList<AST>()),
                     c.startpos, c.endpos);

      case restrictOpt:
         // Empty | Restrict
         if (children.size() == 0)
            return null;
         else
            return token(children.get(0));
    
      case type:
         // type -> nonPointerType starList         
         newchildren = new ArrayList<AST>();
         newchildren.add(trans(children.get(0)));
         newchildren.add(new AST(abssynops.list, translist(children.get(1)),
                                    c.startpos, c.endpos));
         return new AST(abssynops.atype, newchildren,
                        c.startpos, c.endpos);
            
      case nonPointerType:
         // nonPointerType -> classType | primitiveType
         // classType -> name
         // primitiveType -> Boolean | Char | Int | Double | Void
         CST typeAST = children.get(0).getChildren().get(0);
         if(typeAST.istoken) // primitiveType 
            return token(typeAST);
         else
            return token(typeAST.getChildren().get(0));
         
      case block:
         // block -> Lbrace statementList Rbrace
         newchildren = new ArrayList<AST>();
         newchildren.add(new AST(abssynops.stmtlist, translist(children.get(1)),
               c.startpos, c.endpos));
         return new AST(abssynops.scopedStmtList, newchildren,
                        c.startpos, c.endpos);

      case variableDeclarator:
         // variableDeclarator -> name arrayTypeOpt variableInitializerOpt
         // Return a "list" tree whose list has one
         // or two, or three elements (identifier, and possibly
         // an arrayTypeOpt and an expression)
         newchildren = new ArrayList<AST>();
         newchildren.add(trans(children.get(0)));
         newchildren.add(trans(children.get(1)));
         if(children.size() > 2)
            newchildren.add(trans(children.get(2)));
         return new AST(abssynops.list, newchildren,
                        c.startpos, c.endpos);
         
      case constInitializerOpt:
          // constInitializerOpt -> Empty | Equal contExpression
      case variableInitializerOpt:
         // variableInitializerOpt -> Empty | Equal expression
         if (children.size() == 0)
            return null;
         else
            return trans(children.get(1));

      case expression:
         return fixexpr(translist(c));
            
      case identifierOpt:
         // identifierOpt -> Empty | Identifier
      case expressionOpt:
         // expressionOpt -> Empty | expression
      case exprContOpt:
         // exprContOpt -> Empty | exprCont
         if (children.size() == 0)
            return null;
         else
            return trans(children.get(0));
         
         
      case parExpression:
         // parExpression -> Lparen expression Rparen
         return trans(children.get(1));
         
      case primary:
         // primary -> unaryOp primary
         // unaryOp -> Plus | Star | Minus | Bang | Ampersand
         //            PlusPlus | MinusMinus
         if (children.get(0).A == nonterms.unaryOp) {
            newchild = token(children.get(0).getChildren().get(0));
            newchild2 = trans(children.get(1));
            return new AST(abssynops.unarypreoptr, newchild, newchild2,
                           c.startpos, c.endpos);
         }
         else {
            // primary -> simplePrimary primarySuffix
            newchild = trans(children.get(0));
            templist = translist(children.get(1));
            if (templist.size() == 0)
               return newchild;
            else {
               XLList.addAtStart(templist, newchild);
               return new AST(abssynops.nameorargslist, templist,
                              c.startpos, c.endpos);
            }
         }
         
      case simplePrimary:
         if (children.size() == 1)
            // simplePrimary -> listExpression | parExpression | literal
            return trans(children.get(0));
         else {  // maybe Antiquote
            tok = children.get(0).getToken();
            if (tok == tokens.__exprSpliced__)
               return new AST(abssynops.aquote, 
                              token(children.get(1)),
                              c.startpos, c.endpos);
            // else name argumentsOpt
            newchild2 = trans(children.get(1));
            if (newchild2 == null) {
               newchild = trans(children.get(0));
               return new AST(abssynops.ident,
                     newchild,
                     c.startpos, c.endpos);
            } else {
               return new AST(abssynops.funcall, trans(children.get(0)),
                     newchild2, c.startpos, c.endpos);
            }
         }
         
      case argumentsOpt:
         // argumentsOpt -> Empty | arguments
         // arguments -> Lparen expressionList Rparen
         if (children.size() == 0)
            return null;
         templist = translist(children.get(0).getChildren().get(1));
         return new AST(abssynops.exprlist, templist,
                        c.startpos, c.endpos);
         
      case listExpression:
         // listExpression -> Lbrace listElementListOpt Rbrace
         newchildren = translist(children.get(1));
         return new AST(abssynops.listexpr, newchildren,
                        c.startpos, c.endpos);
         
      case listElement:
         // listElement -> expression
         return trans(children.get(0));
         
      case constExpression:
         // constExpression -> literal | listExpression | Minus literal | ExprSpliced Antiquote
         if (children.size() == 1) {
            return trans(children.get(0));
         } else {
            if (children.get(0).getToken() == tokens.__exprSpliced__) {
               return new AST(abssynops.aquote, token(children.get(1)),
                              c.startpos, c.endpos);               
            } else {
               newchild = token(children.get(0)); // minus
               newchild2 = trans(children.get(1));
               return new AST(abssynops.unarypreoptr, newchild, newchild2,
                              c.startpos, c.endpos);
            }
         }

      case literal:
         // literal -> Intlit | Doublelit | Charlit |
         //            Stringlit | True | False | Null
         return new AST(abssynops.litral, token(children.get(0)),
                        c.startpos, c.endpos);
         
      case name:
         // name -> Identifier | Namespliced Antiquote
         if (children.size() == 1)
            return token(children.get(0));
         else
            return new AST(abssynops.aquote, 
                           token(children.get(1)),
                           c.startpos, c.endpos);
                     
      case localVariableDeclarationStatement:
         // localVariableDeclarationStatement ->
         //        localVariableDeclaration Semicolon
         return trans(children.get(0));

      case localVariableDeclaration:
         // Type
         newchild = trans(children.get(0));
         AST restricted = trans(children.get(1));
         // Translates second element and following list; returns
         // list of "list" AST's, each having an identifier and
         // possibly an expression.
         templist = translist(c);
         // Restructure into a list of vardecls, each with
         // a "var" (type and ident) and optional initializer.
         newchildren = new ArrayList<AST>();
         for (int i=0; i<templist.size(); i++) {
             // Each item is a list node with one or two
             // children - ident plus possible expression
             templist2 = templist.get(i).getChildren();
             // ident
             newchild2 = templist2.get(0);
             // expression (may be null)
             newchild3 = templist2.get(2);
             // arrayType (may be null)
             newchild6 = templist2.get(1);             
             // var node
             newchild4 = new AST(abssynops.var, newchild, restricted, newchild2, newchild6,
                   c.startpos, c.endpos);
             // vardecl with var and optional initializer
             if (newchild3 == null)
                newchild5 = new AST(abssynops.vardecl, newchild4,
                        c.startpos, c.endpos);
             else
                newchild5 = new AST(abssynops.vardecl, newchild4, newchild3,
                        c.startpos, c.endpos);
             newchildren.add(newchild5);
         }
         if (newchildren.size() == 1)
             return newchildren.get(0);
         else
             return new AST(abssynops.stmtlist, newchildren,
                        c.startpos, c.endpos);

      case statementList:
         return new AST(abssynops.stmtlist, translist(c),
               c.startpos, c.endpos);
         
      case statement:
         newchildren = new ArrayList<AST>();
         // Need to distinguish among the various productions for stmt
         firstsym = children.get(0);
         if (firstsym.isToken()) {
            t = firstsym.getToken();
            switch (t) {
            case __stmtSpliced__:
               return new AST(abssynops.aquote, 
                              token(children.get(1)),
                              c.startpos, c.endpos);
            case If:
               // statement -> If parExpression statement elseStmtOpt
               // Condition
               newchildren.add(trans(children.get(1)));
               // True stmt
               newchildren.add(trans(children.get(2)));
               // Optional false statement; if absent, use empty stmt list
               newchild = trans(children.get(3));
               if (newchild != null)
                  newchildren.add(newchild);   
               
               return new AST(abssynops.ifstmt, newchildren,
                        c.startpos, c.endpos);
            case For:
               // statement -> For Lparen forstatement
               return trans(children.get(2));
            case While:
               // statement -> While parExpression statement
               // Condition
               newchildren.add(trans(children.get(1)));
               // body
               newchildren.add(trans(children.get(2)));
               return new AST(abssynops.whilestmt, newchildren,
                        c.startpos, c.endpos);
            case Return:
               // statement -> Return expressionOpt Semicolon |
               newchild = trans(children.get(1));
               if (newchild != null)
                  newchildren.add(newchild);
               return new AST(abssynops.returnstmt, newchildren,
                        c.startpos, c.endpos);
            case Break:
               // statement -> Break identifierOpt Semicolon |
               newchild = trans(children.get(1));
               if (newchild != null)
                  newchildren.add(newchild);
               return new AST(abssynops.breakstmt, newchildren,
                        c.startpos, c.endpos);
            case Continue:
               // statement -> Continue identifierOpt Semicolon |
               newchild = trans(children.get(1));
               if (newchild != null)
                  newchildren.add(newchild);
               return new AST(abssynops.continuestmt, newchildren,
                        c.startpos, c.endpos);
            case Semicolon:
               // statement -> Semicolon
               // null stmt = zero-length stmtlist
               return new AST(abssynops.stmtlist, newchildren,
                        c.startpos, c.endpos);
            default: throw new Error("impossible!");
            }
         }
         // Other cases are block, localVariableDeclarationStatement,
         // and expression stmt, where first child is the stmt.

         // statement -> expression Semicolon |
         // statement -> block |
         // statement -> localVariableDeclarationStatement |
         else if (firstsym.getNonterm() == nonterms.expression)
                 return new AST(abssynops.exprstmt, trans(firstsym),
                        c.startpos, c.endpos);
              else
                 return trans(firstsym);

      case elseStmtOpt:
         // elseStmtOpt -> Empty | Else statement
         if (children.size() == 0)
            return null;
         else
            return trans(children.get(1));

      case forstatement:
         // forstatement -> type typeFollower | typeFollower
         CST typefollower;
         if (children.size() == 2) {
            // prod. is forstatement -> type typefollower
            // Translate type
            newchild = trans(children.get(0));
            typefollower = children.get(1);
         }
         else {
            newchild = null;
            typefollower = children.get(0);
         }
         ArrayList<CST> cstlis = typefollower.getChildren();
         if (cstlis.get(0).isToken()) {
            newchild = new AST(abssynops.var,
                             newchild, null, token(cstlis.get(0)), // null is for restrict keyword
                             null, // null is for arrayType
                             c.startpos, c.endpos);
            newchild2 = trans(cstlis.get(2));
            newchild3 = trans(cstlis.get(4));
            return new AST(abssynops.foriterator, newchild,
                               newchild2, newchild3,
                        c.startpos, c.endpos);
         }
         else {
            //  | expressionList Semicolon expressionOpt Semicolon |
            //    expressionList Rparen statement |
            // If no type, do not use any
            // initializing expression list
            templist = translist(cstlis.get(0));
            ArrayList<AST> inits = new ArrayList<AST>();
            for (int i=0; i<templist.size(); i++) {
               AST initexpr = templist.get(i);
               inits.add(new AST(abssynops.exprstmt, initexpr,
                        c.startpos, c.endpos));
            }

            // If type was given, pull out variables to declare
            ArrayList<AST> decls = new ArrayList<AST>();
            if (newchild != null)
               // Go through expressions; for any of the form
               // x = e, add declaration of x.
               for (int i=0; i<templist.size(); i++) {
                  AST ast = templist.get(i);
                  if (ast.getAbssynop() == abssynops.binaryop &&
                        ast.getChildren().get(0).getToken() ==
                                                tokens.Equal) {
                     AST var = new AST(abssynops.var,
                                 newchild, null, // null is for restrict keyword
                                 ast.getChildren().get(1).getChildren().get(0),
                                 null, // null is for arrayType
                                 c.startpos, c.endpos);
                     AST varstmt = new AST(abssynops.vardecl, var,
                                           c.startpos, c.endpos);
                     decls.add(varstmt);
                  }
               }
            // combine decls and inits
            decls.addAll(inits);
            
            // newchild3 is condition
            newchild3 = trans(cstlis.get(2));
            if (newchild3 == null)
               // condition missing; use "true"
               newchild3 = new AST(abssynops.litral,
                                   new AST(tokens.True, null,
                                            c.startpos, c.endpos),
                                 c.startpos, c.endpos);
            // newchild4 is increments
            templist = translist(cstlis.get(4));
            ArrayList<AST> incrs = new ArrayList<AST>();
            for (int i=0; i<templist.size(); i++) {
               AST increxpr = templist.get(i);
               incrs.add(new AST(abssynops.exprstmt, increxpr,
                                 c.startpos, c.endpos));
            }
            // newchild5 is body
            AST body = trans(cstlis.get(6));
            // Put these statement in the form of a while loop:
            // {decls; inits; while (condition) {body; incrs}}
            if(body.A == abssynops.scopedStmtList) {
               body.children.get(0).children.addAll(incrs);
            } else {
               ArrayList<AST> newBody = new ArrayList<AST>();
               newBody.add(body);
               newBody.addAll(incrs);
               newchild5 = new AST(abssynops.stmtlist, newBody,
                     c.startpos, c.endpos);
               body = new AST(abssynops.scopedStmtList, newchild5,
                              c.startpos, c.endpos);
            }
            AST whilestmt = new AST(abssynops.whilestmt,
                                     newchild3, body,
                                   c.startpos, c.endpos);
            // add whilestmt after decls and inits
            decls.add(whilestmt);
            return new AST(abssynops.scopedStmtList, new AST(abssynops.stmtlist, decls, c.startpos, c.endpos),
                                    c.startpos, c.endpos);
         }
      }
      throw new Error("Unmatched term: " + c.A);

   }

   // For CST nodes that are most naturally translated as lists.
   // Returns list corresponding to the children of this node.
   // Never returns null; may return an empty ArrayList<AST>.
   public static ArrayList<AST> translist (CST c)
          throws SyntaxError
   {
      ArrayList<CST> children;
      ArrayList<AST> newchildren, templist, templist2;
      AST newchild, newchild2, newchild3, newchild4, newchild5;

      children = c.getChildren();
      newchildren = new ArrayList<AST>();

      switch (c.A) {
      case declarationList:
      case statementList:
          // statementList -> Empty | statement statementList
      case starList:
         // starList -> Empty | Star starList            
         if (children.size() == 0)
            return newchildren;
         newchild = trans(children.get(0));
         newchildren = translist(children.get(1));
         XLList.addAtStart(newchildren, newchild);
         return newchildren;

      case variableDeclaratorList:
         // variableDeclaratorList -> Empty |
         //      Comma variableDeclarator variableDeclaratorList
      case commaExprList:
         // commaExprList -> Empty | Comma expression commaExprList
         if (children.size() == 0)
            return newchildren;
         newchild = trans(children.get(1));
         newchildren = translist(children.get(2));
         XLList.addAtStart(newchildren, newchild);
         return newchildren;

      case formalParameters:
         // formalParameters -> Lparen formalParameterDeclsOpt Rparen
         return translist(children.get(1));

      case formalParameterDeclsOpt:
         // formalParameterDeclsOpt -> Empty | formalParameterDecls
         if (children.size() == 0)
            return newchildren;
         else
            return translist(children.get(0));

      case formalParameterDecls:
         // formalParameterDecls -> type restrictOpt name arrayTypeOpt typeIdentList
         templist2 = new ArrayList<AST>();
         templist2.add(trans(children.get(0)));
         templist2.add(trans(children.get(1)));
         templist2.add(trans(children.get(2)));
         templist2.add(trans(children.get(3)));
         newchild = new AST(abssynops.funcParam, templist2,
                        c.startpos, c.endpos);
         newchildren = translist(children.get(children.size()-1));
         XLList.addAtStart(newchildren, newchild);
         return newchildren;

      case typeIdentList:
         // typeIdentList -> Empty | Comma type name arrayTypeOpt typeIdentList
         if (children.size() == 0)
           return newchildren;
         templist2 = new ArrayList<AST>();
         templist2.add(trans(children.get(1)));
         templist2.add(trans(children.get(2)));
         templist2.add(trans(children.get(3)));
         templist2.add(trans(children.get(4)));
         newchild = new AST(abssynops.funcParam, templist2,
                        c.startpos, c.endpos);
         newchildren = translist(children.get(children.size()-1));
         XLList.addAtStart(newchildren, newchild);
         return newchildren;

      case localVariableDeclaration:
         // localVariableDeclaration ->
         //      type restrictOpt variableDeclarator variableDeclaratorList
         // variable declarator (ident and optional expr)
         newchild = trans(children.get(2));
         // list of same
         newchildren = translist(children.get(3));
         XLList.addAtStart(newchildren, newchild);
         return newchildren;

      case expression:
         if (children.get(0).A == nonterms.primary) {
            // expression -> primary exprContOpt
            newchild = trans(children.get(0));
            if (children.get(1).getChildren().size() == 0) {
               // exprCont is empty
               XLList.addAtStart(newchildren, newchild);
               return newchildren;
            }
            else {
               // exprCont node
               CST secondSym = children.get(1).getChildren().get(0);
               newchildren = translist(secondSym);
               XLList.addAtStart(newchildren, newchild);
               return newchildren;
            }
         }
         else { // expression -> unaryOp expression
            newchild = token(children.get(0).getChildren().get(0));
            newchildren = translist(children.get(1));
            XLList.addAtStart(newchildren, newchild);
            return newchildren;
         }

      case exprContOpt:
         // exprContOpt -> Empty | exprCont
         if (children.size() == 0)
            return newchildren;
         else
            return translist(children.get(0));

      case listElementListOpt:
         // listElementListOpt -> Empty | Comma
         if (children.size() <= 1)  // Empty or Comma
            return newchildren;
   	 //          listElement commaListElementList |
         newchild = trans(children.get(0));
         templist = translist(children.get(1));
         XLList.addAtStart(templist, newchild);
         return templist;

      case expressionList:
         // expressionList -> Empty | expression commaExprList
         if (children.size() == 0)
            return newchildren;
         newchild = trans(children.get(0));
         templist = translist(children.get(1));
         XLList.addAtStart(templist, newchild);
         return templist;

      case commaListElementList:
         // commaListElementList -> Empty | Comma |
         if (children.size() <= 1)
            return newchildren;
   	 // Comma listElement commaListElementList
         newchild = trans(children.get(1));
         templist = translist(children.get(2));
         XLList.addAtStart(templist, newchild);
         return templist;

      case primarySuffix:
         // primarySuffix -> Empty
         if (children.size() == 0)
            return newchildren;
         else if (children.get(0).getNonterm()
                   == nonterms.listExpression) {
            // listExpression primarySuffix
            newchild = trans(children.get(0));
            templist = translist(children.get(1));
            XLList.addAtStart(templist, newchild);
            return templist;
         }
         else {
   	    // Dot Identifier primarySuffix
            newchild = trans(children.get(1)); // identifier
            templist = translist(children.get(2));
            XLList.addAtStart(templist, newchild);
            return templist;
         }

      case exprCont:
         nonterms A = children.get(0).getNonterm();
         if (A == nonterms.binop) {
            // binop expression
            newchild = token(children.get(0).getChildren()
                                    .get(0).getChildren().get(0));
            templist = translist(children.get(1));
            XLList.addAtStart(templist, newchild);
            return templist;
         } else if (A == nonterms.incrOptr) {
            // incrOptr exprContOpt
            // incrOptr -> PlusPlus | MinusMinus
            // go through exprCont -> incrOptr
            newchild = token(children.get(0).getChildren().get(0));
            templist = translist(children.get(1));
            XLList.addAtStart(templist, newchild);
            return templist;
         } else if (children.get(0).getToken() == tokens.Lbracket) {
            newchild2 = trans(children.get(1));
            templist = translist(children.get(3));
            newchild = new AST(abssynops.arrayindex, newchild2,
                  c.startpos, c.endpos);
            XLList.addAtStart(templist, newchild);
            return templist;
         } else {           
            throw new Error("Unexpected token.");
         }

      default:
         return newchildren;
      }
   }

   // Fixing expressions to take care of precedence and associativity.
   // This needs some explanation.  After handling "primaries," what's
   // left is lists of alternating expressions and operators.
   // fixexpr is called from "expression" above.
   // Specifically, the list passed in has exprs, binary operators,
   // prefix operators, and suffix increment operators (++ and --).
   // The form of these lists can be described thusly:
   //
   //    opndlist = expr optrlist | prefixop opndlist
   //    optrlist = binop opndlist | incrop optrlist
   //             | listexpr optrlist | arrayindex(expr) optrlist 
   //
   // Note that the incrop in an optrlist is a suffix operator which
   // applies to the previous expression.  Also note that a
   // listexpr can occur either at the head of an opndlist, in
   // which case it is an actual list expression, or at the head
   // of an optrlist, in which case it is a subscript of the
   // previous operand.
   //
   // The functions below have two arguments, a stack and a list.
   // The stack is similar to a bottom-up parse stack.  In this
   // case, it is strictly alternating between expressions and
   // binary operators.  For fixopndlist, the top of the stack
   // is an operator and the list argument is an opndlist; for
   // fixoptrlist, the top of the stack is an operand and the
   // list argument is an optrlist.
   //
   // The functions basically operator by either shifting the
   // top item from the list to the stack, or reducing the top
   // of the stack (forming an expression from the operands and
   // operator at the top of the stack.  These actions depend upon
   // the relative precedence and associativity of the operators.
   // Note that they work by side effect on their arguments.
   //
   // List and Stack are both represented by ArrayList<AST>'s, with
   // the "top" of the stack being the front of the list (i.e.
   // at location 0).

   public static AST fixexpr (ArrayList<AST> L) {
      if (L.size() == 0) {
         // Can't happen!
         System.out.println("Empty expression list to fix");
         return null;
      }
      ArrayList<AST> stack = new ArrayList<AST>();
      //stack.add(L.get(0));
      //L.remove(0); // = XLList.tail(L); // L.removeHead();
      //fixoptrlist(L, stack);
      fixopndlist(L, stack);
      while (stack.size() > 1)
         reduce(stack);    
      return stack.get(0);
   }

   // fixopndlist - shift operand from input (L) to stack.
   public static void fixopndlist(ArrayList<AST> L, ArrayList<AST> stack) {
      if (L.size() == 0)  // can't happen!
        return;
      // First shift any prefix operators
      while (L.get(0).istoken)
         shift(L, stack);
      // ... then the operand itself
      shift(L, stack);
      fixoptrlist(L, stack);
      return;
   }
   
   // fixoptrlist - stack contains operand and L, if non-empty,
   //   contains operator.  Shift or reduce depending upon
   //   precedence and associativity
   public static void fixoptrlist(ArrayList<AST> L, ArrayList<AST> stack) {
      if (L.size() == 0) {
        return;
      }
      AST opnd;
      AST optr = L.get(0);
      // Check for suffix increment operator
      if (optr.getAbssynop() == abssynops.arrayindex) {
         L.remove(0);
         opnd = stack.get(0);
         stack.remove(0);
         XLList.addAtStart(stack,
                          new AST(abssynops.arrayexpr, opnd, optr.getChild(0),
                                  opnd.startpos, optr.endpos));
         fixoptrlist(L, stack);
      }
      else if (has_postop(stack)) {
         reduce(stack);
         fixoptrlist(L, stack);
      }
      else if (has_preop(stack)) {
         int prec = unary_precedence(stack.get(1), optr);
         if (prec == -1) {
            shift(L, stack);
            if (is_postop(optr))
               fixoptrlist(L, stack);
            else
               fixopndlist(L, stack);
         }
         else {
            reduce(stack);
            fixoptrlist(L, stack);
         }
      }
      else if (is_postop(optr)) {
         shift(L, stack);
         fixoptrlist(L, stack);
      }
      else {  // it's a binop
         if (stack.size() == 1) {
            shift(L, stack);
            fixopndlist(L, stack);
         }
         else {
            int prec = precedence(stack.get(1), optr);
            if (prec == -1) {
               // stack optr lower precedence than input
               shift(L, stack);
               fixopndlist(L, stack);
            }
            else if (prec == 1) {
               // stack optr higher precedence than input
               reduce(stack);
               fixoptrlist(L, stack);
            }
            else if (isRightAssoc(optr) == 1) {
               shift(L, stack);
               fixopndlist(L, stack);
            }
            else {
               reduce(stack);
               fixoptrlist(L, stack);
            }
         }
      }
   }

   public static void shift (ArrayList<AST> L, ArrayList<AST> stack) {
      XLList.addAtStart(stack, L.get(0));
      L.remove(0);
   }

   // reduce - called only when stack size is 2 or more
   // Remove top 2 or 3 elements of stack and create AST from them
   public static void reduce (ArrayList<AST> stack) {
      AST opnd1, opnd2, optr;
      if (has_postop(stack)) {
         optr = stack.get(0); stack.remove(0);
         opnd1 = stack.get(0); stack.remove(0);
         XLList.addAtStart(stack, new AST(abssynops.unarypostoptr, optr,
                        opnd1, opnd1.startpos, optr.endpos));
      }
      else if (has_preop(stack)) {
         opnd1 = stack.get(0); stack.remove(0);
         optr = stack.get(0); stack.remove(0);
         XLList.addAtStart(stack, new AST(abssynops.unarypreoptr, optr,
                        opnd1, optr.startpos, opnd1.endpos));
      }
      else {
         opnd2 = stack.get(0); stack.remove(0);
         optr = stack.get(0); stack.remove(0);
         opnd1 = stack.get(0); stack.remove(0);
         XLList.addAtStart(stack,
                        new AST(abssynops.binaryop, optr, opnd1, opnd2,
                                opnd1.startpos, opnd2.endpos));
      }
   }

   // has_preop: check if stack contains a prefix operator
   // Called from fixoptrlist, so stack has an operand on top.
   // The question is whether it has a symbol before that, and
   // if that symbol is a preop rather than a binop.
   public static boolean has_preop (ArrayList<AST> stack) {
      return stack.size() == 2  // can only be [preop, opnd]
             // other possibility is [..., op, preop, opnd]
             || (stack.size() >= 3 && stack.get(2).istoken);
   }

   // has_postop: check if top of stack is ++ or --
   public static boolean has_postop (ArrayList<AST> stack) {
      if (stack.size() == 0) // can't happen
         return false;
      AST optr = stack.get(0);
      if (!optr.istoken)
         return false;
      return is_postop(optr);
   }

   public static boolean is_postop (AST optr) {
      return (optr.getToken() == tokens.PlusPlus ||
              optr.getToken() == tokens.MinusMinus);
   }

   public static int javaintval(tokens t) { return t.ordinal(); }

   // N.B. Bang is the last token
   public static byte[] precedences = new byte[javaintval(tokens.Bang)+1];
   public static byte[] rightassoc = new byte[javaintval(tokens.Bang)+1];

   public static void initializeOptrPrecs () {
      precedences[javaintval(tokens.QuestionMark)] = 1;
      precedences[javaintval(tokens.Colon)] = 1;
      precedences[javaintval(tokens.Cons)] = 2;
      precedences[javaintval(tokens.LogicalOr)] = 3;
      precedences[javaintval(tokens.LogicalAnd)] = 4;
      precedences[javaintval(tokens.BitOr)] = 5;
      precedences[javaintval(tokens.Caret)] = 6;
      precedences[javaintval(tokens.Ampersand)] = 7;
      precedences[javaintval(tokens.EqEq)] = 8;
      precedences[javaintval(tokens.NotEq)] = 8;
      precedences[javaintval(tokens.LessEq)] = 8;
      precedences[javaintval(tokens.GreaterEq)] = 8;
      precedences[javaintval(tokens.Less)] = 8;
      precedences[javaintval(tokens.Greater)] = 8;
      precedences[javaintval(tokens.LessLess)] = 9;
      precedences[javaintval(tokens.GtGt)] = 9;
      precedences[javaintval(tokens.Plus)] = 10;
      precedences[javaintval(tokens.Minus)] = 10;
      precedences[javaintval(tokens.Star)] = 11;
      precedences[javaintval(tokens.Slash)] = 11;
      precedences[javaintval(tokens.Mod)] = 11;
      precedences[javaintval(tokens.Equal)] = 1;
      precedences[javaintval(tokens.PlusEqual)] = 0;
      precedences[javaintval(tokens.MinusEqual)] = 0;
      precedences[javaintval(tokens.StarEqual)] = 0;
      precedences[javaintval(tokens.SlashEqual)] = 0;
      precedences[javaintval(tokens.AmpersandEqual)] = 0;
      precedences[javaintval(tokens.BitOrEqual)] = 0;
      precedences[javaintval(tokens.CaretEqual)] = 0;
      precedences[javaintval(tokens.ModEqual)] = 0;
      precedences[javaintval(tokens.LessLessEqual)] = 0;
      precedences[javaintval(tokens.GtGtEqual)] = 0;

      rightassoc[javaintval(tokens.QuestionMark)] = 0;
      rightassoc[javaintval(tokens.Colon)] = 0;
      rightassoc[javaintval(tokens.Cons)] = 1;
      rightassoc[javaintval(tokens.LogicalOr)] = 0;
      rightassoc[javaintval(tokens.LogicalAnd)] = 0;
      rightassoc[javaintval(tokens.BitOr)] = 0;
      rightassoc[javaintval(tokens.Caret)] = 0;
      rightassoc[javaintval(tokens.Ampersand)] = 0;
      rightassoc[javaintval(tokens.EqEq)] = 0;
      rightassoc[javaintval(tokens.NotEq)] = 0;
      rightassoc[javaintval(tokens.LessEq)] = 0;
      rightassoc[javaintval(tokens.GreaterEq)] = 0;
      rightassoc[javaintval(tokens.Less)] = 0;
      rightassoc[javaintval(tokens.Greater)] = 0;
      rightassoc[javaintval(tokens.LessLess)] = 0;
      rightassoc[javaintval(tokens.GtGt)] = 0;
      rightassoc[javaintval(tokens.Plus)] = 0;
      rightassoc[javaintval(tokens.Minus)] = 0;
      rightassoc[javaintval(tokens.Star)] = 0;
      rightassoc[javaintval(tokens.Slash)] = 0;
      rightassoc[javaintval(tokens.Mod)] = 0;
      rightassoc[javaintval(tokens.Equal)] = 1;
      rightassoc[javaintval(tokens.PlusEqual)] = 1;
      rightassoc[javaintval(tokens.MinusEqual)] = 1;
      rightassoc[javaintval(tokens.StarEqual)] = 1;
      rightassoc[javaintval(tokens.SlashEqual)] = 1;
      rightassoc[javaintval(tokens.AmpersandEqual)] = 1;
      rightassoc[javaintval(tokens.BitOrEqual)] = 1;
      rightassoc[javaintval(tokens.CaretEqual)] = 1;
      rightassoc[javaintval(tokens.ModEqual)] = 1;
      rightassoc[javaintval(tokens.LessLessEqual)] = 1;
      rightassoc[javaintval(tokens.GtGtEqual)] = 1;
   }

   public static int precedence (AST optr1, AST optr2) {
      if (is_postop(optr1)) return 1;
      if (is_postop(optr2)) return -1;
      tokens op1 = optr1.getToken(),
             op2 = optr2.getToken();
      int op1p = precedences[javaintval(op1)],
          op2p = precedences[javaintval(op2)];
      if (op1p < op2p)
         return -1;
      else if (op1p > op2p)
         return 1;
      else
         return 0;
   }

   // Compare precedence of unary prefix operator optr1 to binary
   //   operator or postfix operator optr2
   // Rules are simple:  prefix operator has higher precedence than
   //   any binary operator, but lower than postfix operators.
   // Return -1 if prefix optr has lower precedence than next optr,
   //    +1 o.w.
   public static int unary_precedence (AST optr1, AST optr2) {
      tokens op1 = optr1.getToken(),
             op2 = optr2.getToken();
      if (op2 == tokens.PlusPlus || op2 == tokens.MinusMinus)
         return -1;
      else
         return 1;
   }

   public static int isRightAssoc (AST optr) {
      tokens op = optr.getToken();
      return rightassoc[javaintval(op)];
   }
}
  
         
