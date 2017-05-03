class SyntaxError extends Exception {

   public String msg;
   //public CharStream cs;
   public String nonterm;
   public Token prevtoken;
   public int startpos;

   public SyntaxError (// CharStream cs,
             String nonterm, Token prevtoken,
                       int startpos) {
      this.msg = null;
      //this.cs= cs;
      this.nonterm = nonterm;
      this.prevtoken = prevtoken;
      this.startpos = startpos;
   }

   public SyntaxError (String msg, // CharStream cs,
                       String nonterm,
                       Token prevtoken, int startpos) {
      this.msg = msg;
      // this.cs= cs;
      this.nonterm = nonterm;
      this.prevtoken = prevtoken;
      this.startpos = startpos;
   }
}
