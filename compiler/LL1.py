
# removepred(rhs) - return rhs with the predicate removed, if
#   it has one.  Note that rhs's are never empty lists.
listtype = type([])
def removepred (rhs):
  if type(rhs[0]) == listtype:
     return rhs[1:]
  else:
     return rhs

def iterateFandF (vars) :
  nonterms = vars[0]
  grammar = vars[1]
  firsts = vars[2]
  follows = vars[3]
  changed = False
  for nonterm in grammar:
     A = nonterm[0]
     rhslist = nonterm[1]
     newfirsts = []
     newfollows = []
     for rhs in rhslist:
        rhs = removepred(rhs)
	newfirsts = join(newfirsts, first(rhs, nonterms, firsts, follows))
     if "" in firsts[A]:
        newfirsts = join(newfirsts, follows[A])
     if len(newfirsts) > len(firsts[A]):
	firsts[A] = newfirsts
	changed = True
     for rhs in rhslist:
        rhs = removepred(rhs)
        n = len(rhs)
	for i in range(0,n-1):
           if rhs[i] in nonterms:
              newfollows = join(follows[rhs[i]], first(rhs[i+1:],
		                               nonterms, firsts, follows))
              if len(newfollows) > len(follows[rhs[i]]):
                 follows[rhs[i]] = newfollows
                 changed = True
              if "" in first(rhs[i+1:], nonterms, firsts, follows):
                 newfollows = join(follows[rhs[i]], follows[A])
                 if len(newfollows) > len(follows[rhs[i]]):
                    follows[rhs[i]] = newfollows
                    changed = True
        if rhs[n-1] in nonterms:
           newfollows = join(follows[rhs[n-1]], follows[A])
           if len(newfollows) > len(follows[rhs[n-1]]):
              follows[rhs[n-1]] = newfollows
              changed = True
  return changed

def first (rhs, nonterms, firsts, follows):
  # Calculate first(X), where X a list of grammar symbols
  if len(rhs) == 0:
     return [""]
  syms = []
  for sym in rhs:
    # If we've gotten here, all preceding symbols in this rhs had
    # epsilon in first set, so need to add this symbols first set
    if sym in nonterms:
       # For nonterms, add known elements of first set
       syms = join(firsts[sym], syms)
       # If epsilon not in first set of this symbol, then we do
       # not consider the rest of the rhs.
       if "" not in firsts[sym]:
	  return syms
    else:
       # Terminals preclude having epsilon in first of this rhs.
       syms = join([sym], syms)
       return syms
    # Even if epsilon is in first set of this symbol, it still
    # isn't necessarily in first of this rhs.
    syms = remove("", syms)
  # If we've gotten here, "" was in firsts of every symbol,
  # so it is in first set of this rhs.
  return join([""], syms)


def FandF():
    vars = init()
    changed = True
    while changed == True:
       changed = iterateFandF(vars)
    nonterms = vars[0]
    follows = vars[3]
    for nonterm in nonterms:
       if "" in follows[nonterm]:
         follows[nonterm].remove("")
    #print vars[0]
    #print vars[1]
    #print vars[2]
    #print vars[3]
    #print
    return vars

def join (L1, L2):
    if L1 == []:
        return L2
    L3 = join(L1[1:], L2)
    if L1[0] in L2:
        return L3
    return [L1[0]] + L3

def remove(x, L):
    if L==[]:
        return L
    if x==L[0]:
        return L[1:]
    return [L[0]] + remove(x, L[1:])

def intersection (L1, L2):
   if L1 == [] or L2 == []:
      return []
   if L1[0] in L2:
      return [L1[0]] + intersection(L1[1:], remove(L1[0], L2))
   else:
      return intersection(L1[1:], L2)

def checkLL1 ():
  tables = FandF()
  nonterms = tables[0]
  grammar = tables[1]
  firsts = tables[2]
  follows = tables[3]
  isLL1 = 1
  for rule in grammar:
     lhs = rule[0]
     rhss = map(removepred, rule[1])
     firstsets = map (lambda r: first(r, nonterms, firsts, follows),
		     rhss)
     for i in range(0, len(rhss)-1):
        rhs1 = rhss[i]
        f1 = firstsets[i]
        follow = []
	if "" in f1: follow = follows[lhs]
        for j in range(i+1, len(rhss)):
	    rhs2 = rhss[j]
	    f2 = firstsets[j]
	    o = intersection(f1,f2)
	    if o != []:
               print "Conflict ", str(isLL1)+":", lhs, " -> ", rhs1,
	       print " and ", rhs2, " share ", o
	       isLL1 += 1
	    o = intersection(follow, f2)
	    if o != []:
               print "Conflict ", str(isLL1)+":", lhs, " -> eps by ", rhs1,
	       print ", and ", rhs2, "and follow(", lhs, ") share ", o
	       isLL1 += 1
  if isLL1 > 1:
     print "Grammar has ", isLL1-1, "conflicts"
  else:
     print "Grammar is LL1"
  return

def printgrammar (file):
   f = open(file, "w")
   data = init()
   grammar = data[1]
   for nontermrules in grammar:
       nonterm = nontermrules[0]
       rules = nontermrules[1]
       print >>f, nonterm, "\\> $\\rightarrow$ \\>" , 
       rule = rules[0]
       pred = ""
       if type(rule[0]) == listtype:
          pred = rule[0][0]
	  rule = rule[1:]
       for sym in rule:
          if sym == "":
             print >>f, "$\\epsilon$" ,
	  else:
             print >>f, sym ,
       if pred != "":
             print >>f, "\\ \\ [", pred, "]"
       print >> f, "\\\\"
       for rule in rules[1:]:
          pred = ""
          if type(rule[0]) == listtype:
             pred = rule[0][0]
	     rule = rule[1:]
          print >>f, "\\> $|$ \\>" ,
          for sym in rule:
             if type(sym) == listtype: pred = sym[0]; break
	     print>>f,  sym ,
          if pred != "":
             print >>f, "\\ \\ [", pred, "]"
          print >>f, "\\\\"
       print >>f, "\\\\"
   return

def printgrammartotext (file):
   f = open(file, "w")
   data = init()
   grammar = data[1]
   for nontermrules in grammar:
       nonterm = nontermrules[0]
       rules = nontermrules[1]
       print >>f, nonterm, " ::= ",
       rule = rules[0]
       pred = ""
       if type(rule[0]) == listtype:
          pred = rule[0][0]
	  rule = rule[1:]
       for sym in rule:
          if sym != "":
             print >>f, sym,
       if pred != "":
             print >>f, " [", pred, "]"
       else:
             print >>f
       for rule in rules[1:]:
          pred = ""
          if type(rule[0]) == listtype:
             pred = rule[0][0]
	     rule = rule[1:]
          print >>f, "          |  " ,
          for sym in rule:
             if type(sym) == listtype: pred = sym[0];
	     else: print>>f,  sym ,
          if pred != "":
             print >>f, "  [", pred, "]"
          else:
             print >>f
       print >>f, ""
   return

def genparser (file):
  f = open(file, "w")

  tables = FandF()
  nonterms = tables[0]
  grammar = tables[1]
  firsts = tables[2]
  follows = tables[3]

  print >>f, "public enum nonterms {"
  for t in nonterms: print >>f, t, ","
  print >>f, "}\n"

  for nontermrules in grammar:
     nonterm = nontermrules[0]
     rhsides = nontermrules[1]
     print >>f, "   public CST parse"+nonterm, "()"
     print >>f, "       throws SyntaxError"
     print >>f, "   {"
     print >>f, "      ArrayList<CST> children = new ArrayList<CST>();"
     print >>f, "      CST t; Object tokval;"
     print >>f, "      int startpos = thetoken.startloc;"
     #print >>f, "System.out.println(\"", nonterm, ", \" + tok );"
     print >>f, "      if (false) ; "
     for rhs in rhsides:
        pred = ""
	if type(rhs[0]) == listtype:
           pred = rhs[0][0]
	   rhs = removepred(rhs)
        fset = first(rhs, nonterms, firsts, follows)
        if "" in fset:
           fset.remove("")
           fset = join(fset, follows[nonterm])
	if fset == []:
           print "Empty fset: ", nonterm, pred, rhs
	if pred == "":
           cond = eqtest(fset)
	else:
           cond = "(" + eqtest(fset) + ") && (" + pred + ")"
	print >>f, "    else if (", cond, ") {\n         ",
	for sym in rhs:
	   if sym in nonterms:
              print >>f, "         t = parse"+sym, "();"
              print >>f, "         children.add(t);"
           elif sym == "":  # In this case, rhs = [Empty]
              print >>f, "         ;"
	   else:
              print >>f, "         if (tok != tokens.", sym,
	      print >>f, ") parseerror(\""+nonterm+"\", startpos);"
	      print >>f, "else {"
	      #print >>f, "tokval = toklis.getvalue();"
	      print >>f, "tokval = thetoken.tokvalue;"
	      print >>f, "children.add(new CST(tok, tokval, ",
	      print >>f, "thetoken.startloc, thetoken.endloc));"
	      print >>f, "prevtoken = thetoken;"
	      print >>f, "thetoken = toklis.nexttoken();"
	      print >>f, "tok = thetoken.toktype;"
	      print >>f, "}"
	print >>f, "return new CST(nonterms.", nonterm, ", children, ",
	print >>f, "startpos, startpos>prevtoken.endloc \
                    ? startpos : prevtoken.endloc);"
	print >>f, "\n      }"
     print >>f, "else parseerror(\""+nonterm+"\", startpos);"
     print >>f, "return null;"
     print >>f, "}\n"
  return

def eqtest (syms):
  test = "tok == tokens."+syms[0]
  for sym in syms[1:]:
     test += "|| tok == tokens."+sym
  return test

def init () :
# Remaining issues:
#  (1) Haven't used instanceof.  (Need to make "type" an expression.)
#      Note that as a binary optr, instanceof has same precedence
#      as relational operators (<, >).
	
# Grammar is LL(1) with the following exceptions.  We have noted how
# these are handled:

# Conflict  1: typeOpt  -> eps by  [''] ,
#     and  ['type'] and follow( typeOpt ) share  ['Identifier']
# ==> Use another lookahead - if it is ident or "<", then this
#     is a type.

# Conflict  2: elseStmtOpt  -> eps by  [''],
#     and  ['Else', 'statement'] and follow( elseStmtOpt ) share  ['Else']
# ==> "Shift" when "else" in the input; accomplished by adding
#     "tok != tokens.Else" as condition for empty production.

# Conflict 3: statement  ->  ['block']
#     and ['expression', 'Semicolon']  share ['Lbrace']
# ==> Don't allow list expression using braces as statement;
#     accomplished by simply place the "block" production first.

# Conflict  4: statement  ->  ['Identifier', 'Colon', 'statement']
#     and  ['localVariableDeclarationStatement']  share  ['Identifier']
# Conflict  5: statement  ->  ['Identifier', 'Colon', 'statement'] 
#     and  ['expression', 'Semicolon']  share  ['Identifier']
# Conflict  6: statement  ->  ['localVariableDeclarationStatement']
#     and  ['expression', 'Semicolon']  share  ['Identifier']
# ==> The above three productions all deal with identifiers occurring
#     as the first symbol of a statement.  Resolved by checking one
#     more lookahead:  if colon, it's a labeled statement; if
#     identifier or "<", it's a type declaration.  Thus, add
#     "toklis.peek(0) == tokens.Colon" to labeled statement, and add
#     "toklis.peek(0) == tokens.Identifier || toklis.peek(0) == tokens.Less"
#     to LocalVariable Declaration.

# Conflict  7: forstatement  ->  ['type', 'typeFollower']
#     and  ['typeFollower']  share  ['Identifier']

# Conflict  8: typeFollower  ->
#               ['Identifier', 'Colon', 'expression', 'Rparen', 'statement']
#        and  ['expressionList', 'Semicolon', 'expressionOpt', 'Semicolon',
#                'expressionList', 'Rparen', 'statement']
#        share  ['Identifier']
# ==> look ahead for colon

# Conflict 9: excTypeOpt  -> eps by  [''] ,
#     and  ['type'] and follow( excTypeOpt) share  ['Identifier']
# ==> Problem is because catch clause can start catch (id) or
#     catch (type id).  Resolve by looking past id for right parenthesis.

# Conflict 10: catchListOpt  -> eps by  [''] ,
#     and  ['catchList'] and follow( catchListOpt ) share  ['Catch']
# ==> catch clause ends in statement; statements can be followed by
#     catch clause (because try statement is "try statement catch ...").
#     If catch clause is followed by "catch," is part of this sequence
#     of catches.  So resolve by adding "tok != tokens.Catch" to
#     empty production.

# Conflict  11: commaListElementList  ->  ['Comma']
#  and  ['Comma', 'listElement', 'commaListElementList']  share  ['Comma']
# ==> The first rule ('Comma') matches only when the comma is
#     followed by a left bracket or brace.

  # Tokens listed first, start with UC
  Empty = ""
  # Token categories
  Identifier = "Identifier"
  Intlit = "Intlit"
  Doublelit = "Doublelit"
  Charlit = "Charlit"
  Stringlit = "Stringlit"
  Antiquote = "Antiquote"
  # Keywords
  Bool = "Bool"
  Char = "Char"
  Int = "Int"
  Double = "Double"
  Else = "Else"
  If = "If"
  While = "While"
  Return = "Return"
  Break = "Break"
  Continue = "Continue"
  For = "For"
  TrueLit = "True"
  FalseLit = "False"
  Null = "Null"
  Extern = "Extern" 
  Global = "Global" 
  ExprSpliced = "__exprSpliced__"
  StmtSpliced = "__stmtSpliced__"
  UnitSpliced = "__unitSpliced__"
  NameSpliced = "__nameSpliced__"
  Restrict = "__restrict__"
  # Symbol tokens
  Dot = "Dot"
  DotStar = "DotStar"
  DotDot = "DotDot"
  Arrow = "Arrow"
  Star = "Star"
  Lbrace = "Lbrace"
  Rbrace = "Rbrace"
  Semicolon = "Semicolon"
  Colon = "Colon"
  Cons = "Cons"
  Comma = "Comma"
  Less = "Less"
  Greater = "Greater"
  Lparen = "Lparen"
  Rparen = "Rparen"
  Lbracket = "Lbracket"
  Rbracket = "Rbracket"
  Equal = "Equal"
  PlusEqual = "PlusEqual"
  MinusEqual = "MinusEqual"
  StarEqual = "StarEqual"
  SlashEqual = "SlashEqual"
  AmpersandEqual = "AmpersandEqual"
  BitOrEqual = "BitOrEqual"
  Ampersand = "Ampersand"
  BitOr = "BitOr"
  CaretEqual = "CaretEqual"
  ModEqual = "ModEqual"
  LessLessEqual = "LessLessEqual"
  GtGtEqual = "GtGtEqual"
  QuestionMark = "QuestionMark"
  LogicalOr = "LogicalOr"
  LogicalAnd = "LogicalAnd"
  Or = "Or"
  Caret = "Caret"
  EqEq = "EqEq"
  NotEq = "NotEq"
  And = "And"
  LessEq = "LessEq"
  GreaterEq = "GreaterEq"
  Less = "Less"
  Greater = "Greater"
  LessLess ="LessLess"
  GtGt ="GtGt"
  Plus ="Plus"
  Minus ="Minus"
  Slash = "Slash"
  Mod = "Mod"
  PlusPlus = "PlusPlus"
  MinusMinus = "MinusMinus"
  Tilde = "Tilde"
  Bang = "Bang"
  Void = "Void"
  Noyb = "Noyb"
  Case = "Case"
  Default = "Default"
  Catch = "Catch"
  Try = "Try"
  Eof = "Eof"

  tokencats = [Identifier, Intlit, Doublelit, Charlit, Stringlit, Antiquote]
  keywords = [Bool, Char, Void,
              Int, Double, Else, If, While,
              Return, Break, Continue, For, TrueLit, FalseLit, Null,
              Extern, Global,
              ExprSpliced, StmtSpliced, UnitSpliced, NameSpliced,
              Restrict
              ]
  symbols = [Dot, DotStar, DotDot, Arrow, Star, Lbrace, Rbrace,
             Semicolon, Colon, Comma, Less, Greater, Lparen, Rparen,
             Lbracket, Rbracket, Equal, PlusEqual, MinusEqual, StarEqual,
             SlashEqual, AmpersandEqual, BitOrEqual, CaretEqual, ModEqual,
             BitOr, Ampersand,
             LessLessEqual, GtGtEqual, QuestionMark,
             LogicalOr, LogicalAnd, Or, Caret, EqEq, NotEq, And, LessEq,
             GreaterEq, Less, Greater, LessLess, GtGt, Plus,
             Minus, Slash, Mod, PlusPlus, MinusMinus, Tilde, Bang]
  tokens = tokencats + keywords + symbols
  
  # Nonterms start with LC
  compilationUnit = "compilationUnit"
  declarationList = "declarationList"
  declaration = "declaration"
  declBegin = "declBegin"
  declCont = "declCont"
  arrayTypeOpt = "arrayTypeOpt"
  arraySize = "arraySize"
  emptyArrayTypeOpt = "emptyArrayTypeOpt"
  type = "type"
  typeOpt = "typeOpt"
  classType = "classType"
  primitiveType = "primitiveType"
  nonPointerType = "nonPointerType" 
  starList = "starList" 
  typeArgumentsOpt = "typeArgumentsOpt"
  typeArguments = "typeArguments"
  commaTypeList = "commaTypeList"
  formalParameters = "formalParameters"
  formalParameterDeclsOpt = "formalParameterDeclsOpt"
  formalParameterDecls = "formalParameterDecls"
  restrictOpt = "restrictOpt"
  typeIdentList = "typeIdentList"
  block = "block"
  statementList = "statementList"
  localVariableDeclarationStatement = "localVariableDeclarationStatement"
  localVariableDeclaration = "localVariableDeclaration"
  variableDeclaratorList = "variableDeclaratorList"
  variableDeclarator = "variableDeclarator"
  variableInitializerOpt = "variableInitializerOpt"
  constInitializerOpt = "constInitializerOpt"
  colonExprOpt = "colonExprOpt"
  elseStmtOpt = "elseStmtOpt"
  statement = "statement"
  forstatement = "forstatement"
  forInit = "forInit"
  forInitOpt = "forInitOpt"
  nonlistPrimary = "nonlistPrimary"
  expressionList = "expressionList"
  commaExprList = "commaExprList"
  parExpression = "parExpression"
  identifierOpt = "identifierOpt"
  expressionOpt = "expressionOpt"
  expression = "expression"
  exprContOpt = "exprContOpt"
  exprCont = "exprCont"
  listExpression = "listExpression"
  listElementListOpt = "listElementListOpt"
  listElement = "listElement"
  constExpression = "constExpression"
  commaListElementList = "commaListElementList"
  binop = "binop"
  consop = "consop"
  logicalOp = "logicalOp"
  bitwiseOp = "bitwiseOp"
  comparisonOp = "comparisonOp"
  shiftOp = "shiftOp"
  arithOp = "arithOp"
  assignmentOp = "assignmentOp"
  unaryOp = "unaryOp"
  incrOptr = "incrOptr"
  primary = "primary"
  argumentsOpt = "argumentsOpt"
  arguments = "arguments"
  literal = "literal"
  simplePrimary = "simplePrimary"
  primarySuffix = "primarySuffix"
  typeFollower = "typeFollower"
  name = "name"

  grammar = [
  [compilationUnit, [
      [declarationList]
  ]],
      
  [declarationList, [
      [["tok == tokens.Eof"], Empty],
      [declaration, declarationList]
  ]],
       
  [declaration, [
      # Function or global var
      [declBegin, declCont],
      # Function decl
      [UnitSpliced, Antiquote],
      # global var decl
      [Extern, declBegin, emptyArrayTypeOpt, Semicolon], 
  ]],
               
  [declBegin, [
      [type, name]
  ]],
                
  [declCont, [           
      [formalParameters, block],
      [arrayTypeOpt, constInitializerOpt, Semicolon]
  ]],

  [arrayTypeOpt, [
      [Empty],
      [Lbracket, arraySize, Rbracket]
  ]],
                  
  [arraySize, [
      [Intlit],
      [ExprSpliced, Antiquote]
  ]],
                  
  [emptyArrayTypeOpt, [
      [Empty],
      [Lbracket, Rbracket]
  ]],

  [constInitializerOpt, [
      [Empty],
      [Equal, constExpression]
  ]],

  [type, [
      [nonPointerType, starList]
  ]], 
             
  [nonPointerType, [
      [classType],
      [primitiveType]
  ]],
  
  [classType, [
      [name]
  ]],

  [primitiveType, [
      [Bool],
      [Char],
      [Int],
      [Double],
      [Void]
  ]],
  
  [starList, [
      [["tok != tokens.Star"], Empty],
      [Star, starList]
  ]],

  [formalParameters, [
      [Lparen, formalParameterDeclsOpt, Rparen]
  ]],

  [formalParameterDeclsOpt, [
      [Empty],
      [formalParameterDecls]
  ]],

  [formalParameterDecls, [
      [type, restrictOpt, name, emptyArrayTypeOpt, typeIdentList]
  ]],

  [restrictOpt, [
      [Empty],
      [Restrict]
  ]],
  
#  [typeOpt, [
#      [["toklis.peek(0) != tokens.Identifier" +
#        "&&toklis.peek(0) != tokens.Less"], Empty],
#      [type]
#  ]],

  [typeIdentList, [
      [Empty],
      [Comma, type, restrictOpt, name, emptyArrayTypeOpt, typeIdentList]
  ]],

  [block, [
      [Lbrace, statementList, Rbrace]
  ]],

  [statementList, [
      [Empty],
      [statement, statementList]
  ]],

  [localVariableDeclarationStatement, [
      [localVariableDeclaration, Semicolon]
  ]],

  [localVariableDeclaration, [
      [type, restrictOpt, variableDeclarator, variableDeclaratorList]
  ]],

  [variableDeclaratorList, [
      [Empty],
      [Comma, variableDeclarator, variableDeclaratorList]
  ]],

  [variableDeclarator, [
    [name, arrayTypeOpt, variableInitializerOpt]
  ]],

  [variableInitializerOpt, [
      [Empty],
      [Equal, expression]
  ]],

  [elseStmtOpt, [
      [["tok != tokens.Else"], Empty],
      [Else, statement]
  ]],

  [statement, [
      [["tok == tokens.Lbrace"], block],
      [["toklis.peek(0) == tokens.Identifier" +
        "||toklis.peek(0) == tokens.Star"+
        "||toklis.peek(0) == tokens.__restrict__"+
        "||toklis.peek(0) == tokens.__nameSpliced__"],
	                   localVariableDeclarationStatement],
      [If, parExpression, statement, elseStmtOpt],
      [For, Lparen, forstatement],
      [While, parExpression, statement],
      [Return, expressionOpt, Semicolon],
      [Break, identifierOpt, Semicolon],
      [Continue, identifierOpt, Semicolon],
      [expression, Semicolon],
      [Semicolon],
      [StmtSpliced, Antiquote]
  ]],

  [forstatement, [
      [["toklis.peek(0) == tokens.Identifier" +
        "||toklis.peek(0) == tokens.__nameSpliced__"], type, typeFollower],
      [typeFollower]
  ]],

  [typeFollower, [
      [expressionList, Semicolon, expressionOpt, Semicolon,
          expressionList, Rparen, statement]
  ]],

  [expressionList, [
      [Empty],
      [expression, commaExprList]
  ]],
  
  [commaExprList, [
      [Empty],
      [Comma, expression, commaExprList]
  ]],
  
  [parExpression, [
      [Lparen, expression, Rparen]
  ]],
  
  [identifierOpt, [
      [Empty],
      [Identifier]
  ]],
  
  [expressionOpt, [
      [Empty],
      [expression]
  ]],

  [expression, [
      [primary, exprContOpt],
      [unaryOp, expression]
  ]],

  [exprContOpt, [
      [Empty],
      [exprCont]
  ]],

  [exprCont, [
      [binop, expression],
      [incrOptr, exprContOpt],
      [Lbracket, expression, Rbracket, exprContOpt],
  ]],
  
  [primary, [
      [simplePrimary, primarySuffix]
      #[unaryOp, primary],
  ]],

  [simplePrimary, [
      [listExpression],
      [parExpression],
      [name, argumentsOpt], # function call
      [literal],
      [ExprSpliced, Antiquote]
  ]],

  [argumentsOpt, [
      [Empty],
      [arguments]
  ]],

  [arguments, [
      [Lparen, expressionList, Rparen],
  ]],
             
  [primarySuffix, [
      [Empty],
	    [Dot, Identifier, primarySuffix],
	    [listExpression, primarySuffix]
  ]],
  
  [listExpression, [
      [Lbrace, listElementListOpt, Rbrace],
  ]],

  [listElementListOpt, [
      [Empty],
	   [listElement, commaListElementList],
      [["toklis.peek(0) == tokens.Rbrace"], Comma]
  ]],

  [commaListElementList, [
      [Empty],
      [["toklis.peek(0) == tokens.Rbrace"], Comma],
      [Comma, listElement, commaListElementList]
  ]],

  [listElement, [
      [constExpression]
  ]],
  
  [constExpression, [
      [literal],
      [Minus, literal], #to handle -123, -45.09, etc.
      [ExprSpliced, Antiquote],
      [listExpression]
  ]],
  
  [literal, [
      [Intlit],
      [Doublelit],
      [Charlit],
      [Stringlit],
      [TrueLit],
      [FalseLit],
      [Null]
  ]],

  [binop, [
      [assignmentOp],
      [logicalOp],
      [comparisonOp],
      [arithOp]
  ]],

  [logicalOp, [
      [LogicalOr], 
      [LogicalAnd]
  ]],

  [comparisonOp, [
      [EqEq], [NotEq], [LessEq], [GreaterEq], [Less], [Greater]
  ]],

  [arithOp, [
      [Plus], [Minus], [Star], [Slash], [Mod]
  ]],
  
  [assignmentOp, [
      [Equal], [PlusEqual], [MinusEqual], [StarEqual],
      [SlashEqual], [ModEqual] 
  ]],

  [unaryOp, [
      [Plus],
      [Star],
      [Minus],
      [Bang],
      [Ampersand],
      [PlusPlus],
      [MinusMinus]
  ]],
  
  [incrOptr, [
      [PlusPlus],
      [MinusMinus]
  ]],

  [name, [
      [Identifier],
      [NameSpliced, Antiquote]
  ]]
  ]



  nonterms = map(lambda l: l[0], grammar)
  firsts = dict()
  follows = dict()
  for sym in nonterms:
	  firsts[sym] = []
	  follows[sym] = []
  follows[compilationUnit] = [Eof]
  follows[statementList] = [Eof]
  return [nonterms, grammar, firsts, follows]
