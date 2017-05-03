// Auxiliary to Map.  Maps strings to objects.
// Just a temporary hack.

public class MapList {
   public String key;
   public Object value;
   public MapList next;

   public MapList (String key, Object value) {
      this.key = key;
      this.value = value;
      next = null;
   }

   public MapList (String key, Object value, MapList next) {
      this.key = key;
      this.value = value;
      this.next = next;
   }

   public MapList getNode (String key) {
      if (this.key.equals(key))
         return this;
      else if (next == null)
         return null;
      else
         return next.getNode(key);
   }

   public boolean containsKey (String key) {
      return getNode(key) != null;
   }

   public MapList assign (String key, Object value) {
      MapList node = getNode(key);
      if (node == null)
         return new MapList(key, value, this);
      else {
         node.value = value;
         return this;
      }
   }

   public Object get (String key) {
      MapList ml = getNode(key);
      return ml == null ? null : ml.value;
   }

   public MapList clone () {
      MapList n = null;
      if (next != null)
         n = next.clone();
      return new MapList(key, value, n);
   }

   public String toString () {
      return key + "->" + value + (next == null? "" : ", "+next);
   }
}

