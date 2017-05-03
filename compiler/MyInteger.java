// Represents ints and chars.  If isChar bit is set, print as
// char, otherwise as int.

public class MyInteger {

   public int i;
   public boolean isChar;

   public MyInteger (int i, boolean isChar) {
      this.i = i; this.isChar = isChar;
   }

   public MyInteger (int i) {
      this.i = i;
   }

   public int intValue() {
      return i;
   }

   public String toString () {
      if (isChar)
         return "\'" + (char)i + "\'";
      else
         return ""+i;
   }

   public static int parseInt (String s) {
      return Integer.parseInt(s);
   }
}
