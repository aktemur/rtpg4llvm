public class MyFloat {

   public float i;

   public MyFloat (float i) {
      this.i = i;
   }

   public float floatValue() {
      return i;
   }

   public String toString () {
      return ""+i;
   }


   public static float parseFloat (String s) {
      return Float.parseFloat(s);
   }
}
