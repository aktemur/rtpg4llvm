public class IdentInfo {
   public String thename;
   public int thehashedname;

   // A number too big to be an identifier
   public static int noident = 1000000;

   public IdentInfo (String name, int hashedname) {
      this.thename = name;
      this.thehashedname = hashedname;
   }

   public String name () {
      return thename;
   }

   public int hashedname () {
      return thehashedname;
   }

   public String toString () {
      return thename + "(" + thehashedname + ")";
   }
}

