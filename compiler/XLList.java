//using System.Text;
import java.util.ArrayList;

// Class for static methods on lists, used to make code portable
// between Java and C#.

// Aktemur: I removed get methods because Eclipse refuses to compile
// this file because methods have the same erasure causing a type conflict.
// See https://bugs.eclipse.org/bugs/show_bug.cgi?id=317719 for a discussion.

public class XLList {

   public static void addAtStart(ArrayList<AST> al, AST a) {
       al.add(0, a);
   }
   
   public static void addAtStart(ArrayList<ClassMethodPair> al,
                                 ClassMethodPair a) {
       al.add(0, a);
   }
   
   public static void addAtStart(ArrayList<String> al, String a) {
       al.add(0, a);
   }
   
   public static void addAtStart(ArrayList<CharStream> al, CharStream a) {
       al.add(0, a);
   }
   
/*
   // Tired of wrangling with the type system.  Just use different name.
   public static Object getx(ArrayList tl, int i) {
       return tl.get(i);
   }
*/

   public static ArrayList<ArrayList<ClassMethodPair>>
    getx(ArrayList<ArrayList<ArrayList<ClassMethodPair>>> tl, int i) {
       return tl.get(i);
   }

   public static void addAtStart(ArrayList<SimpleEnv> al, SimpleEnv a) {
       al.add(0, a);
   }

   public static void set(ArrayList<Value> al, int i, Value v) {
      al.set(i, v);
   }
   
}
