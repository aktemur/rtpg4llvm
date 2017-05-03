public class SimpleEnv {

   // implements a simple-minded linked list for local environments
   public int ident;
   public Value val;
   public SimpleEnv next;

   public SimpleEnv () {
      // seed with an identifier that will never be seen.
      ident = IdentInfo.noident;
   }

   public SimpleEnv (int ident, Value val, SimpleEnv e) {
      this.ident = ident;
      this.val = val;
      this.next = e;
   }

   // add - add a new id and value.
   // Operates by side effect.
   // If id already present, do nothing.
   public void add(int id, Value o) {
      if (contains(id))
         return;
      // Clone this node
      SimpleEnv se = new SimpleEnv(ident, val, next);
      ident = id;
      val = o;
      next = se;
   }

   // assign - add a new id and value.
   // Operates by side effect.
   // If id not already present, add it.
   public void assign(int id, Value o) {
      SimpleEnv se = lookup(id);
      if (se == null)
         add(id, o);
      else
         se.val = o;
   }
         
   // contains - check if this env contains id
   public boolean contains (int id) {
      return lookup(id) != null;
   }

   // valueof - value associated with this id, or null
   public Value valueof (int id) {
      SimpleEnv se = lookup(id);
      if (se == null)
         return null;
      else
         return se.val;
   }

   // lookup - auxiliary to above.  Get node containing this id, or null
   public SimpleEnv lookup (int id) {
      if (id == ident)
         return this;
      else if (next == null)
         return null;
      else
         return next.lookup(id);
   }

   public String toString () {
      if (ident == IdentInfo.noident)
         return "";
     else
        return ident + "(" + val + ")" + next.toString();
   }

}
      
