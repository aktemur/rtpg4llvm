/*
 *  Class for abstract syntax trees
 *  At the moment, this is exactly like the class for
 *  concrete syntax trees, except for the replacement of "nonterms"
 *  by "abssynops".
 *
*/
import java.util.ArrayList;
//using System.Collections.Generic;

public class AST {

   public boolean istoken;

   public tokens thetoken;
   public Object thetokenvalue;

   public abssynops A;
   public ArrayList<AST> children;

   public int startpos, endpos; // corresponding locations in file;
                            // same as in CST

   public AST (tokens t, Object val, int sl, int el) {
      istoken = true; thetoken = t; thetokenvalue = val;
      startpos = sl; endpos = el;
   }

   public AST (abssynops A, ArrayList<AST> children, int sl, int el) {
      istoken = false; this.A = A; this.children = children;
      startpos = sl; endpos = el;
   }

   public AST (abssynops A, AST t, int sl, int el) {
      istoken = false; this.A = A;
      ArrayList<AST> children = new ArrayList<AST>();
      children.add(t);
      this.children = children;
      startpos = sl; endpos = el;
   }

   public AST (abssynops A, AST t, AST t2, int sl, int el) {
      istoken = false; this.A = A;
      ArrayList<AST> children = new ArrayList<AST>();
      children.add(t);
      children.add(t2);
      this.children = children;
      startpos = sl; endpos = el;
   }

   public AST (abssynops A, AST t, AST t2, AST t3, int sl, int el) {
      istoken = false; this.A = A;
      ArrayList<AST> children = new ArrayList<AST>();
      children.add(t);
      children.add(t2);
      children.add(t3);
      this.children = children;
      startpos = sl; endpos = el;
   }

   public AST (abssynops A, AST t, AST t2, AST t3, AST t4, int sl, int el) {
      istoken = false; this.A = A;
      ArrayList<AST> children = new ArrayList<AST>();
      children.add(t);
      children.add(t2);
      children.add(t3);
      children.add(t4);
      this.children = children;
      startpos = sl; endpos = el;
   }

   public boolean isToken () { return istoken; }

   public tokens getToken () { return thetoken; }
   public Object getValue () { return thetokenvalue; }

   public abssynops getAbssynop () { return A; }
   public ArrayList<AST> getChildren () { return children; }
   public AST getChild (int i) { return children.get(i); }

//   // Abs. syn. operators can have children that are lists of
//   // ASTs.  To handle this, add abssynop "list" as a placeholder.
//   // If an abssynop has, say, three lists as its children, it will
//   // instead use three ASTs with operator "list".  Call getChildList(i),
//   // i = 0,..,2, to get the three lists.
//   public ArrayList<AST> getChildList (int i) {
//	  AST c = children.get(i);
//	  if (c.A == abssynops.list)
//            return c.children;
//	  else
//            return null;  // can't happen if tree used correctly
//   }

   public String toString () {
      return toStringIndent(0);
   }

   public String toStringIndent (int n) {
      String sb = blanks(n);
      if (istoken) {
         sb += thetoken.toString();
         if (thetokenvalue != null)
            sb += ':'+thetokenvalue.toString();
         sb += " ("+startpos+"-"+endpos+")";
         sb += '\n';
      }
      else {
         sb += A.toString();
         sb += " ("+startpos+"-"+endpos+")";
	 sb += '\n';
	 if (children.size() != 0)
            for (int i=0; i<children.size(); i++) {
               AST t = children.get(i);
               if (t == null)
                  continue;
               sb += t.toStringIndent(n+2);
	    }
      }
      return sb;
   }

   static String b = "                                                "
                    +"                                                ";
   String blanks (int n) {
      return b.substring(0, n);
   }

   // Operators to access parts of specific AST nodes - easier than
   // using getChildren().get(i).
      
   // Get formals of a method
   public ArrayList<AST> getFormals () {
      if (A != abssynops.function)
         return null;  // throw exception
      return children.get(2).getChildren();
   }
      
   // Get body of a method
   public AST getBody () {
      if (A != abssynops.function)
         return null;  // throw exception
      return children.get(3);
   }
      
/*
   // Get the rest of the names and/or args from a nameorargslist.
   public ArrayList<AST> getRestOfNames () {
      if (A != abssynops.nameorargslist)
         return null;  // STUB: throw exception
      return XLList.tail(children);
   }
*/

   // Get operator as a token, for binop and unary ops.
   public tokens getOptr () {
      if (A != abssynops.binaryop &&
          A != abssynops.unarypreoptr &&
          A != abssynops.unarypostoptr)
         return tokens.Plus; // STUB: throw exception
      return children.get(0).thetoken;
   }

   public AST getReturnexpr () {
      if (A != abssynops.returnstmt)
         return null;  // STUB: throw exception
      if (children.size() == 0)
         return null;
      else
         return children.get(0);
   }
}
