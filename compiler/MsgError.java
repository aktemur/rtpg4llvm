// Used internally to pass an error in cases where
// the AST that produced it is not available.  These errors
// are caught and then re-thrown as ExecutionErrors with the AST.
class MsgError extends Exception {
   public String msg;

   public MsgError (String msg) {
      this.msg = msg;
   }
}
