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
True = "True"
False = "False"
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
            Return, Break, Continue, For, True, False, Null,
            Extern, Global,
            ExprSpliced, StmtSpliced, UnitSpliced, NameSpliced,
            Restrict
            ]
symbols = [Eof, Dot, DotStar, DotDot, Arrow, Star, Lbrace, Rbrace,
           Semicolon, Colon, Cons, Comma, Lparen, Rparen,
           Lbracket, Rbracket, Equal, PlusEqual, MinusEqual, StarEqual,
           SlashEqual, AmpersandEqual, BitOrEqual, CaretEqual, ModEqual,
           BitOr, Ampersand,
           LessLessEqual, GtGtEqual, QuestionMark,
           LogicalOr, LogicalAnd, Or, Caret, EqEq, NotEq, And, LessEq,
           GreaterEq, Less, Greater, LessLess, GtGt, Plus,
           Minus, Slash, Mod, PlusPlus, MinusMinus, Tilde, Bang]
tokens = tokencats + keywords + symbols
   

def charrep (c):
   if c == '\n': return "\\n"
   if c == '\r': return "\\r"
   if c == '\t': return "\\t"
   if c == '\\': return "\\\\"
   if c == '\'': return "\\'"
   return c

def oneOf (s):
   if s == "": return "false"
   pred = "ch == '" + charrep(s[0]) + "'"
   for c in s[1:]:
      pred += "|| ch == '" + charrep(c) + "'"
   return pred

def notOneOf (s):
   return "!(" + oneOf(s) + ") && ch != '\\0'"

dotNotDotDot = "(ch == '.' && cs.peek(1) != '.')"

isLetter = "('a' <= ch && ch <= 'z' ) || ('A' <= ch && ch <= 'Z' )"
isDigit = "'0' <= ch && ch <= '9'"
isWhite = oneOf(" \n\t\r")
anyChar = notOneOf("")

# States
start = "start"
inIdent = "inIdent"
number = "number"
float1 = "float1"
float2 = "float2"
floaterr1 = "floaterr1"
floaterr2 = "floaterr2"
floaterr3 = "floaterr3"
charlit = "charlit"
charerr1 = "charerr1"
char2 = "char2"
charerr2 = "charerr2"
instring = "instring"
afterEscape = "afterEscape"
string2 = "string2"
slash = "slash"
slasheq = "slasheq"
inCcomment = "inCcomment"
inCcomment2 = "inCcomment2"
endcomment = "endcomment"
inCPcomment = "inCPcomment"
whitespace = "whitespace"
dot = "dot"
dotdot = "dotdot"
dotstar = "dotstar"
minus = "minus"
arrow = "arrow"
minuseq = "minuseq"
minusminus = "minusminus"
star = "star"
stareq = "stareq"
lbrace = "lbrace"
rbrace = "rbrace"
semic = "semic"
colon = "colon"
cons = "cons"
comma = "comma"
lparen = "lparen"
rparen = "rparen"
lbracket = "lbracket"
rbracket = "rbracket"
greater = "greater"
greaterEq = "greaterEq"
gtGt = "gtGt"
gtGtGt = "gtGtGt"
gtGtEqual = "gtGtEqual"
gtGtGtEqual = "gtGtGtEqual"
less = "less"
lesseq = "lesseq"
lessless = "lessless"
lesslesseq = "lesslesseq"
equal = "equal"
eqeq = "eqeq"
bang = "bang"
noteq = "noteq"
plus = "plus"
plusplus = "plusplus"
plusequal = "plusequal"
bitor = "bitor"
bitorequal = "bitorequal"
logicalor = "logicalor"
logicalorequal = "logicalorequal"
ampersand = "ampersand"
ampersandequal = "ampersandequal"
logicaland = "logicaland"
logicalandequal = "logicalandequal"
caret = "caret"
caretequal = "caretequal"
mod = "mod"
modequal = "modequal"
questionmark = "questionmark"
tilde = "tilde"
dollar = "dollar"
codequote = "codequote"
postquote = "postquote"
aquote = "aquote"
eof = "eof"

states = [
    start,
    inIdent,
    number,
    float1,
    float2,
    floaterr1,
    floaterr2,
    floaterr3,
    charlit,
    charerr1,
    char2,
    charerr2,
    instring,
    afterEscape,
    string2,
    slash,
    slasheq,
    inCcomment,
    inCcomment2,
    endcomment,
    inCPcomment,
    whitespace,
    dot,
    dotdot,
    dotstar,
    minus,
    arrow,
    minuseq,
    minusminus,
    star,
    stareq,
    lbrace,
    rbrace,
    semic,
    colon,
    cons,
    comma,
    lparen,
    rparen,
    lbracket,
    rbracket,
    greater,
    greaterEq,
    gtGt,
    gtGtGt,
    gtGtEqual,
    less,
    lesseq,
    lessless,
    lesslesseq,
    equal,
    eqeq,
    bang,
    noteq,
    plus,
    plusplus,
    plusequal,
    bitor,
    bitorequal,
    logicalor,
    logicalorequal,
    ampersand,
    ampersandequal,
    logicaland,
    logicalandequal,
    caret,
    caretequal,
    mod,
    modequal,
    questionmark,
    tilde,
    eof,
    dollar,
    codequote,
    postquote,
    aquote
    ]

# non-token actions
IdentOrKw = "IdentOrKw"
Error = "Error"
Discard = "Discard"
  
dfa = [
  [start, Error, [[isLetter, inIdent],
  	          [oneOf("_"), inIdent],
  	          [isDigit, number],
		  [isWhite, whitespace],
		  [oneOf("'"), charlit],
		  [oneOf("\""), instring],
		  [oneOf("/"), slash],
		  [oneOf("."), dot],
		  [oneOf("-"), minus],
		  [oneOf("*"), star],
		  [oneOf("{"), lbrace],
		  [oneOf("}"), rbrace],
		  [oneOf(";"), semic],
		  [oneOf(":"), colon],
		  [oneOf(","), comma],
		  [oneOf("("), lparen],
		  [oneOf(")"), rparen],
		  [oneOf("["), lbracket],
		  [oneOf("]"), rbracket],
		  [oneOf(">"), greater],
		  [oneOf("<"), less],
		  [oneOf("="), equal],
		  [oneOf("!"), bang],
		  [oneOf("+"), plus],
		  [oneOf("|"), bitor],
		  [oneOf("&"), ampersand],
		  [oneOf("^"), caret],
		  [oneOf("%"), mod],
		  [oneOf("?"), questionmark],
		  [oneOf("~"), tilde],
		  [oneOf("$"), dollar],
		  ["ch == CharStream.eof", eof]],
                "Illegal character"],
  [eof, Eof, []],
  # Identifiers
  [inIdent, IdentOrKw, [[isLetter + "||" + isDigit
                        + "||" + oneOf("_"), inIdent]]],
  # Ints and floats
  [number, Intlit, [[isDigit, number],
	  	  [dotNotDotDot, floaterr1],
	  	  [oneOf("eE"), floaterr2]]],
  [floaterr1, Error, [[isDigit, float1]],
             "Dot not followed by digit in floating-point number"],
  [float1, Doublelit, [[isDigit, float1],
	  	  [oneOf("eE"), floaterr2]]],
  [floaterr2, Error,  [[isDigit, float2],
	  	  [oneOf("+-"), floaterr3]],
             "E not followed by digit or sign in floating-point number"],
  [float2, Doublelit, [[isDigit, float2]]],
  [floaterr3, Error,  [[isDigit, float2]],
             "Exponent not supplied in floating-point number"],
  # Chars
  [charlit, Error, [[notOneOf("\\"), charerr1],
		 [oneOf("\\"), charerr2]],
             "Single quote not followed by valid character"],
  [charerr1, Error, [[oneOf("'"), char2]],
             "Character not terminated by single quote"],
  [char2, Charlit, []],
  [charerr2, Error, [[anyChar, charerr1]],
             "Escape not followed by valid escape character"],
  # Code quotes
  [dollar, Error, [[oneOf("<"), codequote]], "Illegal character: $"],
  [codequote, Error, [[oneOf(">"), postquote],
                      [anyChar, codequote]], "Non-terminated code quote"],
  [postquote, Error, [[oneOf(">"), postquote],
                      [oneOf("$"), aquote],
                      [anyChar, codequote]], "Non-terminated code quote"],
  [aquote, Antiquote, []],
  # Strings
  [instring, Error, [[oneOf("\\"), afterEscape],
	  	   [oneOf("\""), string2],
	           [anyChar, instring]],
             "Invalid character in string"],
  [afterEscape, Error, [[anyChar, instring]],
             "Escape not followed by valid escape character"],
  [string2, Stringlit, []],
  # Comments, division, etc. (start with /)
  [slash, Slash, [[oneOf("*"), inCcomment],
	  	  [oneOf("/"), inCPcomment],
		  [oneOf("="), slasheq]]],
  [slasheq, SlashEqual, []],
  [inCcomment, Error, [[notOneOf("*"), inCcomment],
	               [oneOf("*"), inCcomment2]],
               "Comment not terminated"],
  [inCcomment2, Error, [[notOneOf("*/"), inCcomment],
	               [oneOf("*"), inCcomment2],
	               [oneOf("/"), endcomment]],
               "Comment not terminated"],
  [endcomment, Discard, []],
  [inCPcomment, Error, [[oneOf("\n"), endcomment],
		        [anyChar, inCPcomment]],
               "Comment not terminated"],
  [whitespace, Discard, []],
  [dot, Dot, [[oneOf("."), dotdot],
               [oneOf("*"), dotstar]]],
  [dotdot, DotDot, []],
  [dotstar, DotStar, []],
  [minus, Minus, [[oneOf(">"), arrow],
                  [oneOf("="), minuseq],
                  [oneOf("-"), minusminus]]],
  [arrow, Arrow, []],
  [minuseq, MinusEqual, []],
  [minusminus, MinusMinus, []],
  [star, Star, [[oneOf("="), stareq]]],
  [stareq, StarEqual, []],
  [lbrace, Lbrace, []],
  [rbrace, Rbrace, []],
  [semic, Semicolon, []],
  [colon, Colon, [[oneOf(":"), cons]]],
  [cons, Cons, []],
  [comma, Comma, []],
  [lparen, Lparen, []],
  [rparen, Rparen, []],
  [lbracket, Lbracket, []],
  [rbracket, Rbracket, []],
  [greater, Greater, [[oneOf(">"), gtGt],
	              [oneOf("="), greaterEq]]],
  [greaterEq, GreaterEq, []],
  [gtGt, GtGt, [[oneOf("="), gtGtEqual]]],
  [gtGtEqual, GtGtEqual, []],
  [less, Less,  [[oneOf("<"), lessless],
                 [oneOf("="), lesseq]]],
  [lesseq, LessEq, []],
  [lessless, LessLess, [[oneOf("="), lesslesseq]]],
  [lesslesseq, LessLessEqual, []],
  [equal, Equal, [[oneOf("="), eqeq]]],
  [eqeq, EqEq, []],
  [bang, Bang, [[oneOf("="), noteq]]],
  [noteq, NotEq, []],
  [plus, Plus, [[oneOf("+"), plusplus],
                [oneOf("="), plusequal]]],
  [plusplus, PlusPlus, []],
  [plusequal, PlusEqual, []],
  [bitor, BitOr, [[oneOf("="), bitorequal],
                  [oneOf("|"), logicalor]]],
  [bitorequal, BitOrEqual, []],
  [logicalor, LogicalOr, [[oneOf("="), bitorequal]]],
  [ampersand, Ampersand, [[oneOf("="), ampersandequal],
                  [oneOf("&"), logicaland]]],
  [ampersandequal, AmpersandEqual, []],
  [logicaland, LogicalAnd, [[oneOf("="), logicalandequal]]],
  [caret, Caret, [[oneOf("="), caretequal]]],
  [caretequal, CaretEqual, []],
  [mod, Mod, [[oneOf("="), modequal]]],
  [modequal, ModEqual, []],
  [questionmark, QuestionMark, []],
  [tilde, Tilde, []]
]

def doaction (action, state, moves):
   if action == "Discard":
      return ""
   elif action == "Error":
      return "lexerror(cs, thetoken, ch, \"" + moves[-1] +"\");"
   elif action in symbols:
      return "emit(tokens."+action+");"
   elif action == "IdentOrKw":
      return "emitIdentOrKw(thetoken);"
   else:
      return "emit(tokens."+action+", thetoken);"

def genlexer (file) :
   f = open(file, "w")

   print >>f, "public enum tokens {"
   for t in tokens: print >>f, t, ","
   print >>f, IdentOrKw, ","
   print >>f, Error, ","
   print >>f, Discard, ","
   print >>f, "}\n"

   print >>f, "public enum states {"
   for s in states: print >>f, s, ","
   print >>f, "}\n"

   print >>f, "// Body of keyword checker"
   for kw in keywords:
      print >>f, "else if (thetoken.toUpperCase().equals(\""+kw.upper()+"\"))",
      print >>f, "tok = tokens.", kw, ";"
   print >>f, "// End of body of keyword checker"

   for state in dfa:
      thisstate = state[0]
      action = state[1]
      moves = state[2]
      print >>f, "case", thisstate, ":"
      for move in moves:
         pred = move[0]
         newstate = move[1]
         print >>f, "if (" + pred + ") { newstate = states.", newstate, "; break; }"
      print >>f, doaction(action, thisstate, state), "; nexttoken = true; break;"
   return
