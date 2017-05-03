// Class for user-level maps.  Maps strings to objects.
// Efficiency problem is not knowing how big a map will be,
// but not worrying about it now - this is more or less a prototype.

public class Map {
   public MapList themap;

   public Map () {
      themap = null;
   }

   public Map (MapList ml) {
      themap = ml;
   }

   public Map (String key, Object value) {
      themap = new MapList(key, value);
   }

   // Used in Value to coerce a Map to a List
   public MapList getMapList () {
      return themap;
   }

   public void assign (String key, Object value) {
      if (themap == null)
         themap = new MapList(key, value);
      themap = themap.assign(key, value);
   }

   public Object get (String key) {
      if (themap == null)
         return null;
      return themap.get(key);
   }

   public boolean containsKey (String key) {
      if (themap == null)
         return false;
      return themap.containsKey(key);
   }

   public Map clone () {
      if (themap == null)
         return null;
      return new Map(themap.clone());
   }

   public void putAll (Map m) {
      // Add all bindings in m to this map.
      // Collisions are decided in favor of m.
      if (themap == null)
         themap = m.themap;
      else {
         MapList ml = m.themap;
         while (ml != null) {
            assign(ml.key, ml.value);
            ml = ml.next;
         }
      }
   }

   public String toString () {
      if (themap == null)
         return "[]";
      else
         return "[" + themap + "]";
   }
}

