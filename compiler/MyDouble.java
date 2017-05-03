public class MyDouble {

   public double i;

   public MyDouble (double i) {
      this.i = i;
   }

   public double doubleValue() {
      return i;
   }

   public String toString () {
      return ""+i;
   }


   public static double parseDouble (String s) {
      return Double.parseDouble(s);
   }
}
