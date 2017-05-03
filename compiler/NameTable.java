
// Contains the list of all identifiers, and their hashed values
//
public class NameTable {
   // Map is not designed for large dictionaries; need to replace
   // by more efficient version.
   public Map hashedidents = new Map();
   public int numidents = 0;

   public NameTable () { }

   // Look up string s in hashedidents.  If present, return
   // its IdentInfo, otherwise return null;
   public IdentInfo get (String s) {
      return (IdentInfo)(hashedidents.get(s));
   }

   // Add string s to hashedident; assign it a hashed name
   // and return its IdentInfo.
   public IdentInfo put (String s) {
      IdentInfo ii = new IdentInfo(s, numidents);
      hashedidents.assign(s, ii);
      numidents++;
      return ii;
   }
}
