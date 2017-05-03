public class ClassMethodPair {
   public int classname;
   public int methname;
   public AST methdef;

   public ClassMethodPair(int classname, AST methdef, int methname) {
      this.classname = classname;
      this.methdef = methdef;
      this.methname = methname;
   }
}
