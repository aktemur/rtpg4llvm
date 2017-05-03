/*
 *  Class for concrete syntax trees
 *
*/
import java.util.ArrayList;
//using System.Collections.Generic;

public class CST {

   boolean istoken;

   public tokens thetoken;
   public Object thetokenvalue;

   public nonterms A;
   public ArrayList<CST> children;

   public int startpos, endpos;  // For error messages

   public CST (tokens t, Object val, int sp, int ep) {
      istoken = true; thetoken = t; thetokenvalue = val;
      startpos = sp; endpos = ep;
   }

   public CST (nonterms A, ArrayList<CST> children, int sp, int ep) {
      istoken = false; this.A = A; this.children = children;
      startpos = sp; endpos = ep;
   }

   public boolean isToken () { return istoken; }

   public tokens getToken () { return thetoken; }
   public Object getValue () { return thetokenvalue; }

   public nonterms getNonterm () { return A; }
   public ArrayList<CST> getChildren () { return children; }

   public String toString () {
      return toStringIndent(0);
   }

   public String toStringIndent (int n) {
      String sb = blanks(n);
      if (istoken) {
         sb += thetoken.toString();
         if (thetokenvalue != null)
            sb += ':'+thetokenvalue.toString();
         sb += "("+startpos+"-"+endpos+")";
         sb += '\n';
      }
      else {
         sb += A.toString();
         sb += " ("+startpos+"-"+endpos+")";
	 sb += '\n';
         for (int i=0; i<children.size(); i++) {
            CST t = children.get(i);
            sb += t.toStringIndent(n+2);
	 }
      }
      return sb;
   }

   public static String b = "                                                "
                    +"                                                ";
   public String blanks (int n) {
      if(n > b.length())
          n = b.length();
      return b.substring(0, n);
   }
}
