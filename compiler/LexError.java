//using System.Text;

class LexError extends Exception {

   public CharStream cs;
   public StringBuffer sb;
   public char ch;
   public String msg;

   public LexError ( CharStream cs, StringBuffer sb, char ch, String msg) {
      this.cs = cs;
      this.sb = sb;
      this.ch = ch;
      this.msg = msg;
   }
}
