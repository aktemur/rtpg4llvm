public class ClassInfo {
   public int classname;
   public String file;
   public CharStream cs;

   public ClassInfo (int classname, String file, CharStream cs) {
      this.classname = classname;
      this.file = file;
      this.cs = cs;
   }
}
