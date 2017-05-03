//using System.Text;
import java.util.ArrayList;
//using System.Collections.Generic;

public class Value {

   // ObjectV is used for non-djava values - i.e. values from
   // outside, such as TreeNode objects.
   public enum valtype {IntV, DoubleV, BooleanV, StringV, ListV,
                 DictV, NullV, VoidV, ObjectV};

   public boolean isNumeric () {
      return (thetype == valtype.IntV ||
              thetype == valtype.DoubleV);
   }

   // Maximum type among arithmetic types: Int, Double, Char
   public static valtype maxtype (valtype t1, valtype t2) {
      if (t1 == t2)
         return t1;
      if (t1 == valtype.IntV)
         return t2;
      if (t2 == valtype.IntV)
         return t1;
      return valtype.NullV;  // Should never get here
   }

   public valtype thetype;
   // thevalue is an object of one of the following types:
   // Boolean, Integer, Double, Character,
   // ArrayList<Value>, Map. Null and Void have no associated value.
   public Object thevalue;

   public static Value Void = voidval();
   public static Value Null = nullval();
   public static Value EmptyList = new Value(new ArrayList<Value>());
   public static Value False = falseval();
   public static Value True = trueval();
   public static Value Zero = new Value(0);
   public static Value One = new Value(1);
   public static Value OnePointOh = new Value(1.0f);

   // Check if this is "true" value.  This should always be the
   // True or False object, but may be separately created but equivalent.
   // Check identity first because it's more efficient.
   public boolean isTrue () {
      return this == True
             || (thetype == valtype.BooleanV
                     && ((MyBoolean)thevalue).i);
   }

   // Default value of the given type
   // STUB!
   public static Value defaultvalue (AST type) {
      return Null;
   }

   // Used to create Void, Null, or Object values
   Value () { }

   static Value voidval () {
      Value v = new Value();
      v.thetype = valtype.VoidV;
      return v;
   }

   static Value nullval () {
      Value v = new Value();
      v.thetype = valtype.NullV;
      return v;
   }

   static Value trueval () {
      Value v = new Value();
      v.thetype = valtype.BooleanV;
      v.thevalue = new MyBoolean(true);
      return v;
   }

   static Value falseval () {
      Value v = new Value();
      v.thetype = valtype.BooleanV;
      v.thevalue = new MyBoolean(false);
      return v;
   }

   public static Value objectval (Object o) {
      Value v = new Value();
      v.thetype = valtype.ObjectV;
      v.thevalue = o;
      return v;
   }

   public Value (MyInteger i) {
      thetype = valtype.IntV;
      thevalue = i;
   }

   public Value (int i) {
      thetype = valtype.IntV;
      thevalue = new MyInteger(i);
   }

   public Value (char i) {
      thetype = valtype.IntV;
      thevalue = new MyInteger(i, true);
   }

   public Value (MyDouble i) {
      thetype = valtype.DoubleV;
      thevalue = i;
   }

   public Value (double i) {
      thetype = valtype.DoubleV;
      thevalue = new MyDouble(i);
   }

   public Value (boolean b) {
      // Only for boolean values
      thetype = valtype.BooleanV;
      thevalue = new MyBoolean(b);
   }

   public Value (String s) {
      // Only for boolean values
      thetype = valtype.StringV;
      thevalue = s;
   }

   public Value (ArrayList<Value> vl) {
      thetype = valtype.ListV;
      thevalue = vl;
   }
   
   public Value (Map m) {
      thetype = valtype.DictV;
      thevalue = m;
   }

   // Retrieve int from IntV; caller checks type
   public int getIntvalue () {
      return ((MyInteger)thevalue).intValue();
   }

   // Retrieve bool from BooleanV; caller checks type
   public boolean getBooleanvalue () {
      return ((MyBoolean)thevalue).booleanValue();
   }

   // Retrieve double from FoubleV; caller checks type
   public double getDoublevalue () {
      return ((MyDouble)thevalue).doubleValue();
   }

   // Retrieve String from StringV; caller checks type
   public String getStringvalue () {
      return (String)thevalue;
   }

   // Retrieve list from ListV; caller checks type
   public ArrayList<Value> getListvalue () {
      return (ArrayList<Value>)thevalue;
   }

   // Retrieve Map from DictV; caller checks type
   public Map getDictvalue () {
      return (Map)thevalue;
   }

   // Retrieve object from ObjectV; caller checks type
   public Object getObjectvalue () {
      return thevalue;
   }

   // Concatenate two lists of values (non-destructively)
   public static ArrayList<Value> concat (ArrayList<Value> l1,
                                          ArrayList<Value> l2) {
      //return ArrayList<Value>.concat(l1, l2);
      ArrayList<Value> L = new ArrayList<Value>();
      L.addAll(l1);
      L.addAll(l2);
      return L;
   }

   // Concatenate two dictionaries (non-destructively)
   public static Map concat (Map d1, Map d2) {
      Map h = d1.clone();
      h.putAll(d2);
      return h;
   }

   public ArrayList<Value> coerceToList ()
                       throws MsgError
   {
      if (thetype == valtype.ListV)
         return (ArrayList<Value>)thevalue;
      else if (thetype == valtype.DictV)
         return mapToList();
      else if (thetype == valtype.StringV)
         return stringToList();
      else
         throw new MsgError("");
   }
 
   // Called on String value; returns list of char's
   public ArrayList<Value> stringToList () {
      ArrayList<Value> vl = new ArrayList<Value>();
      String v = (String)thevalue;
      for (int i=0; i<v.length(); i++)
         vl.add(new Value(v.charAt(i)));
      return vl;
   }

   // Convert list of char's to string.  Caller checks argument.
   public String listToString () {
      StringBuffer sb = new StringBuffer();
      ArrayList<Value> vl = getListvalue();
      for (int i=0; i<vl.size(); i++)
          sb.append((char)(vl.get(i).getIntvalue()));
      return sb.toString();
   }
      

   // Called on Dict value; returns list of char's
   public ArrayList<Value> mapToList () {
      ArrayList<Value> vl = new ArrayList<Value>();
      MapList m = ((Map)thevalue).getMapList();
      while (m != null) {
         ArrayList<Value> pair = new ArrayList<Value>();
         pair.add(new Value(m.key));
         pair.add(((Value)(m.value)));
         vl.add(new Value(pair));
         m = m.next;
      }
      return vl;
   }

   // Get length for list or string - more efficient
   // than using coerceToList and taking length.
   // BUG: Should also work for dict.
   public int count ()
             throws MsgError
   {
      if (thetype == valtype.ListV)
         return ((ArrayList<Value>)thevalue).size();
      else if (thetype == valtype.StringV)
         return ((String)thevalue).length();
      else
         throw new MsgError("Can only apply count to list or string");
   }

   // Automatically convert anything to StringV value, except Null.
   // This is for SliceBridge - called functions generally care if
   // a value is null, and should not give passed string "null".
   public String convertToString () {
      if (thetype == valtype.NullV)
         return null;
      else
         return toString();
   }

   public String toString () {
      switch (thetype) {
      case IntV:
         return ((MyInteger)thevalue).toString();
      case DoubleV:
         return ((MyDouble)thevalue).toString();
      case BooleanV:
         return ((MyBoolean)thevalue).toString();
      case StringV:
         return (String)thevalue;
      case ListV:
         return listToDisplayString((ArrayList<Value>)thevalue);
      case DictV:
         return ((Map)thevalue).toString();
      case NullV:
         return "null";
      case VoidV:
         return "void";
      case ObjectV:
         return thevalue.toString();
      }
      return "";
   }

   void print () {
      System.out.println(this);
   }

   // Convert value to int if possible.
   // Throws MsgError giving value that could not be converted.
   // (Does not give fully formattted error message, because this
   // kind of error arises in different contexts.)
   public Value toInt ()
             throws MsgError
   {
      if (thetype == valtype.IntV)
         return this;
      if (thetype == valtype.DoubleV)
         return new Value((int)(getDoublevalue()));
      if (thetype == valtype.StringV)
         try {
            return new Value(MyInteger.parseInt(getStringvalue()));
         }
         catch (NumberFormatException nfe) {
            throw new MsgError(getStringvalue());
         }
      throw new MsgError(toString());
   }
   
   // Convert value to double if possible.
   // Throws MsgError giving printable error message (as distinct
   // from toInt).
   public Value toDouble ()
              throws MsgError
   {
      if (thetype == valtype.StringV)
         try {
            return new Value(MyDouble.parseDouble(getStringvalue()));
         }
         catch (NumberFormatException nfe) {
            throw new MsgError("String " + getStringvalue() +
                               " not formatted as double");
         }
      else if (thetype == valtype.IntV)
         return new Value(0.0f + getIntvalue());
      throw new MsgError("Wrong type of argument to toDouble");
   }

   // Apply a static method of this class.
   // Throw MsgError if undefined (this was only invoked
   // because the call had the form Value.meth(), so if meth is
   // undefined, it is a clear-cut error).
   public static Value applyStatic (String name, ArrayList<Value> args)
                    throws MsgError
   {
      // There are no callable static methods in this class yet.
      throw new MsgError("");
   }

   // Apply a (non-static) method of this class.
   // Throw NodefError if undefined (it is possible that
   // the method will be found in Slice).
   public static Value applyMethod (Value rcvr,
                               String meth, ArrayList<Value> args)
                    throws MsgError, NodefError
   {

      if (meth.equals("toString")) {
         if (args.size() != 0)
            throw new MsgError("Wrong number of arguments to toString");
         else
            return new Value(rcvr.toString());
      }

      if (meth.equals("toInt")) {
         if (args.size() != 0)
            throw new MsgError("Wrong number of arguments to toInt");
         else
            try {
               return rcvr.toInt();
            }
            catch (MsgError me) {
               throw new MsgError("String "+me.msg+" not in int format");
            }
      }

      if (meth.equals("toDouble")) {
         if (args.size() != 0)
            throw new MsgError("Wrong number of arguments to toDouble");
         else
            return rcvr.toDouble();
      }

      if (meth.equals("count")) {
         if (args.size() != 0)
            throw new MsgError("Wrong number of arguments to toDouble");
         else
            return new Value(rcvr.count());
      }

      if (meth.equals("print")) {
         if (args.size() != 0)
            throw new MsgError("Wrong number of arguments to toDouble");
         else {
            rcvr.print();
            return Void;
         }
      }

      throw new NodefError();
   }
   
   // Turn IntV into character by setting bit in MyInteger field
   public void makeChar () {
      ((MyInteger)thevalue).isChar = true;
   }

   // Return copy of al from index i to end
   public static ArrayList<Value> nthtail(ArrayList<Value> al, int i) {
      ArrayList<Value> bl = new ArrayList<Value>();
      for (int j=i; j<al.size(); j++)
         bl.add(al.get(j));
      return bl;
   }

   // Converts list consisting entirely of characters to string
   public static String listToString (ArrayList<Value> vl)
           throws MsgError
   {
      StringBuffer sb = new StringBuffer();
      for (int i=0; i<vl.size(); i++) {
         Value v = vl.get(i);
         if (v.thetype != Value.valtype.IntV)
            throw new MsgError("Assigned non-char to string element");
         int thechar = v.getIntvalue();
         // BUG: Only handling ASCII
         if (thechar < 0 || thechar > 255)
            throw new MsgError("Assigned non-char to string element");
         sb.append((char)thechar);
      }
      return sb.toString();
   }

   public static String listToDisplayString (ArrayList<Value> vl) {
      StringBuffer sb = new StringBuffer();
      sb.append("[");
      for (int i=0; i< vl.size(); i++) {
         sb.append(vl.get(i));
         if (i < vl.size()-1)
            sb.append(", ");
      }
      sb.append("]");
      return sb.toString();
   }

}
