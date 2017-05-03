import java.util.ArrayList;
//using System.Collections.Generic;

public class TokenList {
   public ArrayList<Token> tokenlist; // list of Token objects

   public static NameTable names = null;

   // Client may add some pre-defined names to the name table
   // before lexing any files.
   public TokenList () {
      if (names == null)
         names = new NameTable();
      tokenlist = new ArrayList<Token>();
   }

   public void add (tokens t, int startpos, int endpos) {
      tokenlist.add(new Token(t, null, startpos, endpos));
   }

   public void add (tokens t, int i, int startpos, int endpos) {
      tokenlist.add(new Token(t, new MyInteger(i), startpos, endpos));
   }

   public void add (tokens t, double d, int startpos, int endpos) {
      tokenlist.add(new Token(t, new MyDouble(d), startpos, endpos));
   }

   public void add (tokens t, char c, int startpos, int endpos) {
      tokenlist.add(new Token(t, new MyInteger(c, true), startpos, endpos));
   }

   public void add (tokens t, String s, int startpos, int endpos) {
      if (t == tokens.Stringlit) {
         tokenlist.add(new Token(t, s.substring(1, s.length()-1),
                       startpos, endpos));
      }
      else if (t == tokens.Antiquote) {
         tokenlist.add(new Token(t, s.substring(2, s.length()-2),
                       startpos, endpos));
      }
      else {
	 IdentInfo ii = names.get(s);
	 if (ii == null)
            ii = names.put(s);
         tokenlist.add(new Token(t, ii, startpos, endpos));
      }
   }

   public String toString () {
      String rep = "";
      int i = 0;
      while (i != tokenlist.size()) {
         Token thistoken = tokenlist.get(i);
         tokens thistok = thistoken.toktype;
         Object thisval = thistoken.tokvalue;
         String thisrep = i++ + ": " + thistok + " ";
	 switch (thistok) {
         case Identifier: thisrep += (IdentInfo)thisval; break;
         case Stringlit: thisrep += (String)thisval; break;
         case Intlit: thisrep += ((MyInteger)thisval).toString(); break;
         case Doublelit: thisrep += ((MyDouble)thisval).doubleValue(); break;
         case Charlit: thisrep += ((MyInteger)thisval).toString(); break;
	 }
	 rep += thisrep + "("+ thistoken.startloc +"-"
                 + thistoken.endloc + ")\n";
      }
      return rep;
   }

   public int cursor = 0;
   public Object lastval = null;

   public Token nexttoken () {
      Token T = tokenlist.get(cursor);
      lastval = T.tokvalue;
      cursor++;
      return T;
   }
        
   public Object getvalue () {
      // Get value of last-returned token
      return lastval;
   }

   // Look at i(th) next token, counting from 0, i.e. peektoken(0)
   // looks at the next token.
   public tokens peek (int i) {
      Token T = tokenlist.get(cursor+i);
      if (T == null)
         return tokens.Eof;      // Eof is last item in list
      else
         return T.toktype;
   }
}



