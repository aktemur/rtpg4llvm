# To do:  Handle comments and strings correctly
#         Fix syntax of antiquote
#         Grammar has to include "*" - it is already used in examples
#         How to handle the "C" operator?  Doesn't really make sense,
#            but is kind of convenient.  Without it, need to be sure
#            we're parsing all of C.

# Using the preprocessor:
# 
#     import preprocess
#     preprocess.process(filename-without-extension)
# 
# If the argument to process is <file>, it is assumed that the input is
# <file>.c.  The output will be put in <file>-pp.c.
# 
# The preprocessor still needs work, so much of this will change (i.e. be
# fixed), but this is the current state:

# o Quoted code is in Java notation, not C.  This is mostly the same
#   for simple statements and expressions, with two significant exceptions:
#   type declarations and the dereferencing (unary *) operator.  (The
#   latter is a significant omission and is a high priority for fixing.)
# 
# o Quotes have the form $expr< .. >$, $stmt<...>$, or $method< ... >$,
#   and antiquotes have the form `category( .. `), where category can
#   be expr, stmt, name, int, float, string, and char.  The last four
#   of these are for variables of the given type whose values at the
#   time of code generation should be placed in the generated code
#   as literals ("lifting").  Antiquote syntax `identifier is also
#   allowed; this is equivalent to `name( identifier `).
#
#   Note the closing parenthesis for antiquotes has a backquote.
# 
# o Strings and comments are not handled correctly.  This means that
#   if the occur in quotes, they must not contain code quotes or antiquote.
#   E.g.  $stmt< printf("closing quote is >$"); >$ will crash.
# 
# o Single-name antiquotes, which have the form `ident, are not allowed.

# Define getfraglist(text): takes string and returns list representing
# locations of quotes and antiquotes within it.  The list has the form:
#
#     fraglist = [frag, frag, ...]
#
# where each frag has the form [s, e, s', e', tag, fraglist].  Here,
# s and e are the starting and ending locations of the fragment,
# including the quote and endquote (or antiquote and endantiquote),
# and s' and e' are the start and end of the contents.  The tag
# is the name given in the opening quote (possibly empty).
# The fraglist is fragments contained within this quote or antiquote.
# N.B. the inner fraglist gives locations relative to the start of the
# text portion # of the containing fraglist.
# For example, suppose "$stmt< `exp(x+y) >$" is at location 5.
# The returned fragment would be:
#    [[5, 23, 11, 21, 'stmt', [[1, 9, 6, 8, 'exp', []]]]]
# To wit:
#    o The opening $stmt< occupies positions 5-10, and the corresponding
#      >$ occupies locations 22-23.  Thus, its enclosed text, ' `exp(x+y) ',
#      occupies locations 11-21.
#    o The enclosed `exp( begins at location 1 within the enclosed
#      text, and ends at 5.  The corresponding close parenthesis is
#      at location 9 within the enclosed text.  Its contents, 'x+y',
#      are at locations 6-8.
# Lastly, there is one special case: an antiquoted fragment consisting
# of just an identifier, e.g. `codeval.  The returned fragment follows
# the same principle, which means that s' = s+1 and e = e'.  Furthermore,
# in this case the tag is always "Ident".  This case should be checked by
# seeing if the tag is "Ident".

cursor = 0
txt = ""
quotebrace = None
antiquotebrace1 = None
antiquotebrace2 = None
closequotebrace = None

import re
def getfraglist (text):
   global cursor
   global txt
   global quotebrace
   global closequotebrace
   global antiquotebrace1
   global antiquotebrace2
   cursor = 0
   txt = text
   quotebrace = re.compile('\$[a-zA-Z]*<')
   antiquotebrace1 = re.compile('`[a-zA-Z]*\(')
   antiquotebrace2 = re.compile('`[a-zA-Z][a-zA-Z0-9]*[^\(]')
   closequotebrace = re.compile('(>\$)|(`\))')
   return fraglist(0)

# s is start of this fragment; included fragments are located
# relative to s
# Return list of fragments up to first unmatched closing brace,
# or eof
def fraglist (s):
   global cursor
   global txt
   while cursor < len(txt):
      ob = getopenbrace(txt, cursor)
      if ob != None:
         if ob[2] == "Ident":  # case of `ident
            cursor = ob[1]+1   # just past identifier
            morefrags = fraglist(s)  # relative to enclosing frag
            return [[ob[0]-s, ob[1]-s, ob[0]-s+1, ob[1]-s, "name",
                       []]] + morefrags
         else:  # case of `cat(
            cursor = ob[1]+1   # just past open brace
            innerfrags = fraglist(cursor)   # relative to this frag
            cb = getclosebrace(txt, cursor)
            cursor = cb[1]+1   # just past close brace
            morefrags = fraglist(s)  # relative to enclosing frag
            return [[ob[0]-s, cb[1]-s, ob[1]+1-s, cb[0]-1-s, ob[2],
                      innerfrags]] + morefrags
      elif closebrace(txt, cursor):
         return []
      else:
         cursor = cursor+1
   return []

# Match $...< or `...(.  The "..." gives the category (and may be empty).
# This only matches at start of txt; it does not "search."
# Return start and end locations of matched string, and category, or None.
# Additional special case:  `ident.  In this case, return start and end
# of entire thing, and the category "Ident".
def getopenbrace(txt, cursor):
   isaq2 = False
   m = quotebrace.match(txt, cursor)
   if m == None:
      m = antiquotebrace1.match(txt, cursor)
      if m == None:
         isaq2 = True
         m = antiquotebrace2.match(txt, cursor)
   if m == None:
      return None
   # Regexp match gives location of start of match, and then just past
   # end of match.  So subtract one from end.
   start = m.start(0)
   end = m.end(0)-1
   if isaq2:
      tag = "Ident"
      # For this case, pattern actually matched one more character than
      # is actually included (the non-( char after the ident), so
      # subtract one more
      end = end-1
   else:
      tag = m.group(0)[1:-1]
   return [start, end, tag]

def closebrace(txt, cursor):
   m = closequotebrace.match(txt, cursor)
   return m != None

def getclosebrace(txt, cursor):
   m = closequotebrace.match(txt, cursor)
   return [m.start(0), m.end(0)-1]

# Process quotes and antiquotes by transforming source code.
# Idea is to replace quotation $< ... >$ by constructor calls to
# construct the AST for the quoted text.  For example, $< x+1 >$
# might be replaced by "mkPlus(mkIdent("x"), mkIntconst(1))".
#
# Complication arises from two sources:
# 1. To parse fragments that contain antiquotes, need to replace
#    the antiquote by a parseable string.  We use "spliced<cat>", where
#    <cat> is the name of a syntactic category like Stmt or Exp.
#    After parsing, the original antiquoted code needs to be put back.
# 2. Since antiquotes can contain quotes, and so on, the process is
#    recursive.
#
# This translation starts by calling getfraglist above, which then
# guides the process described here.
#
# The process is a mutual recursion of two functions:
#
# - processtoplevel handles code that is either not within quotes
#   at all or is within an antiquote; in either case, the job is
#   simply to extract the # quotes, process them, and replace them.
# - processquote handles quoted text, which can include antiquotes.
#   It needs to process the antiquotes (recursively), getting some
#   text to replace them with, then parse the quoted text after
#   replacing antiquotes by category names, and then put the antiquote
#   text back in where the category names were.
def processprogram (txt):
   return processtoplevel(txt, getfraglist(txt))

def processtoplevel(txt, frags):
   # Quote within txt are just processed individually, so
   # start by extracting each one:
   quotes = map (lambda f: txt[f[2]:f[3]+1], frags)
   # then recursively process quotes, handlng enclosed frags
   processedquotes = map (lambda p: processquote(p[0], p[1][4], p[1][5]),
                          zip(quotes, frags))
   # then replace quotes with processed text
   result = splicequotes(txt, frags, processedquotes)
   return result

# replace(str, s, e, str2) removes characters from positions s to e in
# str and replaces them by str2
def replace(str, s, e, str2):
   return str[:s] + str2 + str[e+1:]

# Replace frags in txt by new strings.  frags have format given above.
# newfrags are strings.  Length of frags list is equal to length of
# newfrags list.  
def splicequotes(txt, frags, newfrags):
   if frags == []:
      return txt
   frag = frags[0]
   newfrag = newfrags[0]
   result = replace(splicequotes(txt, frags[1:], newfrags[1:]),
                    frag[0], frag[1], newfrag)
   return result

def processquote(txt, tag, frags):
   # Starts same as processtoplevel:
   # Antiquotes within txt are just processed individually, so
   # start by extracting each one:
   aquotes = map (lambda f: txt[f[2]:f[3]+1], frags)
   # and recursively process
   processedaquotes = map (lambda p: processtoplevel(p[0], p[1][5]),
                           zip(aquotes, frags))
   # For each aquote, need to replace by cat name, etc.  Construct
   # list of parameters for each aquote.
   aquoteinfo = zip(frags, processedaquotes)
   # replace aquote by cat name and associated text
   catnames = map(splicecat, aquoteinfo)
   parseable = splicequotes(txt, frags, catnames)
   print "processquote: ", parseable
   # and parse with syntactic type given by tag
   parsedtxt = parse(tag, parseable)
   return parsedtxt

def splicecat (aqi):
   cat = aqi[0][4]
   code = aqi[1]
   if cat == "int" or cat == "float" or cat=="string" or cat = "char":
      code = cat + "lit("+code+")"
      cat = "expr"
   return "__" + cat +"Spliced"+"__ $<"+code+">$"
   
# parse(tag, txt):  Parse txt in syntactic category tag.
# Furthermore, txt may contain spliced text having the form
#       <cat>Spliced $< sometext >$
# This should be parsed as <cat>Spliced - i.e. with an unknown
# fragment of type <cat> - and then when the parsed text is pretty-
# printed, that should be replaced by sometext.
# For example, if the thing being parsed as an Exp is
# x+ExpSpliced $< getexp(j) >$, then this should return something
# like:  mkAdd(mkVar("x"), getexp(j)).
import os
import sys
def parse(tag, txt):
   fragfile = open("fragmentin.java", 'w')
   fragfile.write("/*"+tag+"*/\n")
   if tag == "stmt":
      fragfile.write("{ ")
   fragfile.write(txt)
   if tag == "stmt":
      fragfile.write(" }")
   elif tag == "expr":
      fragfile.write(" ;")
   fragfile.close()
   #print "fragmentin: ", txt
   fout = open("fragmentout.java", "w")
   os.system("java Main fragmentin.java fragmentout.java")
   prog = readfile("fragmentout.java")
   #print "fragmentout: ", prog
   return prog

def readfile(file):
   fin = open(file, 'r')
   if fin == None:
      print n, " missing"
      return
   ln = fin.readline()
   codestr = ln
   while ln <> '':
      ln = fin.readline()
      codestr = codestr + ln
   fin.close()
   return codestr

def process(file):
   codestr = readfile(file+'.c')

   prog = processprogram(codestr)

   fout = open(file + '-pp.c', 'w')
   fout.write(prog)
   return
