//using System.Text;

public class Lexer {

   public enum states {
      start ,
      inIdent ,
      number ,
      float1 ,
      float2 ,
      floaterr1 ,
      floaterr2 ,
      floaterr3 ,
      charlit ,
      charerr1 ,
      char2 ,
      charerr2 ,
      instring ,
      afterEscape ,
      string2 ,
      slash ,
      slasheq ,
      inCcomment ,
      inCcomment2 ,
      endcomment ,
      inCPcomment ,
      whitespace ,
      dot ,
      dotdot ,
      dotstar ,
      minus ,
      arrow ,
      minuseq ,
      minusminus ,
      star ,
      stareq ,
      lbrace ,
      rbrace ,
      semic ,
      colon ,
      cons ,
      comma ,
      lparen ,
      rparen ,
      lbracket ,
      rbracket ,
      greater ,
      greaterEq ,
      gtGt ,
      gtGtGt ,
      gtGtEqual ,
      less ,
      lesseq ,
      lessless ,
      lesslesseq ,
      equal ,
      eqeq ,
      bang ,
      noteq ,
      plus ,
      plusplus ,
      plusequal ,
      bitor ,
      bitorequal ,
      logicalor ,
      logicalorequal ,
      ampersand ,
      ampersandequal ,
      logicaland ,
      logicalandequal ,
      caret ,
      caretequal ,
      mod ,
      modequal ,
      questionmark ,
      tilde ,
      eof ,
      dollar ,
      codequote ,
      postquote ,
      aquote ,
   }

// END of generated enums

  public CharStream cs;
  public int tokstartpos;  // starting char. pos. of this token

  public TokenList toklis = new TokenList();

  public Lexer (CharStream cs) {
     this.cs = cs;
  }

  public void emit(tokens t) { toklis.add(t, tokstartpos, cs.cursor-1); }

  public void emit(tokens t, StringBuffer sb) {
     switch (t) {
     case Identifier:
     case Stringlit:
     case Antiquote:
                     toklis.add(t, processescapes(sb).toString(),
                                tokstartpos, cs.cursor-1);
		     return;
     case Intlit: toklis.add(t, MyInteger.parseInt(sb.toString()),
                             tokstartpos, cs.cursor-1);
		  return;
     case Doublelit: toklis.add(t, MyDouble.parseDouble(sb.toString()),
                             tokstartpos, cs.cursor-1);
		  return;
     case Charlit: toklis.add(t, getchar(processescapes(sb), 1),
                               tokstartpos, cs.cursor-1);
		  return;
     }
  }

  // Process escape characters in string buffer; return string.
  // All characters are processed, including initial and final
  // double or single quote.
  StringBuffer processescapes (StringBuffer sb) {
     StringBuffer sbout = new StringBuffer();
     int i = 0;
     while (i < sb.length()) {
        char c = getchar(sb, i);
        if (c != '\\')
           sbout.append(c);
        else {
           i++; c = getchar(sb, i);
           if ("ntbfr".indexOf(c) == -1)
              sbout.append(c);
           else switch (c) {
              case 'n': sbout.append('\n'); break;
              case 't': sbout.append('\t'); break;
              case 'b': sbout.append('\b'); break;
              case 'f': sbout.append('\f'); break;
              case 'r': sbout.append('\r'); break;
           }
        }
        i++;
     }
     return sbout;
  }

  // Just to simplify C# conversion
  public static char getchar(StringBuffer s, int i) {
     return s.charAt(i);
  }

  // CharStream gives all characters in file; can get current
  // line and character position.
  // StringBuffer gives characters in token thus far.
  // ch is last character read (when the error occurred), s is
  // the lexer state, and msg is a message for this error.
  public void lexerror (CharStream cs, StringBuffer sb, char ch,
                        String msg) 
     throws LexError
  {
     throw new LexError(cs, sb, ch, msg);
  }

  public void emitIdentOrKw (StringBuffer sb) {
     String thetoken = sb.toString();
     tokens tok;
     if (false) ;
     // START of gen'd body of checkKeyword
     else if (thetoken.toUpperCase().equals("BOOL")) tok = tokens. Bool ;
     else if (thetoken.toUpperCase().equals("CHAR")) tok = tokens. Char ;
     else if (thetoken.toUpperCase().equals("VOID")) tok = tokens. Void ;
     else if (thetoken.toUpperCase().equals("INT")) tok = tokens. Int ;
     else if (thetoken.toUpperCase().equals("DOUBLE")) tok = tokens. Double ;
     else if (thetoken.toUpperCase().equals("ELSE")) tok = tokens. Else ;
     else if (thetoken.toUpperCase().equals("IF")) tok = tokens. If ;
     else if (thetoken.toUpperCase().equals("WHILE")) tok = tokens. While ;
     else if (thetoken.toUpperCase().equals("RETURN")) tok = tokens. Return ;
     else if (thetoken.toUpperCase().equals("BREAK")) tok = tokens. Break ;
     else if (thetoken.toUpperCase().equals("CONTINUE")) tok = tokens. Continue ;
     else if (thetoken.toUpperCase().equals("FOR")) tok = tokens. For ;
     else if (thetoken.toUpperCase().equals("TRUE")) tok = tokens. True ;
     else if (thetoken.toUpperCase().equals("FALSE")) tok = tokens. False ;
     else if (thetoken.toUpperCase().equals("NULL")) tok = tokens. Null ;
     else if (thetoken.toUpperCase().equals("EXTERN")) tok = tokens. Extern ;
     else if (thetoken.toUpperCase().equals("GLOBAL")) tok = tokens. Global ;
     else if (thetoken.toUpperCase().equals("__EXPRSPLICED__")) tok = tokens. __exprSpliced__ ;
     else if (thetoken.toUpperCase().equals("__STMTSPLICED__")) tok = tokens. __stmtSpliced__ ;
     else if (thetoken.toUpperCase().equals("__UNITSPLICED__")) tok = tokens. __unitSpliced__ ;
     else if (thetoken.toUpperCase().equals("__NAMESPLICED__")) tok = tokens. __nameSpliced__ ;
     else if (thetoken.toUpperCase().equals("__RESTRICT__")) tok = tokens. __restrict__ ;
     // END of gen'd body of checkKeyword
     else tok = tokens.Identifier;
  if (tok == tokens.Identifier)
     emit(tok, sb);
  else 
     emit(tok);
  }

  public TokenList lexer ()
      throws LexError
  {
     StringBuffer thetoken = new StringBuffer();
     char ch = ' ';
     while (true) { // check for eof at bottom of loop
           states state, newstate = states.start;
           boolean nexttoken = false;
           tokstartpos = cs.cursor;
	   while (!nexttoken) {
	      state = newstate;
              ch = cs.peek();
              switch (state) {
		      // START of generated code here
                 case start :
                    if (('a' <= ch && ch <= 'z' ) || ('A' <= ch && ch <= 'Z' )) { newstate = states. inIdent ; break; }
                    if (ch == '_') { newstate = states. inIdent ; break; }
                    if ('0' <= ch && ch <= '9') { newstate = states. number ; break; }
                    if (ch == ' '|| ch == '\n'|| ch == '\t'|| ch == '\r') { newstate = states. whitespace ; break; }
                    if (ch == '\'') { newstate = states. charlit ; break; }
                    if (ch == '"') { newstate = states. instring ; break; }
                    if (ch == '/') { newstate = states. slash ; break; }
                    if (ch == '.') { newstate = states. dot ; break; }
                    if (ch == '-') { newstate = states. minus ; break; }
                    if (ch == '*') { newstate = states. star ; break; }
                    if (ch == '{') { newstate = states. lbrace ; break; }
                    if (ch == '}') { newstate = states. rbrace ; break; }
                    if (ch == ';') { newstate = states. semic ; break; }
                    if (ch == ':') { newstate = states. colon ; break; }
                    if (ch == ',') { newstate = states. comma ; break; }
                    if (ch == '(') { newstate = states. lparen ; break; }
                    if (ch == ')') { newstate = states. rparen ; break; }
                    if (ch == '[') { newstate = states. lbracket ; break; }
                    if (ch == ']') { newstate = states. rbracket ; break; }
                    if (ch == '>') { newstate = states. greater ; break; }
                    if (ch == '<') { newstate = states. less ; break; }
                    if (ch == '=') { newstate = states. equal ; break; }
                    if (ch == '!') { newstate = states. bang ; break; }
                    if (ch == '+') { newstate = states. plus ; break; }
                    if (ch == '|') { newstate = states. bitor ; break; }
                    if (ch == '&') { newstate = states. ampersand ; break; }
                    if (ch == '^') { newstate = states. caret ; break; }
                    if (ch == '%') { newstate = states. mod ; break; }
                    if (ch == '?') { newstate = states. questionmark ; break; }
                    if (ch == '~') { newstate = states. tilde ; break; }
                    if (ch == '$') { newstate = states. dollar ; break; }
                    if (ch == CharStream.eof) { newstate = states. eof ; break; }
                    lexerror(cs, thetoken, ch, "Illegal character"); ; nexttoken = true; break;
                 case eof :
                    emit(tokens.Eof, thetoken); ; nexttoken = true; break;
                 case inIdent :
                    if (('a' <= ch && ch <= 'z' ) || ('A' <= ch && ch <= 'Z' )||'0' <= ch && ch <= '9'||ch == '_') { newstate = states. inIdent ; break; }
                    emitIdentOrKw(thetoken); ; nexttoken = true; break;
                 case number :
                    if ('0' <= ch && ch <= '9') { newstate = states. number ; break; }
                    if ((ch == '.' && cs.peek(1) != '.')) { newstate = states. floaterr1 ; break; }
                    if (ch == 'e'|| ch == 'E') { newstate = states. floaterr2 ; break; }
                    emit(tokens.Intlit, thetoken); ; nexttoken = true; break;
                 case floaterr1 :
                    if ('0' <= ch && ch <= '9') { newstate = states. float1 ; break; }
                    lexerror(cs, thetoken, ch, "Dot not followed by digit in floating-point number"); ; nexttoken = true; break;
                 case float1 :
                    if ('0' <= ch && ch <= '9') { newstate = states. float1 ; break; }
                    if (ch == 'e'|| ch == 'E') { newstate = states. floaterr2 ; break; }
                    emit(tokens.Doublelit, thetoken); ; nexttoken = true; break;
                 case floaterr2 :
                    if ('0' <= ch && ch <= '9') { newstate = states. float2 ; break; }
                    if (ch == '+'|| ch == '-') { newstate = states. floaterr3 ; break; }
                    lexerror(cs, thetoken, ch, "E not followed by digit or sign in floating-point number"); ; nexttoken = true; break;
                 case float2 :
                    if ('0' <= ch && ch <= '9') { newstate = states. float2 ; break; }
                    emit(tokens.Doublelit, thetoken); ; nexttoken = true; break;
                 case floaterr3 :
                    if ('0' <= ch && ch <= '9') { newstate = states. float2 ; break; }
                    lexerror(cs, thetoken, ch, "Exponent not supplied in floating-point number"); ; nexttoken = true; break;
                 case charlit :
                    if (!(ch == '\\') && ch != '\0') { newstate = states. charerr1 ; break; }
                    if (ch == '\\') { newstate = states. charerr2 ; break; }
                    lexerror(cs, thetoken, ch, "Single quote not followed by valid character"); ; nexttoken = true; break;
                 case charerr1 :
                    if (ch == '\'') { newstate = states. char2 ; break; }
                    lexerror(cs, thetoken, ch, "Character not terminated by single quote"); ; nexttoken = true; break;
                 case char2 :
                    emit(tokens.Charlit, thetoken); ; nexttoken = true; break;
                 case charerr2 :
                    if (!(false) && ch != '\0') { newstate = states. charerr1 ; break; }
                    lexerror(cs, thetoken, ch, "Escape not followed by valid escape character"); ; nexttoken = true; break;
                 case dollar :
                    if (ch == '<') { newstate = states. codequote ; break; }
                    lexerror(cs, thetoken, ch, "Illegal character: $"); ; nexttoken = true; break;
                 case codequote :
                    if (ch == '>') { newstate = states. postquote ; break; }
                    if (!(false) && ch != '\0') { newstate = states. codequote ; break; }
                    lexerror(cs, thetoken, ch, "Non-terminated code quote"); ; nexttoken = true; break;
                 case postquote :
                    if (ch == '>') { newstate = states. postquote ; break; }
                    if (ch == '$') { newstate = states. aquote ; break; }
                    if (!(false) && ch != '\0') { newstate = states. codequote ; break; }
                    lexerror(cs, thetoken, ch, "Non-terminated code quote"); ; nexttoken = true; break;
                 case aquote :
                    emit(tokens.Antiquote, thetoken); ; nexttoken = true; break;
                 case instring :
                    if (ch == '\\') { newstate = states. afterEscape ; break; }
                    if (ch == '"') { newstate = states. string2 ; break; }
                    if (!(false) && ch != '\0') { newstate = states. instring ; break; }
                    lexerror(cs, thetoken, ch, "Invalid character in string"); ; nexttoken = true; break;
                 case afterEscape :
                    if (!(false) && ch != '\0') { newstate = states. instring ; break; }
                    lexerror(cs, thetoken, ch, "Escape not followed by valid escape character"); ; nexttoken = true; break;
                 case string2 :
                    emit(tokens.Stringlit, thetoken); ; nexttoken = true; break;
                 case slash :
                    if (ch == '*') { newstate = states. inCcomment ; break; }
                    if (ch == '/') { newstate = states. inCPcomment ; break; }
                    if (ch == '=') { newstate = states. slasheq ; break; }
                    emit(tokens.Slash); ; nexttoken = true; break;
                 case slasheq :
                    emit(tokens.SlashEqual); ; nexttoken = true; break;
                 case inCcomment :
                    if (!(ch == '*') && ch != '\0') { newstate = states. inCcomment ; break; }
                    if (ch == '*') { newstate = states. inCcomment2 ; break; }
                    lexerror(cs, thetoken, ch, "Comment not terminated"); ; nexttoken = true; break;
                 case inCcomment2 :
                    if (!(ch == '*'|| ch == '/') && ch != '\0') { newstate = states. inCcomment ; break; }
                    if (ch == '*') { newstate = states. inCcomment2 ; break; }
                    if (ch == '/') { newstate = states. endcomment ; break; }
                    lexerror(cs, thetoken, ch, "Comment not terminated"); ; nexttoken = true; break;
                 case endcomment :
                    ; nexttoken = true; break;
                 case inCPcomment :
                    if (ch == '\n') { newstate = states. endcomment ; break; }
                    if (!(false) && ch != '\0') { newstate = states. inCPcomment ; break; }
                    lexerror(cs, thetoken, ch, "Comment not terminated"); ; nexttoken = true; break;
                 case whitespace :
                    ; nexttoken = true; break;
                 case dot :
                    if (ch == '.') { newstate = states. dotdot ; break; }
                    if (ch == '*') { newstate = states. dotstar ; break; }
                    emit(tokens.Dot); ; nexttoken = true; break;
                 case dotdot :
                    emit(tokens.DotDot); ; nexttoken = true; break;
                 case dotstar :
                    emit(tokens.DotStar); ; nexttoken = true; break;
                 case minus :
                    if (ch == '>') { newstate = states. arrow ; break; }
                    if (ch == '=') { newstate = states. minuseq ; break; }
                    if (ch == '-') { newstate = states. minusminus ; break; }
                    emit(tokens.Minus); ; nexttoken = true; break;
                 case arrow :
                    emit(tokens.Arrow); ; nexttoken = true; break;
                 case minuseq :
                    emit(tokens.MinusEqual); ; nexttoken = true; break;
                 case minusminus :
                    emit(tokens.MinusMinus); ; nexttoken = true; break;
                 case star :
                    if (ch == '=') { newstate = states. stareq ; break; }
                    emit(tokens.Star); ; nexttoken = true; break;
                 case stareq :
                    emit(tokens.StarEqual); ; nexttoken = true; break;
                 case lbrace :
                    emit(tokens.Lbrace); ; nexttoken = true; break;
                 case rbrace :
                    emit(tokens.Rbrace); ; nexttoken = true; break;
                 case semic :
                    emit(tokens.Semicolon); ; nexttoken = true; break;
                 case colon :
                    if (ch == ':') { newstate = states. cons ; break; }
                    emit(tokens.Colon); ; nexttoken = true; break;
                 case cons :
                    emit(tokens.Cons, thetoken); ; nexttoken = true; break;
                 case comma :
                    emit(tokens.Comma); ; nexttoken = true; break;
                 case lparen :
                    emit(tokens.Lparen); ; nexttoken = true; break;
                 case rparen :
                    emit(tokens.Rparen); ; nexttoken = true; break;
                 case lbracket :
                    emit(tokens.Lbracket); ; nexttoken = true; break;
                 case rbracket :
                    emit(tokens.Rbracket); ; nexttoken = true; break;
                 case greater :
                    if (ch == '>') { newstate = states. gtGt ; break; }
                    if (ch == '=') { newstate = states. greaterEq ; break; }
                    emit(tokens.Greater); ; nexttoken = true; break;
                 case greaterEq :
                    emit(tokens.GreaterEq); ; nexttoken = true; break;
                 case gtGt :
                    if (ch == '=') { newstate = states. gtGtEqual ; break; }
                    emit(tokens.GtGt); ; nexttoken = true; break;
                 case gtGtEqual :
                    emit(tokens.GtGtEqual); ; nexttoken = true; break;
                 case less :
                    if (ch == '<') { newstate = states. lessless ; break; }
                    if (ch == '=') { newstate = states. lesseq ; break; }
                    emit(tokens.Less); ; nexttoken = true; break;
                 case lesseq :
                    emit(tokens.LessEq); ; nexttoken = true; break;
                 case lessless :
                    if (ch == '=') { newstate = states. lesslesseq ; break; }
                    emit(tokens.LessLess); ; nexttoken = true; break;
                 case lesslesseq :
                    emit(tokens.LessLessEqual); ; nexttoken = true; break;
                 case equal :
                    if (ch == '=') { newstate = states. eqeq ; break; }
                    emit(tokens.Equal); ; nexttoken = true; break;
                 case eqeq :
                    emit(tokens.EqEq); ; nexttoken = true; break;
                 case bang :
                    if (ch == '=') { newstate = states. noteq ; break; }
                    emit(tokens.Bang); ; nexttoken = true; break;
                 case noteq :
                    emit(tokens.NotEq); ; nexttoken = true; break;
                 case plus :
                    if (ch == '+') { newstate = states. plusplus ; break; }
                    if (ch == '=') { newstate = states. plusequal ; break; }
                    emit(tokens.Plus); ; nexttoken = true; break;
                 case plusplus :
                    emit(tokens.PlusPlus); ; nexttoken = true; break;
                 case plusequal :
                    emit(tokens.PlusEqual); ; nexttoken = true; break;
                 case bitor :
                    if (ch == '=') { newstate = states. bitorequal ; break; }
                    if (ch == '|') { newstate = states. logicalor ; break; }
                    emit(tokens.BitOr); ; nexttoken = true; break;
                 case bitorequal :
                    emit(tokens.BitOrEqual); ; nexttoken = true; break;
                 case logicalor :
                    if (ch == '=') { newstate = states. bitorequal ; break; }
                    emit(tokens.LogicalOr); ; nexttoken = true; break;
                 case ampersand :
                    if (ch == '=') { newstate = states. ampersandequal ; break; }
                    if (ch == '&') { newstate = states. logicaland ; break; }
                    emit(tokens.Ampersand); ; nexttoken = true; break;
                 case ampersandequal :
                    emit(tokens.AmpersandEqual); ; nexttoken = true; break;
                 case logicaland :
                    if (ch == '=') { newstate = states. logicalandequal ; break; }
                    emit(tokens.LogicalAnd); ; nexttoken = true; break;
                 case caret :
                    if (ch == '=') { newstate = states. caretequal ; break; }
                    emit(tokens.Caret); ; nexttoken = true; break;
                 case caretequal :
                    emit(tokens.CaretEqual); ; nexttoken = true; break;
                 case mod :
                    if (ch == '=') { newstate = states. modequal ; break; }
                    emit(tokens.Mod); ; nexttoken = true; break;
                 case modequal :
                    emit(tokens.ModEqual); ; nexttoken = true; break;
                 case questionmark :
                    emit(tokens.QuestionMark); ; nexttoken = true; break;
                 case tilde :
                    emit(tokens.Tilde); ; nexttoken = true; break;
		      // END of generated code
              }
              if (!nexttoken) {
                 thetoken.append(ch);
                 cs.getChar();
              }
        }
	if (ch == CharStream.eof) {
	   emit(tokens.Eof);
	   break;
	}
	thetoken.delete(0, thetoken.length());
     }
     return toklis;
  }
}

