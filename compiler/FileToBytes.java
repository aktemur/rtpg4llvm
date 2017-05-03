import java.io.*;

// Class with one static method:  getBytes(filename) opens
// file filename and returns an array of bytes.
public class FileToBytes {

   public static byte[] getBytes (String file) throws IOException
   {
      byte[] buffer = new byte[(int) new File(file).length()];
      BufferedInputStream f = new BufferedInputStream(
		                 new FileInputStream(file));
      f.read(buffer);
      return buffer;
   }
}

