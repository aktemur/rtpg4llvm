public class Token {
   public tokens toktype;
   public Object tokvalue;
   public int startloc, endloc; // starting and ending locations in file
                                // filename (or rather, its associated
                                // CharStream) is recorded elsewhere

   public Token (tokens toktype, Object tokvalue,
                 int startloc, int endloc) {
      this.toktype = toktype;
      this.tokvalue = tokvalue;
      this.startloc = startloc;
      this.endloc = endloc;
  }

  public String toString2 () {
      return tokvalue + "";
  }

  public String toString () {
      return toktype + "";
  }
}

