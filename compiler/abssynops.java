// Each abstract syntax operator has certain expected arguments.
// The AST type is very generic - each node either has an operator (one
// of these), an arithmetic operator (if AST operaot is binaryop, unarypreoptr,
// or unarypostoptr), and a list of AST nodes as children, or it has a token.
// The following says what kind of arguments are expected.  The notation
// is this;
//   - If an item is listed as optional, that means it may be absent;
//     either the list of which it is a part has one fewer element,
//     or the operator has a zero-element list.  (The optional element
//     is always the last child.)
//   - If an item's children are listed as "AST's", then it has
//     a variable number of children; otherwise, it has exactly
//     the number of children listed (except where one is optional,
//     as noted above).
//   - "null" is never used for the case when a node has no children;
//     instead, a zero-element list is inserted.
//   - The types in parentheses after AST are the expected type of
//     that child.  E.g. "ident" means the child should be a token
//     node; "qualifiedName" means that should be the abssynop of
//     that child.  If the name is an abssynops, or a token, then
//     the listed item has that abssynop or that token.  If it
//     is a category of AST's, then here are the possibilities:
//
//     statement: stmtlist, exprstmt, vardecl, labeledstmt, assertstmt,
//     ifstmt, whilestmt, dowhilestmt, returnstmt,
//     breakstmt, continuestmt, trystmt, foriterator, forloop
//     (The latter two are two forms of "for" statement.)
//
//     expression: literal, nameorfunlist, unarypreoptr,
//     unarypostoptr, binaryop, condexpr, listexpr, dictexpr,
//     mappair, rangeexpr (<- these two can only appear within
//     a list context)
//     (I believe an Identifier acts as an expression also, in
//     some circumstances - need to check on this.)
//
public enum abssynops {
   list,          // Used only as temporary name to return multiple
	          // values from trans; does not survive in full AST.
   compUnit,  
   globalDecl,
   externDecl,
   function,       
   formals,       // AST's (var)
   atype,         // AST (ident or prim type), then type params
   arrayType,
   var,           // AST (atype) * AST (ident)  (use noyb for atype if absent)
   funcParam,
   stmtlist,      // AST's (stmt)
   scopedStmtList,
   exprstmt,      // AST (expr)
   vardecl,       // AST (var) * AST (expr) (optional)
   ifstmt,        // AST (expr) * AST (stmt) * AST (stmt)
   whilestmt,     // AST (expr) * AST (stmt) * AST(ident) (optional)
   returnstmt,    // AST (expr) (optional)
   breakstmt,     // AST (ident) (optional)
   continuestmt,  // AST (ident) (optional)
   foriterator,   // AST (var) * AST (expression) * AST(stmt)
                  //                              * AST(ident) (optional)
   exprlist,      // AST's (expr)
   litral,        // AST (literal)
   aquote,        // AST (literal)  - can be stmt, expr, ...
   ident,         // token AST
   funcall,       // AST (ident) * AST (exprlist) (possibly empty)
   nameorargslist, // AST's - expr followed by names (i.e. Identifier
                  // tokens) or exprlists (arguments) or listexprs (subscripts)
   unarypreoptr,  // AST (Optr) * AST (expr)
   unarypostoptr, // AST (Optr) * AST (expr)
   binaryop,         // AST (Optr) * AST (expr) * AST (expr)
   listexpr,      // AST's (expr)
   arrayexpr,     // AST (expr) * AST (expr)
   arrayindex,    // AST (expr)
}
