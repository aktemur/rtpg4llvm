import java.util.ArrayList;
//using System.Collections.Generic;

public class Parser {

   public TokenList toklis;
   public tokens tok;
   public Token thetoken, prevtoken;
   public CharStream cs;

   public Parser (TokenList toklis, CharStream cs) {
      this.toklis = toklis;
      this.cs = cs;
      thetoken = toklis.nexttoken();
      prevtoken = new Token(tokens.Identifier, null, 1, 1);
      // Doesn't matter what type of token prevtoken is to
      // start - it is never looked at.  This is just to
      // something there, to avoid null test.
      tok = thetoken.toktype;
   }

   public void parseerror (String nonterm, int startpos)
   throws SyntaxError
   {
      throw new SyntaxError(nonterm, prevtoken, startpos);
   }

   // START of generated parser code here

   public CST parsecompilationUnit ()
   throws SyntaxError
{
  ArrayList<CST> children = new ArrayList<CST>();
  CST t; Object tokval;
  int startpos = thetoken.startloc;
  if (false) ; 
else if ( tok == tokens.Identifier|| tok == tokens.__nameSpliced__|| tok == tokens.Bool|| tok == tokens.Char|| tok == tokens.Int|| tok == tokens.Double|| tok == tokens.Void|| tok == tokens.__unitSpliced__|| tok == tokens.Extern|| tok == tokens.Eof ) {
               t = parsedeclarationList ();
     children.add(t);
return new CST(nonterms. compilationUnit , children,  startpos, startpos>prevtoken.endloc                     ? startpos : prevtoken.endloc);

  }
else parseerror("compilationUnit", startpos);
return null;
}

public CST parsedeclarationList ()
   throws SyntaxError
{
  ArrayList<CST> children = new ArrayList<CST>();
  CST t; Object tokval;
  int startpos = thetoken.startloc;
  if (false) ; 
else if ( (tok == tokens.Eof) && (tok == tokens.Eof) ) {
               ;
return new CST(nonterms. declarationList , children,  startpos, startpos>prevtoken.endloc                     ? startpos : prevtoken.endloc);

  }
else if ( tok == tokens.Identifier|| tok == tokens.__nameSpliced__|| tok == tokens.Bool|| tok == tokens.Char|| tok == tokens.Int|| tok == tokens.Double|| tok == tokens.Void|| tok == tokens.__unitSpliced__|| tok == tokens.Extern ) {
               t = parsedeclaration ();
     children.add(t);
     t = parsedeclarationList ();
     children.add(t);
return new CST(nonterms. declarationList , children,  startpos, startpos>prevtoken.endloc                     ? startpos : prevtoken.endloc);

  }
else parseerror("declarationList", startpos);
return null;
}

public CST parsedeclaration ()
   throws SyntaxError
{
  ArrayList<CST> children = new ArrayList<CST>();
  CST t; Object tokval;
  int startpos = thetoken.startloc;
  if (false) ; 
else if ( tok == tokens.Identifier|| tok == tokens.__nameSpliced__|| tok == tokens.Bool|| tok == tokens.Char|| tok == tokens.Int|| tok == tokens.Double|| tok == tokens.Void ) {
               t = parsedeclBegin ();
     children.add(t);
     t = parsedeclCont ();
     children.add(t);
return new CST(nonterms. declaration , children,  startpos, startpos>prevtoken.endloc                     ? startpos : prevtoken.endloc);

  }
else if ( tok == tokens.__unitSpliced__ ) {
               if (tok != tokens. __unitSpliced__ ) parseerror("declaration", startpos);
else {
tokval = thetoken.tokvalue;
children.add(new CST(tok, tokval,  thetoken.startloc, thetoken.endloc));
prevtoken = thetoken;
thetoken = toklis.nexttoken();
tok = thetoken.toktype;
}
     if (tok != tokens. Antiquote ) parseerror("declaration", startpos);
else {
tokval = thetoken.tokvalue;
children.add(new CST(tok, tokval,  thetoken.startloc, thetoken.endloc));
prevtoken = thetoken;
thetoken = toklis.nexttoken();
tok = thetoken.toktype;
}
return new CST(nonterms. declaration , children,  startpos, startpos>prevtoken.endloc                     ? startpos : prevtoken.endloc);

  }
else if ( tok == tokens.Extern ) {
               if (tok != tokens. Extern ) parseerror("declaration", startpos);
else {
tokval = thetoken.tokvalue;
children.add(new CST(tok, tokval,  thetoken.startloc, thetoken.endloc));
prevtoken = thetoken;
thetoken = toklis.nexttoken();
tok = thetoken.toktype;
}
     t = parsedeclBegin ();
     children.add(t);
     t = parseemptyArrayTypeOpt ();
     children.add(t);
     if (tok != tokens. Semicolon ) parseerror("declaration", startpos);
else {
tokval = thetoken.tokvalue;
children.add(new CST(tok, tokval,  thetoken.startloc, thetoken.endloc));
prevtoken = thetoken;
thetoken = toklis.nexttoken();
tok = thetoken.toktype;
}
return new CST(nonterms. declaration , children,  startpos, startpos>prevtoken.endloc                     ? startpos : prevtoken.endloc);

  }
else parseerror("declaration", startpos);
return null;
}

public CST parsedeclBegin ()
   throws SyntaxError
{
  ArrayList<CST> children = new ArrayList<CST>();
  CST t; Object tokval;
  int startpos = thetoken.startloc;
  if (false) ; 
else if ( tok == tokens.Identifier|| tok == tokens.__nameSpliced__|| tok == tokens.Bool|| tok == tokens.Char|| tok == tokens.Int|| tok == tokens.Double|| tok == tokens.Void ) {
               t = parsetype ();
     children.add(t);
     t = parsename ();
     children.add(t);
return new CST(nonterms. declBegin , children,  startpos, startpos>prevtoken.endloc                     ? startpos : prevtoken.endloc);

  }
else parseerror("declBegin", startpos);
return null;
}

public CST parsedeclCont ()
   throws SyntaxError
{
  ArrayList<CST> children = new ArrayList<CST>();
  CST t; Object tokval;
  int startpos = thetoken.startloc;
  if (false) ; 
else if ( tok == tokens.Lparen ) {
               t = parseformalParameters ();
     children.add(t);
     t = parseblock ();
     children.add(t);
return new CST(nonterms. declCont , children,  startpos, startpos>prevtoken.endloc                     ? startpos : prevtoken.endloc);

  }
else if ( tok == tokens.Lbracket|| tok == tokens.Equal|| tok == tokens.Comma|| tok == tokens.Semicolon ) {
               t = parsearrayTypeOpt ();
     children.add(t);
     t = parseconstInitializerOpt ();
     children.add(t);
     if (tok != tokens. Semicolon ) parseerror("declCont", startpos);
else {
tokval = thetoken.tokvalue;
children.add(new CST(tok, tokval,  thetoken.startloc, thetoken.endloc));
prevtoken = thetoken;
thetoken = toklis.nexttoken();
tok = thetoken.toktype;
}
return new CST(nonterms. declCont , children,  startpos, startpos>prevtoken.endloc                     ? startpos : prevtoken.endloc);

  }
else parseerror("declCont", startpos);
return null;
}

public CST parsearrayTypeOpt ()
   throws SyntaxError
{
  ArrayList<CST> children = new ArrayList<CST>();
  CST t; Object tokval;
  int startpos = thetoken.startloc;
  if (false) ; 
else if ( tok == tokens.Equal|| tok == tokens.Comma|| tok == tokens.Semicolon ) {
               ;
return new CST(nonterms. arrayTypeOpt , children,  startpos, startpos>prevtoken.endloc                     ? startpos : prevtoken.endloc);

  }
else if ( tok == tokens.Lbracket ) {
               if (tok != tokens. Lbracket ) parseerror("arrayTypeOpt", startpos);
else {
tokval = thetoken.tokvalue;
children.add(new CST(tok, tokval,  thetoken.startloc, thetoken.endloc));
prevtoken = thetoken;
thetoken = toklis.nexttoken();
tok = thetoken.toktype;
}
     t = parsearraySize ();
     children.add(t);
     if (tok != tokens. Rbracket ) parseerror("arrayTypeOpt", startpos);
else {
tokval = thetoken.tokvalue;
children.add(new CST(tok, tokval,  thetoken.startloc, thetoken.endloc));
prevtoken = thetoken;
thetoken = toklis.nexttoken();
tok = thetoken.toktype;
}
return new CST(nonterms. arrayTypeOpt , children,  startpos, startpos>prevtoken.endloc                     ? startpos : prevtoken.endloc);

  }
else parseerror("arrayTypeOpt", startpos);
return null;
}

public CST parsearraySize ()
   throws SyntaxError
{
  ArrayList<CST> children = new ArrayList<CST>();
  CST t; Object tokval;
  int startpos = thetoken.startloc;
  if (false) ; 
else if ( tok == tokens.Intlit ) {
               if (tok != tokens. Intlit ) parseerror("arraySize", startpos);
else {
tokval = thetoken.tokvalue;
children.add(new CST(tok, tokval,  thetoken.startloc, thetoken.endloc));
prevtoken = thetoken;
thetoken = toklis.nexttoken();
tok = thetoken.toktype;
}
return new CST(nonterms. arraySize , children,  startpos, startpos>prevtoken.endloc                     ? startpos : prevtoken.endloc);

  }
else if ( tok == tokens.__exprSpliced__ ) {
               if (tok != tokens. __exprSpliced__ ) parseerror("arraySize", startpos);
else {
tokval = thetoken.tokvalue;
children.add(new CST(tok, tokval,  thetoken.startloc, thetoken.endloc));
prevtoken = thetoken;
thetoken = toklis.nexttoken();
tok = thetoken.toktype;
}
     if (tok != tokens. Antiquote ) parseerror("arraySize", startpos);
else {
tokval = thetoken.tokvalue;
children.add(new CST(tok, tokval,  thetoken.startloc, thetoken.endloc));
prevtoken = thetoken;
thetoken = toklis.nexttoken();
tok = thetoken.toktype;
}
return new CST(nonterms. arraySize , children,  startpos, startpos>prevtoken.endloc                     ? startpos : prevtoken.endloc);

  }
else parseerror("arraySize", startpos);
return null;
}

public CST parseemptyArrayTypeOpt ()
   throws SyntaxError
{
  ArrayList<CST> children = new ArrayList<CST>();
  CST t; Object tokval;
  int startpos = thetoken.startloc;
  if (false) ; 
else if ( tok == tokens.Semicolon|| tok == tokens.Comma|| tok == tokens.Rparen ) {
               ;
return new CST(nonterms. emptyArrayTypeOpt , children,  startpos, startpos>prevtoken.endloc                     ? startpos : prevtoken.endloc);

  }
else if ( tok == tokens.Lbracket ) {
               if (tok != tokens. Lbracket ) parseerror("emptyArrayTypeOpt", startpos);
else {
tokval = thetoken.tokvalue;
children.add(new CST(tok, tokval,  thetoken.startloc, thetoken.endloc));
prevtoken = thetoken;
thetoken = toklis.nexttoken();
tok = thetoken.toktype;
}
     if (tok != tokens. Rbracket ) parseerror("emptyArrayTypeOpt", startpos);
else {
tokval = thetoken.tokvalue;
children.add(new CST(tok, tokval,  thetoken.startloc, thetoken.endloc));
prevtoken = thetoken;
thetoken = toklis.nexttoken();
tok = thetoken.toktype;
}
return new CST(nonterms. emptyArrayTypeOpt , children,  startpos, startpos>prevtoken.endloc                     ? startpos : prevtoken.endloc);

  }
else parseerror("emptyArrayTypeOpt", startpos);
return null;
}

public CST parseconstInitializerOpt ()
   throws SyntaxError
{
  ArrayList<CST> children = new ArrayList<CST>();
  CST t; Object tokval;
  int startpos = thetoken.startloc;
  if (false) ; 
else if ( tok == tokens.Semicolon ) {
               ;
return new CST(nonterms. constInitializerOpt , children,  startpos, startpos>prevtoken.endloc                     ? startpos : prevtoken.endloc);

  }
else if ( tok == tokens.Equal ) {
               if (tok != tokens. Equal ) parseerror("constInitializerOpt", startpos);
else {
tokval = thetoken.tokvalue;
children.add(new CST(tok, tokval,  thetoken.startloc, thetoken.endloc));
prevtoken = thetoken;
thetoken = toklis.nexttoken();
tok = thetoken.toktype;
}
     t = parseconstExpression ();
     children.add(t);
return new CST(nonterms. constInitializerOpt , children,  startpos, startpos>prevtoken.endloc                     ? startpos : prevtoken.endloc);

  }
else parseerror("constInitializerOpt", startpos);
return null;
}

public CST parsetype ()
   throws SyntaxError
{
  ArrayList<CST> children = new ArrayList<CST>();
  CST t; Object tokval;
  int startpos = thetoken.startloc;
  if (false) ; 
else if ( tok == tokens.Identifier|| tok == tokens.__nameSpliced__|| tok == tokens.Bool|| tok == tokens.Char|| tok == tokens.Int|| tok == tokens.Double|| tok == tokens.Void ) {
               t = parsenonPointerType ();
     children.add(t);
     t = parsestarList ();
     children.add(t);
return new CST(nonterms. type , children,  startpos, startpos>prevtoken.endloc                     ? startpos : prevtoken.endloc);

  }
else parseerror("type", startpos);
return null;
}

public CST parsenonPointerType ()
   throws SyntaxError
{
  ArrayList<CST> children = new ArrayList<CST>();
  CST t; Object tokval;
  int startpos = thetoken.startloc;
  if (false) ; 
else if ( tok == tokens.Identifier|| tok == tokens.__nameSpliced__ ) {
               t = parseclassType ();
     children.add(t);
return new CST(nonterms. nonPointerType , children,  startpos, startpos>prevtoken.endloc                     ? startpos : prevtoken.endloc);

  }
else if ( tok == tokens.Bool|| tok == tokens.Char|| tok == tokens.Int|| tok == tokens.Double|| tok == tokens.Void ) {
               t = parseprimitiveType ();
     children.add(t);
return new CST(nonterms. nonPointerType , children,  startpos, startpos>prevtoken.endloc                     ? startpos : prevtoken.endloc);

  }
else parseerror("nonPointerType", startpos);
return null;
}

public CST parseclassType ()
   throws SyntaxError
{
  ArrayList<CST> children = new ArrayList<CST>();
  CST t; Object tokval;
  int startpos = thetoken.startloc;
  if (false) ; 
else if ( tok == tokens.Identifier|| tok == tokens.__nameSpliced__ ) {
               t = parsename ();
     children.add(t);
return new CST(nonterms. classType , children,  startpos, startpos>prevtoken.endloc                     ? startpos : prevtoken.endloc);

  }
else parseerror("classType", startpos);
return null;
}

public CST parseprimitiveType ()
   throws SyntaxError
{
  ArrayList<CST> children = new ArrayList<CST>();
  CST t; Object tokval;
  int startpos = thetoken.startloc;
  if (false) ; 
else if ( tok == tokens.Bool ) {
               if (tok != tokens. Bool ) parseerror("primitiveType", startpos);
else {
tokval = thetoken.tokvalue;
children.add(new CST(tok, tokval,  thetoken.startloc, thetoken.endloc));
prevtoken = thetoken;
thetoken = toklis.nexttoken();
tok = thetoken.toktype;
}
return new CST(nonterms. primitiveType , children,  startpos, startpos>prevtoken.endloc                     ? startpos : prevtoken.endloc);

  }
else if ( tok == tokens.Char ) {
               if (tok != tokens. Char ) parseerror("primitiveType", startpos);
else {
tokval = thetoken.tokvalue;
children.add(new CST(tok, tokval,  thetoken.startloc, thetoken.endloc));
prevtoken = thetoken;
thetoken = toklis.nexttoken();
tok = thetoken.toktype;
}
return new CST(nonterms. primitiveType , children,  startpos, startpos>prevtoken.endloc                     ? startpos : prevtoken.endloc);

  }
else if ( tok == tokens.Int ) {
               if (tok != tokens. Int ) parseerror("primitiveType", startpos);
else {
tokval = thetoken.tokvalue;
children.add(new CST(tok, tokval,  thetoken.startloc, thetoken.endloc));
prevtoken = thetoken;
thetoken = toklis.nexttoken();
tok = thetoken.toktype;
}
return new CST(nonterms. primitiveType , children,  startpos, startpos>prevtoken.endloc                     ? startpos : prevtoken.endloc);

  }
else if ( tok == tokens.Double ) {
               if (tok != tokens. Double ) parseerror("primitiveType", startpos);
else {
tokval = thetoken.tokvalue;
children.add(new CST(tok, tokval,  thetoken.startloc, thetoken.endloc));
prevtoken = thetoken;
thetoken = toklis.nexttoken();
tok = thetoken.toktype;
}
return new CST(nonterms. primitiveType , children,  startpos, startpos>prevtoken.endloc                     ? startpos : prevtoken.endloc);

  }
else if ( tok == tokens.Void ) {
               if (tok != tokens. Void ) parseerror("primitiveType", startpos);
else {
tokval = thetoken.tokvalue;
children.add(new CST(tok, tokval,  thetoken.startloc, thetoken.endloc));
prevtoken = thetoken;
thetoken = toklis.nexttoken();
tok = thetoken.toktype;
}
return new CST(nonterms. primitiveType , children,  startpos, startpos>prevtoken.endloc                     ? startpos : prevtoken.endloc);

  }
else parseerror("primitiveType", startpos);
return null;
}

public CST parsestarList ()
   throws SyntaxError
{
  ArrayList<CST> children = new ArrayList<CST>();
  CST t; Object tokval;
  int startpos = thetoken.startloc;
  if (false) ; 
else if ( (tok == tokens.__restrict__|| tok == tokens.Lbrace|| tok == tokens.Lparen|| tok == tokens.Identifier|| tok == tokens.__nameSpliced__|| tok == tokens.Intlit|| tok == tokens.Doublelit|| tok == tokens.Charlit|| tok == tokens.Stringlit|| tok == tokens.True|| tok == tokens.False|| tok == tokens.Null|| tok == tokens.__exprSpliced__|| tok == tokens.Plus|| tok == tokens.Star|| tok == tokens.Minus|| tok == tokens.Bang|| tok == tokens.Ampersand|| tok == tokens.PlusPlus|| tok == tokens.MinusMinus|| tok == tokens.Semicolon|| tok == tokens.Rparen) && (tok != tokens.Star) ) {
               ;
return new CST(nonterms. starList , children,  startpos, startpos>prevtoken.endloc                     ? startpos : prevtoken.endloc);

  }
else if ( tok == tokens.Star ) {
               if (tok != tokens. Star ) parseerror("starList", startpos);
else {
tokval = thetoken.tokvalue;
children.add(new CST(tok, tokval,  thetoken.startloc, thetoken.endloc));
prevtoken = thetoken;
thetoken = toklis.nexttoken();
tok = thetoken.toktype;
}
     t = parsestarList ();
     children.add(t);
return new CST(nonterms. starList , children,  startpos, startpos>prevtoken.endloc                     ? startpos : prevtoken.endloc);

  }
else parseerror("starList", startpos);
return null;
}

public CST parseformalParameters ()
   throws SyntaxError
{
  ArrayList<CST> children = new ArrayList<CST>();
  CST t; Object tokval;
  int startpos = thetoken.startloc;
  if (false) ; 
else if ( tok == tokens.Lparen ) {
               if (tok != tokens. Lparen ) parseerror("formalParameters", startpos);
else {
tokval = thetoken.tokvalue;
children.add(new CST(tok, tokval,  thetoken.startloc, thetoken.endloc));
prevtoken = thetoken;
thetoken = toklis.nexttoken();
tok = thetoken.toktype;
}
     t = parseformalParameterDeclsOpt ();
     children.add(t);
     if (tok != tokens. Rparen ) parseerror("formalParameters", startpos);
else {
tokval = thetoken.tokvalue;
children.add(new CST(tok, tokval,  thetoken.startloc, thetoken.endloc));
prevtoken = thetoken;
thetoken = toklis.nexttoken();
tok = thetoken.toktype;
}
return new CST(nonterms. formalParameters , children,  startpos, startpos>prevtoken.endloc                     ? startpos : prevtoken.endloc);

  }
else parseerror("formalParameters", startpos);
return null;
}

public CST parseformalParameterDeclsOpt ()
   throws SyntaxError
{
  ArrayList<CST> children = new ArrayList<CST>();
  CST t; Object tokval;
  int startpos = thetoken.startloc;
  if (false) ; 
else if ( tok == tokens.Rparen ) {
               ;
return new CST(nonterms. formalParameterDeclsOpt , children,  startpos, startpos>prevtoken.endloc                     ? startpos : prevtoken.endloc);

  }
else if ( tok == tokens.Identifier|| tok == tokens.__nameSpliced__|| tok == tokens.Bool|| tok == tokens.Char|| tok == tokens.Int|| tok == tokens.Double|| tok == tokens.Void ) {
               t = parseformalParameterDecls ();
     children.add(t);
return new CST(nonterms. formalParameterDeclsOpt , children,  startpos, startpos>prevtoken.endloc                     ? startpos : prevtoken.endloc);

  }
else parseerror("formalParameterDeclsOpt", startpos);
return null;
}

public CST parseformalParameterDecls ()
   throws SyntaxError
{
  ArrayList<CST> children = new ArrayList<CST>();
  CST t; Object tokval;
  int startpos = thetoken.startloc;
  if (false) ; 
else if ( tok == tokens.Identifier|| tok == tokens.__nameSpliced__|| tok == tokens.Bool|| tok == tokens.Char|| tok == tokens.Int|| tok == tokens.Double|| tok == tokens.Void ) {
               t = parsetype ();
     children.add(t);
     t = parserestrictOpt ();
     children.add(t);
     t = parsename ();
     children.add(t);
     t = parseemptyArrayTypeOpt ();
     children.add(t);
     t = parsetypeIdentList ();
     children.add(t);
return new CST(nonterms. formalParameterDecls , children,  startpos, startpos>prevtoken.endloc                     ? startpos : prevtoken.endloc);

  }
else parseerror("formalParameterDecls", startpos);
return null;
}

public CST parserestrictOpt ()
   throws SyntaxError
{
  ArrayList<CST> children = new ArrayList<CST>();
  CST t; Object tokval;
  int startpos = thetoken.startloc;
  if (false) ; 
else if ( tok == tokens.Identifier|| tok == tokens.__nameSpliced__ ) {
               ;
return new CST(nonterms. restrictOpt , children,  startpos, startpos>prevtoken.endloc                     ? startpos : prevtoken.endloc);

  }
else if ( tok == tokens.__restrict__ ) {
               if (tok != tokens. __restrict__ ) parseerror("restrictOpt", startpos);
else {
tokval = thetoken.tokvalue;
children.add(new CST(tok, tokval,  thetoken.startloc, thetoken.endloc));
prevtoken = thetoken;
thetoken = toklis.nexttoken();
tok = thetoken.toktype;
}
return new CST(nonterms. restrictOpt , children,  startpos, startpos>prevtoken.endloc                     ? startpos : prevtoken.endloc);

  }
else parseerror("restrictOpt", startpos);
return null;
}

public CST parsetypeIdentList ()
   throws SyntaxError
{
  ArrayList<CST> children = new ArrayList<CST>();
  CST t; Object tokval;
  int startpos = thetoken.startloc;
  if (false) ; 
else if ( tok == tokens.Rparen ) {
               ;
return new CST(nonterms. typeIdentList , children,  startpos, startpos>prevtoken.endloc                     ? startpos : prevtoken.endloc);

  }
else if ( tok == tokens.Comma ) {
               if (tok != tokens. Comma ) parseerror("typeIdentList", startpos);
else {
tokval = thetoken.tokvalue;
children.add(new CST(tok, tokval,  thetoken.startloc, thetoken.endloc));
prevtoken = thetoken;
thetoken = toklis.nexttoken();
tok = thetoken.toktype;
}
     t = parsetype ();
     children.add(t);
     t = parserestrictOpt ();
     children.add(t);
     t = parsename ();
     children.add(t);
     t = parseemptyArrayTypeOpt ();
     children.add(t);
     t = parsetypeIdentList ();
     children.add(t);
return new CST(nonterms. typeIdentList , children,  startpos, startpos>prevtoken.endloc                     ? startpos : prevtoken.endloc);

  }
else parseerror("typeIdentList", startpos);
return null;
}

public CST parseblock ()
   throws SyntaxError
{
  ArrayList<CST> children = new ArrayList<CST>();
  CST t; Object tokval;
  int startpos = thetoken.startloc;
  if (false) ; 
else if ( tok == tokens.Lbrace ) {
               if (tok != tokens. Lbrace ) parseerror("block", startpos);
else {
tokval = thetoken.tokvalue;
children.add(new CST(tok, tokval,  thetoken.startloc, thetoken.endloc));
prevtoken = thetoken;
thetoken = toklis.nexttoken();
tok = thetoken.toktype;
}
     t = parsestatementList ();
     children.add(t);
     if (tok != tokens. Rbrace ) parseerror("block", startpos);
else {
tokval = thetoken.tokvalue;
children.add(new CST(tok, tokval,  thetoken.startloc, thetoken.endloc));
prevtoken = thetoken;
thetoken = toklis.nexttoken();
tok = thetoken.toktype;
}
return new CST(nonterms. block , children,  startpos, startpos>prevtoken.endloc                     ? startpos : prevtoken.endloc);

  }
else parseerror("block", startpos);
return null;
}

public CST parsestatementList ()
   throws SyntaxError
{
  ArrayList<CST> children = new ArrayList<CST>();
  CST t; Object tokval;
  int startpos = thetoken.startloc;
  if (false) ; 
else if ( tok == tokens.Eof|| tok == tokens.Rbrace ) {
               ;
return new CST(nonterms. statementList , children,  startpos, startpos>prevtoken.endloc                     ? startpos : prevtoken.endloc);

  }
else if ( tok == tokens.Bool|| tok == tokens.Char|| tok == tokens.Int|| tok == tokens.Double|| tok == tokens.Void|| tok == tokens.If|| tok == tokens.For|| tok == tokens.While|| tok == tokens.Return|| tok == tokens.Break|| tok == tokens.Continue|| tok == tokens.Lbrace|| tok == tokens.Lparen|| tok == tokens.Identifier|| tok == tokens.__nameSpliced__|| tok == tokens.Intlit|| tok == tokens.Doublelit|| tok == tokens.Charlit|| tok == tokens.Stringlit|| tok == tokens.True|| tok == tokens.False|| tok == tokens.Null|| tok == tokens.__exprSpliced__|| tok == tokens.Plus|| tok == tokens.Star|| tok == tokens.Minus|| tok == tokens.Bang|| tok == tokens.Ampersand|| tok == tokens.PlusPlus|| tok == tokens.MinusMinus|| tok == tokens.Semicolon|| tok == tokens.__stmtSpliced__ ) {
               t = parsestatement ();
     children.add(t);
     t = parsestatementList ();
     children.add(t);
return new CST(nonterms. statementList , children,  startpos, startpos>prevtoken.endloc                     ? startpos : prevtoken.endloc);

  }
else parseerror("statementList", startpos);
return null;
}

public CST parselocalVariableDeclarationStatement ()
   throws SyntaxError
{
  ArrayList<CST> children = new ArrayList<CST>();
  CST t; Object tokval;
  int startpos = thetoken.startloc;
  if (false) ; 
else if ( tok == tokens.Identifier|| tok == tokens.__nameSpliced__|| tok == tokens.Bool|| tok == tokens.Char|| tok == tokens.Int|| tok == tokens.Double|| tok == tokens.Void ) {
               t = parselocalVariableDeclaration ();
     children.add(t);
     if (tok != tokens. Semicolon ) parseerror("localVariableDeclarationStatement", startpos);
else {
tokval = thetoken.tokvalue;
children.add(new CST(tok, tokval,  thetoken.startloc, thetoken.endloc));
prevtoken = thetoken;
thetoken = toklis.nexttoken();
tok = thetoken.toktype;
}
return new CST(nonterms. localVariableDeclarationStatement , children,  startpos, startpos>prevtoken.endloc                     ? startpos : prevtoken.endloc);

  }
else parseerror("localVariableDeclarationStatement", startpos);
return null;
}

public CST parselocalVariableDeclaration ()
   throws SyntaxError
{
  ArrayList<CST> children = new ArrayList<CST>();
  CST t; Object tokval;
  int startpos = thetoken.startloc;
  if (false) ; 
else if ( tok == tokens.Identifier|| tok == tokens.__nameSpliced__|| tok == tokens.Bool|| tok == tokens.Char|| tok == tokens.Int|| tok == tokens.Double|| tok == tokens.Void ) {
               t = parsetype ();
     children.add(t);
     t = parserestrictOpt ();
     children.add(t);
     t = parsevariableDeclarator ();
     children.add(t);
     t = parsevariableDeclaratorList ();
     children.add(t);
return new CST(nonterms. localVariableDeclaration , children,  startpos, startpos>prevtoken.endloc                     ? startpos : prevtoken.endloc);

  }
else parseerror("localVariableDeclaration", startpos);
return null;
}

public CST parsevariableDeclaratorList ()
   throws SyntaxError
{
  ArrayList<CST> children = new ArrayList<CST>();
  CST t; Object tokval;
  int startpos = thetoken.startloc;
  if (false) ; 
else if ( tok == tokens.Semicolon ) {
               ;
return new CST(nonterms. variableDeclaratorList , children,  startpos, startpos>prevtoken.endloc                     ? startpos : prevtoken.endloc);

  }
else if ( tok == tokens.Comma ) {
               if (tok != tokens. Comma ) parseerror("variableDeclaratorList", startpos);
else {
tokval = thetoken.tokvalue;
children.add(new CST(tok, tokval,  thetoken.startloc, thetoken.endloc));
prevtoken = thetoken;
thetoken = toklis.nexttoken();
tok = thetoken.toktype;
}
     t = parsevariableDeclarator ();
     children.add(t);
     t = parsevariableDeclaratorList ();
     children.add(t);
return new CST(nonterms. variableDeclaratorList , children,  startpos, startpos>prevtoken.endloc                     ? startpos : prevtoken.endloc);

  }
else parseerror("variableDeclaratorList", startpos);
return null;
}

public CST parsevariableDeclarator ()
   throws SyntaxError
{
  ArrayList<CST> children = new ArrayList<CST>();
  CST t; Object tokval;
  int startpos = thetoken.startloc;
  if (false) ; 
else if ( tok == tokens.Identifier|| tok == tokens.__nameSpliced__ ) {
               t = parsename ();
     children.add(t);
     t = parsearrayTypeOpt ();
     children.add(t);
     t = parsevariableInitializerOpt ();
     children.add(t);
return new CST(nonterms. variableDeclarator , children,  startpos, startpos>prevtoken.endloc                     ? startpos : prevtoken.endloc);

  }
else parseerror("variableDeclarator", startpos);
return null;
}

public CST parsevariableInitializerOpt ()
   throws SyntaxError
{
  ArrayList<CST> children = new ArrayList<CST>();
  CST t; Object tokval;
  int startpos = thetoken.startloc;
  if (false) ; 
else if ( tok == tokens.Comma|| tok == tokens.Semicolon ) {
               ;
return new CST(nonterms. variableInitializerOpt , children,  startpos, startpos>prevtoken.endloc                     ? startpos : prevtoken.endloc);

  }
else if ( tok == tokens.Equal ) {
               if (tok != tokens. Equal ) parseerror("variableInitializerOpt", startpos);
else {
tokval = thetoken.tokvalue;
children.add(new CST(tok, tokval,  thetoken.startloc, thetoken.endloc));
prevtoken = thetoken;
thetoken = toklis.nexttoken();
tok = thetoken.toktype;
}
     t = parseexpression ();
     children.add(t);
return new CST(nonterms. variableInitializerOpt , children,  startpos, startpos>prevtoken.endloc                     ? startpos : prevtoken.endloc);

  }
else parseerror("variableInitializerOpt", startpos);
return null;
}

public CST parseelseStmtOpt ()
   throws SyntaxError
{
  ArrayList<CST> children = new ArrayList<CST>();
  CST t; Object tokval;
  int startpos = thetoken.startloc;
  if (false) ; 
else if ( (tok == tokens.Else|| tok == tokens.Bool|| tok == tokens.Char|| tok == tokens.Int|| tok == tokens.Double|| tok == tokens.Void|| tok == tokens.If|| tok == tokens.For|| tok == tokens.While|| tok == tokens.Return|| tok == tokens.Break|| tok == tokens.Continue|| tok == tokens.Lbrace|| tok == tokens.Lparen|| tok == tokens.Identifier|| tok == tokens.__nameSpliced__|| tok == tokens.Intlit|| tok == tokens.Doublelit|| tok == tokens.Charlit|| tok == tokens.Stringlit|| tok == tokens.True|| tok == tokens.False|| tok == tokens.Null|| tok == tokens.__exprSpliced__|| tok == tokens.Plus|| tok == tokens.Star|| tok == tokens.Minus|| tok == tokens.Bang|| tok == tokens.Ampersand|| tok == tokens.PlusPlus|| tok == tokens.MinusMinus|| tok == tokens.Semicolon|| tok == tokens.__stmtSpliced__|| tok == tokens.Eof|| tok == tokens.Rbrace) && (tok != tokens.Else) ) {
               ;
return new CST(nonterms. elseStmtOpt , children,  startpos, startpos>prevtoken.endloc                     ? startpos : prevtoken.endloc);

  }
else if ( tok == tokens.Else ) {
               if (tok != tokens. Else ) parseerror("elseStmtOpt", startpos);
else {
tokval = thetoken.tokvalue;
children.add(new CST(tok, tokval,  thetoken.startloc, thetoken.endloc));
prevtoken = thetoken;
thetoken = toklis.nexttoken();
tok = thetoken.toktype;
}
     t = parsestatement ();
     children.add(t);
return new CST(nonterms. elseStmtOpt , children,  startpos, startpos>prevtoken.endloc                     ? startpos : prevtoken.endloc);

  }
else parseerror("elseStmtOpt", startpos);
return null;
}

public CST parsestatement ()
   throws SyntaxError
{
  ArrayList<CST> children = new ArrayList<CST>();
  CST t; Object tokval;
  int startpos = thetoken.startloc;
  if (false) ; 
else if ( (tok == tokens.Lbrace) && (tok == tokens.Lbrace) ) {
               t = parseblock ();
     children.add(t);
return new CST(nonterms. statement , children,  startpos, startpos>prevtoken.endloc                     ? startpos : prevtoken.endloc);

  }
else if ( (tok == tokens.Identifier|| tok == tokens.__nameSpliced__|| tok == tokens.Bool|| tok == tokens.Char|| tok == tokens.Int|| tok == tokens.Double|| tok == tokens.Void) && (toklis.peek(0) == tokens.Identifier||toklis.peek(0) == tokens.Star||toklis.peek(0) == tokens.__restrict__||toklis.peek(0) == tokens.__nameSpliced__) ) {
               t = parselocalVariableDeclarationStatement ();
     children.add(t);
return new CST(nonterms. statement , children,  startpos, startpos>prevtoken.endloc                     ? startpos : prevtoken.endloc);

  }
else if ( tok == tokens.If ) {
               if (tok != tokens. If ) parseerror("statement", startpos);
else {
tokval = thetoken.tokvalue;
children.add(new CST(tok, tokval,  thetoken.startloc, thetoken.endloc));
prevtoken = thetoken;
thetoken = toklis.nexttoken();
tok = thetoken.toktype;
}
     t = parseparExpression ();
     children.add(t);
     t = parsestatement ();
     children.add(t);
     t = parseelseStmtOpt ();
     children.add(t);
return new CST(nonterms. statement , children,  startpos, startpos>prevtoken.endloc                     ? startpos : prevtoken.endloc);

  }
else if ( tok == tokens.For ) {
               if (tok != tokens. For ) parseerror("statement", startpos);
else {
tokval = thetoken.tokvalue;
children.add(new CST(tok, tokval,  thetoken.startloc, thetoken.endloc));
prevtoken = thetoken;
thetoken = toklis.nexttoken();
tok = thetoken.toktype;
}
     if (tok != tokens. Lparen ) parseerror("statement", startpos);
else {
tokval = thetoken.tokvalue;
children.add(new CST(tok, tokval,  thetoken.startloc, thetoken.endloc));
prevtoken = thetoken;
thetoken = toklis.nexttoken();
tok = thetoken.toktype;
}
     t = parseforstatement ();
     children.add(t);
return new CST(nonterms. statement , children,  startpos, startpos>prevtoken.endloc                     ? startpos : prevtoken.endloc);

  }
else if ( tok == tokens.While ) {
               if (tok != tokens. While ) parseerror("statement", startpos);
else {
tokval = thetoken.tokvalue;
children.add(new CST(tok, tokval,  thetoken.startloc, thetoken.endloc));
prevtoken = thetoken;
thetoken = toklis.nexttoken();
tok = thetoken.toktype;
}
     t = parseparExpression ();
     children.add(t);
     t = parsestatement ();
     children.add(t);
return new CST(nonterms. statement , children,  startpos, startpos>prevtoken.endloc                     ? startpos : prevtoken.endloc);

  }
else if ( tok == tokens.Return ) {
               if (tok != tokens. Return ) parseerror("statement", startpos);
else {
tokval = thetoken.tokvalue;
children.add(new CST(tok, tokval,  thetoken.startloc, thetoken.endloc));
prevtoken = thetoken;
thetoken = toklis.nexttoken();
tok = thetoken.toktype;
}
     t = parseexpressionOpt ();
     children.add(t);
     if (tok != tokens. Semicolon ) parseerror("statement", startpos);
else {
tokval = thetoken.tokvalue;
children.add(new CST(tok, tokval,  thetoken.startloc, thetoken.endloc));
prevtoken = thetoken;
thetoken = toklis.nexttoken();
tok = thetoken.toktype;
}
return new CST(nonterms. statement , children,  startpos, startpos>prevtoken.endloc                     ? startpos : prevtoken.endloc);

  }
else if ( tok == tokens.Break ) {
               if (tok != tokens. Break ) parseerror("statement", startpos);
else {
tokval = thetoken.tokvalue;
children.add(new CST(tok, tokval,  thetoken.startloc, thetoken.endloc));
prevtoken = thetoken;
thetoken = toklis.nexttoken();
tok = thetoken.toktype;
}
     t = parseidentifierOpt ();
     children.add(t);
     if (tok != tokens. Semicolon ) parseerror("statement", startpos);
else {
tokval = thetoken.tokvalue;
children.add(new CST(tok, tokval,  thetoken.startloc, thetoken.endloc));
prevtoken = thetoken;
thetoken = toklis.nexttoken();
tok = thetoken.toktype;
}
return new CST(nonterms. statement , children,  startpos, startpos>prevtoken.endloc                     ? startpos : prevtoken.endloc);

  }
else if ( tok == tokens.Continue ) {
               if (tok != tokens. Continue ) parseerror("statement", startpos);
else {
tokval = thetoken.tokvalue;
children.add(new CST(tok, tokval,  thetoken.startloc, thetoken.endloc));
prevtoken = thetoken;
thetoken = toklis.nexttoken();
tok = thetoken.toktype;
}
     t = parseidentifierOpt ();
     children.add(t);
     if (tok != tokens. Semicolon ) parseerror("statement", startpos);
else {
tokval = thetoken.tokvalue;
children.add(new CST(tok, tokval,  thetoken.startloc, thetoken.endloc));
prevtoken = thetoken;
thetoken = toklis.nexttoken();
tok = thetoken.toktype;
}
return new CST(nonterms. statement , children,  startpos, startpos>prevtoken.endloc                     ? startpos : prevtoken.endloc);

  }
else if ( tok == tokens.Lbrace|| tok == tokens.Lparen|| tok == tokens.Identifier|| tok == tokens.__nameSpliced__|| tok == tokens.Intlit|| tok == tokens.Doublelit|| tok == tokens.Charlit|| tok == tokens.Stringlit|| tok == tokens.True|| tok == tokens.False|| tok == tokens.Null|| tok == tokens.__exprSpliced__|| tok == tokens.Plus|| tok == tokens.Star|| tok == tokens.Minus|| tok == tokens.Bang|| tok == tokens.Ampersand|| tok == tokens.PlusPlus|| tok == tokens.MinusMinus ) {
               t = parseexpression ();
     children.add(t);
     if (tok != tokens. Semicolon ) parseerror("statement", startpos);
else {
tokval = thetoken.tokvalue;
children.add(new CST(tok, tokval,  thetoken.startloc, thetoken.endloc));
prevtoken = thetoken;
thetoken = toklis.nexttoken();
tok = thetoken.toktype;
}
return new CST(nonterms. statement , children,  startpos, startpos>prevtoken.endloc                     ? startpos : prevtoken.endloc);

  }
else if ( tok == tokens.Semicolon ) {
               if (tok != tokens. Semicolon ) parseerror("statement", startpos);
else {
tokval = thetoken.tokvalue;
children.add(new CST(tok, tokval,  thetoken.startloc, thetoken.endloc));
prevtoken = thetoken;
thetoken = toklis.nexttoken();
tok = thetoken.toktype;
}
return new CST(nonterms. statement , children,  startpos, startpos>prevtoken.endloc                     ? startpos : prevtoken.endloc);

  }
else if ( tok == tokens.__stmtSpliced__ ) {
               if (tok != tokens. __stmtSpliced__ ) parseerror("statement", startpos);
else {
tokval = thetoken.tokvalue;
children.add(new CST(tok, tokval,  thetoken.startloc, thetoken.endloc));
prevtoken = thetoken;
thetoken = toklis.nexttoken();
tok = thetoken.toktype;
}
     if (tok != tokens. Antiquote ) parseerror("statement", startpos);
else {
tokval = thetoken.tokvalue;
children.add(new CST(tok, tokval,  thetoken.startloc, thetoken.endloc));
prevtoken = thetoken;
thetoken = toklis.nexttoken();
tok = thetoken.toktype;
}
return new CST(nonterms. statement , children,  startpos, startpos>prevtoken.endloc                     ? startpos : prevtoken.endloc);

  }
else parseerror("statement", startpos);
return null;
}

public CST parseforstatement ()
   throws SyntaxError
{
  ArrayList<CST> children = new ArrayList<CST>();
  CST t; Object tokval;
  int startpos = thetoken.startloc;
  if (false) ; 
else if ( (tok == tokens.Identifier|| tok == tokens.__nameSpliced__|| tok == tokens.Bool|| tok == tokens.Char|| tok == tokens.Int|| tok == tokens.Double|| tok == tokens.Void) && (toklis.peek(0) == tokens.Identifier||toklis.peek(0) == tokens.__nameSpliced__) ) {
               t = parsetype ();
     children.add(t);
     t = parsetypeFollower ();
     children.add(t);
return new CST(nonterms. forstatement , children,  startpos, startpos>prevtoken.endloc                     ? startpos : prevtoken.endloc);

  }
else if ( tok == tokens.Lbrace|| tok == tokens.Lparen|| tok == tokens.Identifier|| tok == tokens.__nameSpliced__|| tok == tokens.Intlit|| tok == tokens.Doublelit|| tok == tokens.Charlit|| tok == tokens.Stringlit|| tok == tokens.True|| tok == tokens.False|| tok == tokens.Null|| tok == tokens.__exprSpliced__|| tok == tokens.Plus|| tok == tokens.Star|| tok == tokens.Minus|| tok == tokens.Bang|| tok == tokens.Ampersand|| tok == tokens.PlusPlus|| tok == tokens.MinusMinus|| tok == tokens.Semicolon|| tok == tokens.Rparen ) {
               t = parsetypeFollower ();
     children.add(t);
return new CST(nonterms. forstatement , children,  startpos, startpos>prevtoken.endloc                     ? startpos : prevtoken.endloc);

  }
else parseerror("forstatement", startpos);
return null;
}

public CST parsetypeFollower ()
   throws SyntaxError
{
  ArrayList<CST> children = new ArrayList<CST>();
  CST t; Object tokval;
  int startpos = thetoken.startloc;
  if (false) ; 
else if ( tok == tokens.Lbrace|| tok == tokens.Lparen|| tok == tokens.Identifier|| tok == tokens.__nameSpliced__|| tok == tokens.Intlit|| tok == tokens.Doublelit|| tok == tokens.Charlit|| tok == tokens.Stringlit|| tok == tokens.True|| tok == tokens.False|| tok == tokens.Null|| tok == tokens.__exprSpliced__|| tok == tokens.Plus|| tok == tokens.Star|| tok == tokens.Minus|| tok == tokens.Bang|| tok == tokens.Ampersand|| tok == tokens.PlusPlus|| tok == tokens.MinusMinus|| tok == tokens.Semicolon|| tok == tokens.Rparen ) {
               t = parseexpressionList ();
     children.add(t);
     if (tok != tokens. Semicolon ) parseerror("typeFollower", startpos);
else {
tokval = thetoken.tokvalue;
children.add(new CST(tok, tokval,  thetoken.startloc, thetoken.endloc));
prevtoken = thetoken;
thetoken = toklis.nexttoken();
tok = thetoken.toktype;
}
     t = parseexpressionOpt ();
     children.add(t);
     if (tok != tokens. Semicolon ) parseerror("typeFollower", startpos);
else {
tokval = thetoken.tokvalue;
children.add(new CST(tok, tokval,  thetoken.startloc, thetoken.endloc));
prevtoken = thetoken;
thetoken = toklis.nexttoken();
tok = thetoken.toktype;
}
     t = parseexpressionList ();
     children.add(t);
     if (tok != tokens. Rparen ) parseerror("typeFollower", startpos);
else {
tokval = thetoken.tokvalue;
children.add(new CST(tok, tokval,  thetoken.startloc, thetoken.endloc));
prevtoken = thetoken;
thetoken = toklis.nexttoken();
tok = thetoken.toktype;
}
     t = parsestatement ();
     children.add(t);
return new CST(nonterms. typeFollower , children,  startpos, startpos>prevtoken.endloc                     ? startpos : prevtoken.endloc);

  }
else parseerror("typeFollower", startpos);
return null;
}

public CST parseexpressionList ()
   throws SyntaxError
{
  ArrayList<CST> children = new ArrayList<CST>();
  CST t; Object tokval;
  int startpos = thetoken.startloc;
  if (false) ; 
else if ( tok == tokens.Semicolon|| tok == tokens.Rparen ) {
               ;
return new CST(nonterms. expressionList , children,  startpos, startpos>prevtoken.endloc                     ? startpos : prevtoken.endloc);

  }
else if ( tok == tokens.Lbrace|| tok == tokens.Lparen|| tok == tokens.Identifier|| tok == tokens.__nameSpliced__|| tok == tokens.Intlit|| tok == tokens.Doublelit|| tok == tokens.Charlit|| tok == tokens.Stringlit|| tok == tokens.True|| tok == tokens.False|| tok == tokens.Null|| tok == tokens.__exprSpliced__|| tok == tokens.Plus|| tok == tokens.Star|| tok == tokens.Minus|| tok == tokens.Bang|| tok == tokens.Ampersand|| tok == tokens.PlusPlus|| tok == tokens.MinusMinus ) {
               t = parseexpression ();
     children.add(t);
     t = parsecommaExprList ();
     children.add(t);
return new CST(nonterms. expressionList , children,  startpos, startpos>prevtoken.endloc                     ? startpos : prevtoken.endloc);

  }
else parseerror("expressionList", startpos);
return null;
}

public CST parsecommaExprList ()
   throws SyntaxError
{
  ArrayList<CST> children = new ArrayList<CST>();
  CST t; Object tokval;
  int startpos = thetoken.startloc;
  if (false) ; 
else if ( tok == tokens.Semicolon|| tok == tokens.Rparen ) {
               ;
return new CST(nonterms. commaExprList , children,  startpos, startpos>prevtoken.endloc                     ? startpos : prevtoken.endloc);

  }
else if ( tok == tokens.Comma ) {
               if (tok != tokens. Comma ) parseerror("commaExprList", startpos);
else {
tokval = thetoken.tokvalue;
children.add(new CST(tok, tokval,  thetoken.startloc, thetoken.endloc));
prevtoken = thetoken;
thetoken = toklis.nexttoken();
tok = thetoken.toktype;
}
     t = parseexpression ();
     children.add(t);
     t = parsecommaExprList ();
     children.add(t);
return new CST(nonterms. commaExprList , children,  startpos, startpos>prevtoken.endloc                     ? startpos : prevtoken.endloc);

  }
else parseerror("commaExprList", startpos);
return null;
}

public CST parseparExpression ()
   throws SyntaxError
{
  ArrayList<CST> children = new ArrayList<CST>();
  CST t; Object tokval;
  int startpos = thetoken.startloc;
  if (false) ; 
else if ( tok == tokens.Lparen ) {
               if (tok != tokens. Lparen ) parseerror("parExpression", startpos);
else {
tokval = thetoken.tokvalue;
children.add(new CST(tok, tokval,  thetoken.startloc, thetoken.endloc));
prevtoken = thetoken;
thetoken = toklis.nexttoken();
tok = thetoken.toktype;
}
     t = parseexpression ();
     children.add(t);
     if (tok != tokens. Rparen ) parseerror("parExpression", startpos);
else {
tokval = thetoken.tokvalue;
children.add(new CST(tok, tokval,  thetoken.startloc, thetoken.endloc));
prevtoken = thetoken;
thetoken = toklis.nexttoken();
tok = thetoken.toktype;
}
return new CST(nonterms. parExpression , children,  startpos, startpos>prevtoken.endloc                     ? startpos : prevtoken.endloc);

  }
else parseerror("parExpression", startpos);
return null;
}

public CST parseidentifierOpt ()
   throws SyntaxError
{
  ArrayList<CST> children = new ArrayList<CST>();
  CST t; Object tokval;
  int startpos = thetoken.startloc;
  if (false) ; 
else if ( tok == tokens.Semicolon ) {
               ;
return new CST(nonterms. identifierOpt , children,  startpos, startpos>prevtoken.endloc                     ? startpos : prevtoken.endloc);

  }
else if ( tok == tokens.Identifier ) {
               if (tok != tokens. Identifier ) parseerror("identifierOpt", startpos);
else {
tokval = thetoken.tokvalue;
children.add(new CST(tok, tokval,  thetoken.startloc, thetoken.endloc));
prevtoken = thetoken;
thetoken = toklis.nexttoken();
tok = thetoken.toktype;
}
return new CST(nonterms. identifierOpt , children,  startpos, startpos>prevtoken.endloc                     ? startpos : prevtoken.endloc);

  }
else parseerror("identifierOpt", startpos);
return null;
}

public CST parseexpressionOpt ()
   throws SyntaxError
{
  ArrayList<CST> children = new ArrayList<CST>();
  CST t; Object tokval;
  int startpos = thetoken.startloc;
  if (false) ; 
else if ( tok == tokens.Semicolon ) {
               ;
return new CST(nonterms. expressionOpt , children,  startpos, startpos>prevtoken.endloc                     ? startpos : prevtoken.endloc);

  }
else if ( tok == tokens.Lbrace|| tok == tokens.Lparen|| tok == tokens.Identifier|| tok == tokens.__nameSpliced__|| tok == tokens.Intlit|| tok == tokens.Doublelit|| tok == tokens.Charlit|| tok == tokens.Stringlit|| tok == tokens.True|| tok == tokens.False|| tok == tokens.Null|| tok == tokens.__exprSpliced__|| tok == tokens.Plus|| tok == tokens.Star|| tok == tokens.Minus|| tok == tokens.Bang|| tok == tokens.Ampersand|| tok == tokens.PlusPlus|| tok == tokens.MinusMinus ) {
               t = parseexpression ();
     children.add(t);
return new CST(nonterms. expressionOpt , children,  startpos, startpos>prevtoken.endloc                     ? startpos : prevtoken.endloc);

  }
else parseerror("expressionOpt", startpos);
return null;
}

public CST parseexpression ()
   throws SyntaxError
{
  ArrayList<CST> children = new ArrayList<CST>();
  CST t; Object tokval;
  int startpos = thetoken.startloc;
  if (false) ; 
else if ( tok == tokens.Lbrace|| tok == tokens.Lparen|| tok == tokens.Identifier|| tok == tokens.__nameSpliced__|| tok == tokens.Intlit|| tok == tokens.Doublelit|| tok == tokens.Charlit|| tok == tokens.Stringlit|| tok == tokens.True|| tok == tokens.False|| tok == tokens.Null|| tok == tokens.__exprSpliced__ ) {
               t = parseprimary ();
     children.add(t);
     t = parseexprContOpt ();
     children.add(t);
return new CST(nonterms. expression , children,  startpos, startpos>prevtoken.endloc                     ? startpos : prevtoken.endloc);

  }
else if ( tok == tokens.Plus|| tok == tokens.Star|| tok == tokens.Minus|| tok == tokens.Bang|| tok == tokens.Ampersand|| tok == tokens.PlusPlus|| tok == tokens.MinusMinus ) {
               t = parseunaryOp ();
     children.add(t);
     t = parseexpression ();
     children.add(t);
return new CST(nonterms. expression , children,  startpos, startpos>prevtoken.endloc                     ? startpos : prevtoken.endloc);

  }
else parseerror("expression", startpos);
return null;
}

public CST parseexprContOpt ()
   throws SyntaxError
{
  ArrayList<CST> children = new ArrayList<CST>();
  CST t; Object tokval;
  int startpos = thetoken.startloc;
  if (false) ; 
else if ( tok == tokens.Comma|| tok == tokens.Semicolon|| tok == tokens.Rparen|| tok == tokens.Rbracket ) {
               ;
return new CST(nonterms. exprContOpt , children,  startpos, startpos>prevtoken.endloc                     ? startpos : prevtoken.endloc);

  }
else if ( tok == tokens.Equal|| tok == tokens.PlusEqual|| tok == tokens.MinusEqual|| tok == tokens.StarEqual|| tok == tokens.SlashEqual|| tok == tokens.ModEqual|| tok == tokens.LogicalOr|| tok == tokens.LogicalAnd|| tok == tokens.EqEq|| tok == tokens.NotEq|| tok == tokens.LessEq|| tok == tokens.GreaterEq|| tok == tokens.Less|| tok == tokens.Greater|| tok == tokens.Plus|| tok == tokens.Minus|| tok == tokens.Star|| tok == tokens.Slash|| tok == tokens.Mod|| tok == tokens.PlusPlus|| tok == tokens.MinusMinus|| tok == tokens.Lbracket ) {
               t = parseexprCont ();
     children.add(t);
return new CST(nonterms. exprContOpt , children,  startpos, startpos>prevtoken.endloc                     ? startpos : prevtoken.endloc);

  }
else parseerror("exprContOpt", startpos);
return null;
}

public CST parseexprCont ()
   throws SyntaxError
{
  ArrayList<CST> children = new ArrayList<CST>();
  CST t; Object tokval;
  int startpos = thetoken.startloc;
  if (false) ; 
else if ( tok == tokens.Equal|| tok == tokens.PlusEqual|| tok == tokens.MinusEqual|| tok == tokens.StarEqual|| tok == tokens.SlashEqual|| tok == tokens.ModEqual|| tok == tokens.LogicalOr|| tok == tokens.LogicalAnd|| tok == tokens.EqEq|| tok == tokens.NotEq|| tok == tokens.LessEq|| tok == tokens.GreaterEq|| tok == tokens.Less|| tok == tokens.Greater|| tok == tokens.Plus|| tok == tokens.Minus|| tok == tokens.Star|| tok == tokens.Slash|| tok == tokens.Mod ) {
               t = parsebinop ();
     children.add(t);
     t = parseexpression ();
     children.add(t);
return new CST(nonterms. exprCont , children,  startpos, startpos>prevtoken.endloc                     ? startpos : prevtoken.endloc);

  }
else if ( tok == tokens.PlusPlus|| tok == tokens.MinusMinus ) {
               t = parseincrOptr ();
     children.add(t);
     t = parseexprContOpt ();
     children.add(t);
return new CST(nonterms. exprCont , children,  startpos, startpos>prevtoken.endloc                     ? startpos : prevtoken.endloc);

  }
else if ( tok == tokens.Lbracket ) {
               if (tok != tokens. Lbracket ) parseerror("exprCont", startpos);
else {
tokval = thetoken.tokvalue;
children.add(new CST(tok, tokval,  thetoken.startloc, thetoken.endloc));
prevtoken = thetoken;
thetoken = toklis.nexttoken();
tok = thetoken.toktype;
}
     t = parseexpression ();
     children.add(t);
     if (tok != tokens. Rbracket ) parseerror("exprCont", startpos);
else {
tokval = thetoken.tokvalue;
children.add(new CST(tok, tokval,  thetoken.startloc, thetoken.endloc));
prevtoken = thetoken;
thetoken = toklis.nexttoken();
tok = thetoken.toktype;
}
     t = parseexprContOpt ();
     children.add(t);
return new CST(nonterms. exprCont , children,  startpos, startpos>prevtoken.endloc                     ? startpos : prevtoken.endloc);

  }
else parseerror("exprCont", startpos);
return null;
}

public CST parseprimary ()
   throws SyntaxError
{
  ArrayList<CST> children = new ArrayList<CST>();
  CST t; Object tokval;
  int startpos = thetoken.startloc;
  if (false) ; 
else if ( tok == tokens.Lbrace|| tok == tokens.Lparen|| tok == tokens.Identifier|| tok == tokens.__nameSpliced__|| tok == tokens.Intlit|| tok == tokens.Doublelit|| tok == tokens.Charlit|| tok == tokens.Stringlit|| tok == tokens.True|| tok == tokens.False|| tok == tokens.Null|| tok == tokens.__exprSpliced__ ) {
               t = parsesimplePrimary ();
     children.add(t);
     t = parseprimarySuffix ();
     children.add(t);
return new CST(nonterms. primary , children,  startpos, startpos>prevtoken.endloc                     ? startpos : prevtoken.endloc);

  }
else parseerror("primary", startpos);
return null;
}

public CST parsesimplePrimary ()
   throws SyntaxError
{
  ArrayList<CST> children = new ArrayList<CST>();
  CST t; Object tokval;
  int startpos = thetoken.startloc;
  if (false) ; 
else if ( tok == tokens.Lbrace ) {
               t = parselistExpression ();
     children.add(t);
return new CST(nonterms. simplePrimary , children,  startpos, startpos>prevtoken.endloc                     ? startpos : prevtoken.endloc);

  }
else if ( tok == tokens.Lparen ) {
               t = parseparExpression ();
     children.add(t);
return new CST(nonterms. simplePrimary , children,  startpos, startpos>prevtoken.endloc                     ? startpos : prevtoken.endloc);

  }
else if ( tok == tokens.Identifier|| tok == tokens.__nameSpliced__ ) {
               t = parsename ();
     children.add(t);
     t = parseargumentsOpt ();
     children.add(t);
return new CST(nonterms. simplePrimary , children,  startpos, startpos>prevtoken.endloc                     ? startpos : prevtoken.endloc);

  }
else if ( tok == tokens.Intlit|| tok == tokens.Doublelit|| tok == tokens.Charlit|| tok == tokens.Stringlit|| tok == tokens.True|| tok == tokens.False|| tok == tokens.Null ) {
               t = parseliteral ();
     children.add(t);
return new CST(nonterms. simplePrimary , children,  startpos, startpos>prevtoken.endloc                     ? startpos : prevtoken.endloc);

  }
else if ( tok == tokens.__exprSpliced__ ) {
               if (tok != tokens. __exprSpliced__ ) parseerror("simplePrimary", startpos);
else {
tokval = thetoken.tokvalue;
children.add(new CST(tok, tokval,  thetoken.startloc, thetoken.endloc));
prevtoken = thetoken;
thetoken = toklis.nexttoken();
tok = thetoken.toktype;
}
     if (tok != tokens. Antiquote ) parseerror("simplePrimary", startpos);
else {
tokval = thetoken.tokvalue;
children.add(new CST(tok, tokval,  thetoken.startloc, thetoken.endloc));
prevtoken = thetoken;
thetoken = toklis.nexttoken();
tok = thetoken.toktype;
}
return new CST(nonterms. simplePrimary , children,  startpos, startpos>prevtoken.endloc                     ? startpos : prevtoken.endloc);

  }
else parseerror("simplePrimary", startpos);
return null;
}

public CST parseargumentsOpt ()
   throws SyntaxError
{
  ArrayList<CST> children = new ArrayList<CST>();
  CST t; Object tokval;
  int startpos = thetoken.startloc;
  if (false) ; 
else if ( tok == tokens.Dot|| tok == tokens.Lbrace|| tok == tokens.Equal|| tok == tokens.PlusEqual|| tok == tokens.MinusEqual|| tok == tokens.StarEqual|| tok == tokens.SlashEqual|| tok == tokens.ModEqual|| tok == tokens.LogicalOr|| tok == tokens.LogicalAnd|| tok == tokens.EqEq|| tok == tokens.NotEq|| tok == tokens.LessEq|| tok == tokens.GreaterEq|| tok == tokens.Less|| tok == tokens.Greater|| tok == tokens.Plus|| tok == tokens.Minus|| tok == tokens.Star|| tok == tokens.Slash|| tok == tokens.Mod|| tok == tokens.PlusPlus|| tok == tokens.MinusMinus|| tok == tokens.Lbracket|| tok == tokens.Comma|| tok == tokens.Semicolon|| tok == tokens.Rparen|| tok == tokens.Rbracket ) {
               ;
return new CST(nonterms. argumentsOpt , children,  startpos, startpos>prevtoken.endloc                     ? startpos : prevtoken.endloc);

  }
else if ( tok == tokens.Lparen ) {
               t = parsearguments ();
     children.add(t);
return new CST(nonterms. argumentsOpt , children,  startpos, startpos>prevtoken.endloc                     ? startpos : prevtoken.endloc);

  }
else parseerror("argumentsOpt", startpos);
return null;
}

public CST parsearguments ()
   throws SyntaxError
{
  ArrayList<CST> children = new ArrayList<CST>();
  CST t; Object tokval;
  int startpos = thetoken.startloc;
  if (false) ; 
else if ( tok == tokens.Lparen ) {
               if (tok != tokens. Lparen ) parseerror("arguments", startpos);
else {
tokval = thetoken.tokvalue;
children.add(new CST(tok, tokval,  thetoken.startloc, thetoken.endloc));
prevtoken = thetoken;
thetoken = toklis.nexttoken();
tok = thetoken.toktype;
}
     t = parseexpressionList ();
     children.add(t);
     if (tok != tokens. Rparen ) parseerror("arguments", startpos);
else {
tokval = thetoken.tokvalue;
children.add(new CST(tok, tokval,  thetoken.startloc, thetoken.endloc));
prevtoken = thetoken;
thetoken = toklis.nexttoken();
tok = thetoken.toktype;
}
return new CST(nonterms. arguments , children,  startpos, startpos>prevtoken.endloc                     ? startpos : prevtoken.endloc);

  }
else parseerror("arguments", startpos);
return null;
}

public CST parseprimarySuffix ()
   throws SyntaxError
{
  ArrayList<CST> children = new ArrayList<CST>();
  CST t; Object tokval;
  int startpos = thetoken.startloc;
  if (false) ; 
else if ( tok == tokens.Equal|| tok == tokens.PlusEqual|| tok == tokens.MinusEqual|| tok == tokens.StarEqual|| tok == tokens.SlashEqual|| tok == tokens.ModEqual|| tok == tokens.LogicalOr|| tok == tokens.LogicalAnd|| tok == tokens.EqEq|| tok == tokens.NotEq|| tok == tokens.LessEq|| tok == tokens.GreaterEq|| tok == tokens.Less|| tok == tokens.Greater|| tok == tokens.Plus|| tok == tokens.Minus|| tok == tokens.Star|| tok == tokens.Slash|| tok == tokens.Mod|| tok == tokens.PlusPlus|| tok == tokens.MinusMinus|| tok == tokens.Lbracket|| tok == tokens.Comma|| tok == tokens.Semicolon|| tok == tokens.Rparen|| tok == tokens.Rbracket ) {
               ;
return new CST(nonterms. primarySuffix , children,  startpos, startpos>prevtoken.endloc                     ? startpos : prevtoken.endloc);

  }
else if ( tok == tokens.Dot ) {
               if (tok != tokens. Dot ) parseerror("primarySuffix", startpos);
else {
tokval = thetoken.tokvalue;
children.add(new CST(tok, tokval,  thetoken.startloc, thetoken.endloc));
prevtoken = thetoken;
thetoken = toklis.nexttoken();
tok = thetoken.toktype;
}
     if (tok != tokens. Identifier ) parseerror("primarySuffix", startpos);
else {
tokval = thetoken.tokvalue;
children.add(new CST(tok, tokval,  thetoken.startloc, thetoken.endloc));
prevtoken = thetoken;
thetoken = toklis.nexttoken();
tok = thetoken.toktype;
}
     t = parseprimarySuffix ();
     children.add(t);
return new CST(nonterms. primarySuffix , children,  startpos, startpos>prevtoken.endloc                     ? startpos : prevtoken.endloc);

  }
else if ( tok == tokens.Lbrace ) {
               t = parselistExpression ();
     children.add(t);
     t = parseprimarySuffix ();
     children.add(t);
return new CST(nonterms. primarySuffix , children,  startpos, startpos>prevtoken.endloc                     ? startpos : prevtoken.endloc);

  }
else parseerror("primarySuffix", startpos);
return null;
}

public CST parselistExpression ()
   throws SyntaxError
{
  ArrayList<CST> children = new ArrayList<CST>();
  CST t; Object tokval;
  int startpos = thetoken.startloc;
  if (false) ; 
else if ( tok == tokens.Lbrace ) {
               if (tok != tokens. Lbrace ) parseerror("listExpression", startpos);
else {
tokval = thetoken.tokvalue;
children.add(new CST(tok, tokval,  thetoken.startloc, thetoken.endloc));
prevtoken = thetoken;
thetoken = toklis.nexttoken();
tok = thetoken.toktype;
}
     t = parselistElementListOpt ();
     children.add(t);
     if (tok != tokens. Rbrace ) parseerror("listExpression", startpos);
else {
tokval = thetoken.tokvalue;
children.add(new CST(tok, tokval,  thetoken.startloc, thetoken.endloc));
prevtoken = thetoken;
thetoken = toklis.nexttoken();
tok = thetoken.toktype;
}
return new CST(nonterms. listExpression , children,  startpos, startpos>prevtoken.endloc                     ? startpos : prevtoken.endloc);

  }
else parseerror("listExpression", startpos);
return null;
}

public CST parselistElementListOpt ()
   throws SyntaxError
{
  ArrayList<CST> children = new ArrayList<CST>();
  CST t; Object tokval;
  int startpos = thetoken.startloc;
  if (false) ; 
else if ( tok == tokens.Rbrace ) {
               ;
return new CST(nonterms. listElementListOpt , children,  startpos, startpos>prevtoken.endloc                     ? startpos : prevtoken.endloc);

  }
else if ( tok == tokens.Intlit|| tok == tokens.Doublelit|| tok == tokens.Charlit|| tok == tokens.Stringlit|| tok == tokens.True|| tok == tokens.False|| tok == tokens.Null|| tok == tokens.Minus|| tok == tokens.__exprSpliced__|| tok == tokens.Lbrace ) {
               t = parselistElement ();
     children.add(t);
     t = parsecommaListElementList ();
     children.add(t);
return new CST(nonterms. listElementListOpt , children,  startpos, startpos>prevtoken.endloc                     ? startpos : prevtoken.endloc);

  }
else if ( (tok == tokens.Comma) && (toklis.peek(0) == tokens.Rbrace) ) {
               if (tok != tokens. Comma ) parseerror("listElementListOpt", startpos);
else {
tokval = thetoken.tokvalue;
children.add(new CST(tok, tokval,  thetoken.startloc, thetoken.endloc));
prevtoken = thetoken;
thetoken = toklis.nexttoken();
tok = thetoken.toktype;
}
return new CST(nonterms. listElementListOpt , children,  startpos, startpos>prevtoken.endloc                     ? startpos : prevtoken.endloc);

  }
else parseerror("listElementListOpt", startpos);
return null;
}

public CST parsecommaListElementList ()
   throws SyntaxError
{
  ArrayList<CST> children = new ArrayList<CST>();
  CST t; Object tokval;
  int startpos = thetoken.startloc;
  if (false) ; 
else if ( tok == tokens.Rbrace ) {
               ;
return new CST(nonterms. commaListElementList , children,  startpos, startpos>prevtoken.endloc                     ? startpos : prevtoken.endloc);

  }
else if ( (tok == tokens.Comma) && (toklis.peek(0) == tokens.Rbrace) ) {
               if (tok != tokens. Comma ) parseerror("commaListElementList", startpos);
else {
tokval = thetoken.tokvalue;
children.add(new CST(tok, tokval,  thetoken.startloc, thetoken.endloc));
prevtoken = thetoken;
thetoken = toklis.nexttoken();
tok = thetoken.toktype;
}
return new CST(nonterms. commaListElementList , children,  startpos, startpos>prevtoken.endloc                     ? startpos : prevtoken.endloc);

  }
else if ( tok == tokens.Comma ) {
               if (tok != tokens. Comma ) parseerror("commaListElementList", startpos);
else {
tokval = thetoken.tokvalue;
children.add(new CST(tok, tokval,  thetoken.startloc, thetoken.endloc));
prevtoken = thetoken;
thetoken = toklis.nexttoken();
tok = thetoken.toktype;
}
     t = parselistElement ();
     children.add(t);
     t = parsecommaListElementList ();
     children.add(t);
return new CST(nonterms. commaListElementList , children,  startpos, startpos>prevtoken.endloc                     ? startpos : prevtoken.endloc);

  }
else parseerror("commaListElementList", startpos);
return null;
}

public CST parselistElement ()
   throws SyntaxError
{
  ArrayList<CST> children = new ArrayList<CST>();
  CST t; Object tokval;
  int startpos = thetoken.startloc;
  if (false) ; 
else if ( tok == tokens.Intlit|| tok == tokens.Doublelit|| tok == tokens.Charlit|| tok == tokens.Stringlit|| tok == tokens.True|| tok == tokens.False|| tok == tokens.Null|| tok == tokens.Minus|| tok == tokens.__exprSpliced__|| tok == tokens.Lbrace ) {
               t = parseconstExpression ();
     children.add(t);
return new CST(nonterms. listElement , children,  startpos, startpos>prevtoken.endloc                     ? startpos : prevtoken.endloc);

  }
else parseerror("listElement", startpos);
return null;
}

public CST parseconstExpression ()
   throws SyntaxError
{
  ArrayList<CST> children = new ArrayList<CST>();
  CST t; Object tokval;
  int startpos = thetoken.startloc;
  if (false) ; 
else if ( tok == tokens.Intlit|| tok == tokens.Doublelit|| tok == tokens.Charlit|| tok == tokens.Stringlit|| tok == tokens.True|| tok == tokens.False|| tok == tokens.Null ) {
               t = parseliteral ();
     children.add(t);
return new CST(nonterms. constExpression , children,  startpos, startpos>prevtoken.endloc                     ? startpos : prevtoken.endloc);

  }
else if ( tok == tokens.Minus ) {
               if (tok != tokens. Minus ) parseerror("constExpression", startpos);
else {
tokval = thetoken.tokvalue;
children.add(new CST(tok, tokval,  thetoken.startloc, thetoken.endloc));
prevtoken = thetoken;
thetoken = toklis.nexttoken();
tok = thetoken.toktype;
}
     t = parseliteral ();
     children.add(t);
return new CST(nonterms. constExpression , children,  startpos, startpos>prevtoken.endloc                     ? startpos : prevtoken.endloc);

  }
else if ( tok == tokens.__exprSpliced__ ) {
               if (tok != tokens. __exprSpliced__ ) parseerror("constExpression", startpos);
else {
tokval = thetoken.tokvalue;
children.add(new CST(tok, tokval,  thetoken.startloc, thetoken.endloc));
prevtoken = thetoken;
thetoken = toklis.nexttoken();
tok = thetoken.toktype;
}
     if (tok != tokens. Antiquote ) parseerror("constExpression", startpos);
else {
tokval = thetoken.tokvalue;
children.add(new CST(tok, tokval,  thetoken.startloc, thetoken.endloc));
prevtoken = thetoken;
thetoken = toklis.nexttoken();
tok = thetoken.toktype;
}
return new CST(nonterms. constExpression , children,  startpos, startpos>prevtoken.endloc                     ? startpos : prevtoken.endloc);

  }
else if ( tok == tokens.Lbrace ) {
               t = parselistExpression ();
     children.add(t);
return new CST(nonterms. constExpression , children,  startpos, startpos>prevtoken.endloc                     ? startpos : prevtoken.endloc);

  }
else parseerror("constExpression", startpos);
return null;
}

public CST parseliteral ()
   throws SyntaxError
{
  ArrayList<CST> children = new ArrayList<CST>();
  CST t; Object tokval;
  int startpos = thetoken.startloc;
  if (false) ; 
else if ( tok == tokens.Intlit ) {
               if (tok != tokens. Intlit ) parseerror("literal", startpos);
else {
tokval = thetoken.tokvalue;
children.add(new CST(tok, tokval,  thetoken.startloc, thetoken.endloc));
prevtoken = thetoken;
thetoken = toklis.nexttoken();
tok = thetoken.toktype;
}
return new CST(nonterms. literal , children,  startpos, startpos>prevtoken.endloc                     ? startpos : prevtoken.endloc);

  }
else if ( tok == tokens.Doublelit ) {
               if (tok != tokens. Doublelit ) parseerror("literal", startpos);
else {
tokval = thetoken.tokvalue;
children.add(new CST(tok, tokval,  thetoken.startloc, thetoken.endloc));
prevtoken = thetoken;
thetoken = toklis.nexttoken();
tok = thetoken.toktype;
}
return new CST(nonterms. literal , children,  startpos, startpos>prevtoken.endloc                     ? startpos : prevtoken.endloc);

  }
else if ( tok == tokens.Charlit ) {
               if (tok != tokens. Charlit ) parseerror("literal", startpos);
else {
tokval = thetoken.tokvalue;
children.add(new CST(tok, tokval,  thetoken.startloc, thetoken.endloc));
prevtoken = thetoken;
thetoken = toklis.nexttoken();
tok = thetoken.toktype;
}
return new CST(nonterms. literal , children,  startpos, startpos>prevtoken.endloc                     ? startpos : prevtoken.endloc);

  }
else if ( tok == tokens.Stringlit ) {
               if (tok != tokens. Stringlit ) parseerror("literal", startpos);
else {
tokval = thetoken.tokvalue;
children.add(new CST(tok, tokval,  thetoken.startloc, thetoken.endloc));
prevtoken = thetoken;
thetoken = toklis.nexttoken();
tok = thetoken.toktype;
}
return new CST(nonterms. literal , children,  startpos, startpos>prevtoken.endloc                     ? startpos : prevtoken.endloc);

  }
else if ( tok == tokens.True ) {
               if (tok != tokens. True ) parseerror("literal", startpos);
else {
tokval = thetoken.tokvalue;
children.add(new CST(tok, tokval,  thetoken.startloc, thetoken.endloc));
prevtoken = thetoken;
thetoken = toklis.nexttoken();
tok = thetoken.toktype;
}
return new CST(nonterms. literal , children,  startpos, startpos>prevtoken.endloc                     ? startpos : prevtoken.endloc);

  }
else if ( tok == tokens.False ) {
               if (tok != tokens. False ) parseerror("literal", startpos);
else {
tokval = thetoken.tokvalue;
children.add(new CST(tok, tokval,  thetoken.startloc, thetoken.endloc));
prevtoken = thetoken;
thetoken = toklis.nexttoken();
tok = thetoken.toktype;
}
return new CST(nonterms. literal , children,  startpos, startpos>prevtoken.endloc                     ? startpos : prevtoken.endloc);

  }
else if ( tok == tokens.Null ) {
               if (tok != tokens. Null ) parseerror("literal", startpos);
else {
tokval = thetoken.tokvalue;
children.add(new CST(tok, tokval,  thetoken.startloc, thetoken.endloc));
prevtoken = thetoken;
thetoken = toklis.nexttoken();
tok = thetoken.toktype;
}
return new CST(nonterms. literal , children,  startpos, startpos>prevtoken.endloc                     ? startpos : prevtoken.endloc);

  }
else parseerror("literal", startpos);
return null;
}

public CST parsebinop ()
   throws SyntaxError
{
  ArrayList<CST> children = new ArrayList<CST>();
  CST t; Object tokval;
  int startpos = thetoken.startloc;
  if (false) ; 
else if ( tok == tokens.Equal|| tok == tokens.PlusEqual|| tok == tokens.MinusEqual|| tok == tokens.StarEqual|| tok == tokens.SlashEqual|| tok == tokens.ModEqual ) {
               t = parseassignmentOp ();
     children.add(t);
return new CST(nonterms. binop , children,  startpos, startpos>prevtoken.endloc                     ? startpos : prevtoken.endloc);

  }
else if ( tok == tokens.LogicalOr|| tok == tokens.LogicalAnd ) {
               t = parselogicalOp ();
     children.add(t);
return new CST(nonterms. binop , children,  startpos, startpos>prevtoken.endloc                     ? startpos : prevtoken.endloc);

  }
else if ( tok == tokens.EqEq|| tok == tokens.NotEq|| tok == tokens.LessEq|| tok == tokens.GreaterEq|| tok == tokens.Less|| tok == tokens.Greater ) {
               t = parsecomparisonOp ();
     children.add(t);
return new CST(nonterms. binop , children,  startpos, startpos>prevtoken.endloc                     ? startpos : prevtoken.endloc);

  }
else if ( tok == tokens.Plus|| tok == tokens.Minus|| tok == tokens.Star|| tok == tokens.Slash|| tok == tokens.Mod ) {
               t = parsearithOp ();
     children.add(t);
return new CST(nonterms. binop , children,  startpos, startpos>prevtoken.endloc                     ? startpos : prevtoken.endloc);

  }
else parseerror("binop", startpos);
return null;
}

public CST parselogicalOp ()
   throws SyntaxError
{
  ArrayList<CST> children = new ArrayList<CST>();
  CST t; Object tokval;
  int startpos = thetoken.startloc;
  if (false) ; 
else if ( tok == tokens.LogicalOr ) {
               if (tok != tokens. LogicalOr ) parseerror("logicalOp", startpos);
else {
tokval = thetoken.tokvalue;
children.add(new CST(tok, tokval,  thetoken.startloc, thetoken.endloc));
prevtoken = thetoken;
thetoken = toklis.nexttoken();
tok = thetoken.toktype;
}
return new CST(nonterms. logicalOp , children,  startpos, startpos>prevtoken.endloc                     ? startpos : prevtoken.endloc);

  }
else if ( tok == tokens.LogicalAnd ) {
               if (tok != tokens. LogicalAnd ) parseerror("logicalOp", startpos);
else {
tokval = thetoken.tokvalue;
children.add(new CST(tok, tokval,  thetoken.startloc, thetoken.endloc));
prevtoken = thetoken;
thetoken = toklis.nexttoken();
tok = thetoken.toktype;
}
return new CST(nonterms. logicalOp , children,  startpos, startpos>prevtoken.endloc                     ? startpos : prevtoken.endloc);

  }
else parseerror("logicalOp", startpos);
return null;
}

public CST parsecomparisonOp ()
   throws SyntaxError
{
  ArrayList<CST> children = new ArrayList<CST>();
  CST t; Object tokval;
  int startpos = thetoken.startloc;
  if (false) ; 
else if ( tok == tokens.EqEq ) {
               if (tok != tokens. EqEq ) parseerror("comparisonOp", startpos);
else {
tokval = thetoken.tokvalue;
children.add(new CST(tok, tokval,  thetoken.startloc, thetoken.endloc));
prevtoken = thetoken;
thetoken = toklis.nexttoken();
tok = thetoken.toktype;
}
return new CST(nonterms. comparisonOp , children,  startpos, startpos>prevtoken.endloc                     ? startpos : prevtoken.endloc);

  }
else if ( tok == tokens.NotEq ) {
               if (tok != tokens. NotEq ) parseerror("comparisonOp", startpos);
else {
tokval = thetoken.tokvalue;
children.add(new CST(tok, tokval,  thetoken.startloc, thetoken.endloc));
prevtoken = thetoken;
thetoken = toklis.nexttoken();
tok = thetoken.toktype;
}
return new CST(nonterms. comparisonOp , children,  startpos, startpos>prevtoken.endloc                     ? startpos : prevtoken.endloc);

  }
else if ( tok == tokens.LessEq ) {
               if (tok != tokens. LessEq ) parseerror("comparisonOp", startpos);
else {
tokval = thetoken.tokvalue;
children.add(new CST(tok, tokval,  thetoken.startloc, thetoken.endloc));
prevtoken = thetoken;
thetoken = toklis.nexttoken();
tok = thetoken.toktype;
}
return new CST(nonterms. comparisonOp , children,  startpos, startpos>prevtoken.endloc                     ? startpos : prevtoken.endloc);

  }
else if ( tok == tokens.GreaterEq ) {
               if (tok != tokens. GreaterEq ) parseerror("comparisonOp", startpos);
else {
tokval = thetoken.tokvalue;
children.add(new CST(tok, tokval,  thetoken.startloc, thetoken.endloc));
prevtoken = thetoken;
thetoken = toklis.nexttoken();
tok = thetoken.toktype;
}
return new CST(nonterms. comparisonOp , children,  startpos, startpos>prevtoken.endloc                     ? startpos : prevtoken.endloc);

  }
else if ( tok == tokens.Less ) {
               if (tok != tokens. Less ) parseerror("comparisonOp", startpos);
else {
tokval = thetoken.tokvalue;
children.add(new CST(tok, tokval,  thetoken.startloc, thetoken.endloc));
prevtoken = thetoken;
thetoken = toklis.nexttoken();
tok = thetoken.toktype;
}
return new CST(nonterms. comparisonOp , children,  startpos, startpos>prevtoken.endloc                     ? startpos : prevtoken.endloc);

  }
else if ( tok == tokens.Greater ) {
               if (tok != tokens. Greater ) parseerror("comparisonOp", startpos);
else {
tokval = thetoken.tokvalue;
children.add(new CST(tok, tokval,  thetoken.startloc, thetoken.endloc));
prevtoken = thetoken;
thetoken = toklis.nexttoken();
tok = thetoken.toktype;
}
return new CST(nonterms. comparisonOp , children,  startpos, startpos>prevtoken.endloc                     ? startpos : prevtoken.endloc);

  }
else parseerror("comparisonOp", startpos);
return null;
}

public CST parsearithOp ()
   throws SyntaxError
{
  ArrayList<CST> children = new ArrayList<CST>();
  CST t; Object tokval;
  int startpos = thetoken.startloc;
  if (false) ; 
else if ( tok == tokens.Plus ) {
               if (tok != tokens. Plus ) parseerror("arithOp", startpos);
else {
tokval = thetoken.tokvalue;
children.add(new CST(tok, tokval,  thetoken.startloc, thetoken.endloc));
prevtoken = thetoken;
thetoken = toklis.nexttoken();
tok = thetoken.toktype;
}
return new CST(nonterms. arithOp , children,  startpos, startpos>prevtoken.endloc                     ? startpos : prevtoken.endloc);

  }
else if ( tok == tokens.Minus ) {
               if (tok != tokens. Minus ) parseerror("arithOp", startpos);
else {
tokval = thetoken.tokvalue;
children.add(new CST(tok, tokval,  thetoken.startloc, thetoken.endloc));
prevtoken = thetoken;
thetoken = toklis.nexttoken();
tok = thetoken.toktype;
}
return new CST(nonterms. arithOp , children,  startpos, startpos>prevtoken.endloc                     ? startpos : prevtoken.endloc);

  }
else if ( tok == tokens.Star ) {
               if (tok != tokens. Star ) parseerror("arithOp", startpos);
else {
tokval = thetoken.tokvalue;
children.add(new CST(tok, tokval,  thetoken.startloc, thetoken.endloc));
prevtoken = thetoken;
thetoken = toklis.nexttoken();
tok = thetoken.toktype;
}
return new CST(nonterms. arithOp , children,  startpos, startpos>prevtoken.endloc                     ? startpos : prevtoken.endloc);

  }
else if ( tok == tokens.Slash ) {
               if (tok != tokens. Slash ) parseerror("arithOp", startpos);
else {
tokval = thetoken.tokvalue;
children.add(new CST(tok, tokval,  thetoken.startloc, thetoken.endloc));
prevtoken = thetoken;
thetoken = toklis.nexttoken();
tok = thetoken.toktype;
}
return new CST(nonterms. arithOp , children,  startpos, startpos>prevtoken.endloc                     ? startpos : prevtoken.endloc);

  }
else if ( tok == tokens.Mod ) {
               if (tok != tokens. Mod ) parseerror("arithOp", startpos);
else {
tokval = thetoken.tokvalue;
children.add(new CST(tok, tokval,  thetoken.startloc, thetoken.endloc));
prevtoken = thetoken;
thetoken = toklis.nexttoken();
tok = thetoken.toktype;
}
return new CST(nonterms. arithOp , children,  startpos, startpos>prevtoken.endloc                     ? startpos : prevtoken.endloc);

  }
else parseerror("arithOp", startpos);
return null;
}

public CST parseassignmentOp ()
   throws SyntaxError
{
  ArrayList<CST> children = new ArrayList<CST>();
  CST t; Object tokval;
  int startpos = thetoken.startloc;
  if (false) ; 
else if ( tok == tokens.Equal ) {
               if (tok != tokens. Equal ) parseerror("assignmentOp", startpos);
else {
tokval = thetoken.tokvalue;
children.add(new CST(tok, tokval,  thetoken.startloc, thetoken.endloc));
prevtoken = thetoken;
thetoken = toklis.nexttoken();
tok = thetoken.toktype;
}
return new CST(nonterms. assignmentOp , children,  startpos, startpos>prevtoken.endloc                     ? startpos : prevtoken.endloc);

  }
else if ( tok == tokens.PlusEqual ) {
               if (tok != tokens. PlusEqual ) parseerror("assignmentOp", startpos);
else {
tokval = thetoken.tokvalue;
children.add(new CST(tok, tokval,  thetoken.startloc, thetoken.endloc));
prevtoken = thetoken;
thetoken = toklis.nexttoken();
tok = thetoken.toktype;
}
return new CST(nonterms. assignmentOp , children,  startpos, startpos>prevtoken.endloc                     ? startpos : prevtoken.endloc);

  }
else if ( tok == tokens.MinusEqual ) {
               if (tok != tokens. MinusEqual ) parseerror("assignmentOp", startpos);
else {
tokval = thetoken.tokvalue;
children.add(new CST(tok, tokval,  thetoken.startloc, thetoken.endloc));
prevtoken = thetoken;
thetoken = toklis.nexttoken();
tok = thetoken.toktype;
}
return new CST(nonterms. assignmentOp , children,  startpos, startpos>prevtoken.endloc                     ? startpos : prevtoken.endloc);

  }
else if ( tok == tokens.StarEqual ) {
               if (tok != tokens. StarEqual ) parseerror("assignmentOp", startpos);
else {
tokval = thetoken.tokvalue;
children.add(new CST(tok, tokval,  thetoken.startloc, thetoken.endloc));
prevtoken = thetoken;
thetoken = toklis.nexttoken();
tok = thetoken.toktype;
}
return new CST(nonterms. assignmentOp , children,  startpos, startpos>prevtoken.endloc                     ? startpos : prevtoken.endloc);

  }
else if ( tok == tokens.SlashEqual ) {
               if (tok != tokens. SlashEqual ) parseerror("assignmentOp", startpos);
else {
tokval = thetoken.tokvalue;
children.add(new CST(tok, tokval,  thetoken.startloc, thetoken.endloc));
prevtoken = thetoken;
thetoken = toklis.nexttoken();
tok = thetoken.toktype;
}
return new CST(nonterms. assignmentOp , children,  startpos, startpos>prevtoken.endloc                     ? startpos : prevtoken.endloc);

  }
else if ( tok == tokens.ModEqual ) {
               if (tok != tokens. ModEqual ) parseerror("assignmentOp", startpos);
else {
tokval = thetoken.tokvalue;
children.add(new CST(tok, tokval,  thetoken.startloc, thetoken.endloc));
prevtoken = thetoken;
thetoken = toklis.nexttoken();
tok = thetoken.toktype;
}
return new CST(nonterms. assignmentOp , children,  startpos, startpos>prevtoken.endloc                     ? startpos : prevtoken.endloc);

  }
else parseerror("assignmentOp", startpos);
return null;
}

public CST parseunaryOp ()
   throws SyntaxError
{
  ArrayList<CST> children = new ArrayList<CST>();
  CST t; Object tokval;
  int startpos = thetoken.startloc;
  if (false) ; 
else if ( tok == tokens.Plus ) {
               if (tok != tokens. Plus ) parseerror("unaryOp", startpos);
else {
tokval = thetoken.tokvalue;
children.add(new CST(tok, tokval,  thetoken.startloc, thetoken.endloc));
prevtoken = thetoken;
thetoken = toklis.nexttoken();
tok = thetoken.toktype;
}
return new CST(nonterms. unaryOp , children,  startpos, startpos>prevtoken.endloc                     ? startpos : prevtoken.endloc);

  }
else if ( tok == tokens.Star ) {
               if (tok != tokens. Star ) parseerror("unaryOp", startpos);
else {
tokval = thetoken.tokvalue;
children.add(new CST(tok, tokval,  thetoken.startloc, thetoken.endloc));
prevtoken = thetoken;
thetoken = toklis.nexttoken();
tok = thetoken.toktype;
}
return new CST(nonterms. unaryOp , children,  startpos, startpos>prevtoken.endloc                     ? startpos : prevtoken.endloc);

  }
else if ( tok == tokens.Minus ) {
               if (tok != tokens. Minus ) parseerror("unaryOp", startpos);
else {
tokval = thetoken.tokvalue;
children.add(new CST(tok, tokval,  thetoken.startloc, thetoken.endloc));
prevtoken = thetoken;
thetoken = toklis.nexttoken();
tok = thetoken.toktype;
}
return new CST(nonterms. unaryOp , children,  startpos, startpos>prevtoken.endloc                     ? startpos : prevtoken.endloc);

  }
else if ( tok == tokens.Bang ) {
               if (tok != tokens. Bang ) parseerror("unaryOp", startpos);
else {
tokval = thetoken.tokvalue;
children.add(new CST(tok, tokval,  thetoken.startloc, thetoken.endloc));
prevtoken = thetoken;
thetoken = toklis.nexttoken();
tok = thetoken.toktype;
}
return new CST(nonterms. unaryOp , children,  startpos, startpos>prevtoken.endloc                     ? startpos : prevtoken.endloc);

  }
else if ( tok == tokens.Ampersand ) {
               if (tok != tokens. Ampersand ) parseerror("unaryOp", startpos);
else {
tokval = thetoken.tokvalue;
children.add(new CST(tok, tokval,  thetoken.startloc, thetoken.endloc));
prevtoken = thetoken;
thetoken = toklis.nexttoken();
tok = thetoken.toktype;
}
return new CST(nonterms. unaryOp , children,  startpos, startpos>prevtoken.endloc                     ? startpos : prevtoken.endloc);

  }
else if ( tok == tokens.PlusPlus ) {
               if (tok != tokens. PlusPlus ) parseerror("unaryOp", startpos);
else {
tokval = thetoken.tokvalue;
children.add(new CST(tok, tokval,  thetoken.startloc, thetoken.endloc));
prevtoken = thetoken;
thetoken = toklis.nexttoken();
tok = thetoken.toktype;
}
return new CST(nonterms. unaryOp , children,  startpos, startpos>prevtoken.endloc                     ? startpos : prevtoken.endloc);

  }
else if ( tok == tokens.MinusMinus ) {
               if (tok != tokens. MinusMinus ) parseerror("unaryOp", startpos);
else {
tokval = thetoken.tokvalue;
children.add(new CST(tok, tokval,  thetoken.startloc, thetoken.endloc));
prevtoken = thetoken;
thetoken = toklis.nexttoken();
tok = thetoken.toktype;
}
return new CST(nonterms. unaryOp , children,  startpos, startpos>prevtoken.endloc                     ? startpos : prevtoken.endloc);

  }
else parseerror("unaryOp", startpos);
return null;
}

public CST parseincrOptr ()
   throws SyntaxError
{
  ArrayList<CST> children = new ArrayList<CST>();
  CST t; Object tokval;
  int startpos = thetoken.startloc;
  if (false) ; 
else if ( tok == tokens.PlusPlus ) {
               if (tok != tokens. PlusPlus ) parseerror("incrOptr", startpos);
else {
tokval = thetoken.tokvalue;
children.add(new CST(tok, tokval,  thetoken.startloc, thetoken.endloc));
prevtoken = thetoken;
thetoken = toklis.nexttoken();
tok = thetoken.toktype;
}
return new CST(nonterms. incrOptr , children,  startpos, startpos>prevtoken.endloc                     ? startpos : prevtoken.endloc);

  }
else if ( tok == tokens.MinusMinus ) {
               if (tok != tokens. MinusMinus ) parseerror("incrOptr", startpos);
else {
tokval = thetoken.tokvalue;
children.add(new CST(tok, tokval,  thetoken.startloc, thetoken.endloc));
prevtoken = thetoken;
thetoken = toklis.nexttoken();
tok = thetoken.toktype;
}
return new CST(nonterms. incrOptr , children,  startpos, startpos>prevtoken.endloc                     ? startpos : prevtoken.endloc);

  }
else parseerror("incrOptr", startpos);
return null;
}

public CST parsename ()
   throws SyntaxError
{
  ArrayList<CST> children = new ArrayList<CST>();
  CST t; Object tokval;
  int startpos = thetoken.startloc;
  if (false) ; 
else if ( tok == tokens.Identifier ) {
               if (tok != tokens. Identifier ) parseerror("name", startpos);
else {
tokval = thetoken.tokvalue;
children.add(new CST(tok, tokval,  thetoken.startloc, thetoken.endloc));
prevtoken = thetoken;
thetoken = toklis.nexttoken();
tok = thetoken.toktype;
}
return new CST(nonterms. name , children,  startpos, startpos>prevtoken.endloc                     ? startpos : prevtoken.endloc);

  }
else if ( tok == tokens.__nameSpliced__ ) {
               if (tok != tokens. __nameSpliced__ ) parseerror("name", startpos);
else {
tokval = thetoken.tokvalue;
children.add(new CST(tok, tokval,  thetoken.startloc, thetoken.endloc));
prevtoken = thetoken;
thetoken = toklis.nexttoken();
tok = thetoken.toktype;
}
     if (tok != tokens. Antiquote ) parseerror("name", startpos);
else {
tokval = thetoken.tokvalue;
children.add(new CST(tok, tokval,  thetoken.startloc, thetoken.endloc));
prevtoken = thetoken;
thetoken = toklis.nexttoken();
tok = thetoken.toktype;
}
return new CST(nonterms. name , children,  startpos, startpos>prevtoken.endloc                     ? startpos : prevtoken.endloc);

  }
else parseerror("name", startpos);
return null;
}

   // END of generated parser code here

}

