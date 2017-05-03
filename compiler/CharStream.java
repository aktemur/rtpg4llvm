import java.io.*;
//using System.Text;

public class CharStream {
   public String filename;
   public int linenum;
   public int cursor, filelength;
   public byte[] buffer;
   public static char eof = '\0';

   public CharStream (byte[] buffer, String f) throws IOException
   {
      filename = f;
      this.buffer = buffer;
      filelength = buffer.length;
      cursor = 0;
      linenum = 1;
      return;
   }
 
   public char getChar() {
      if (cursor == filelength)
         return eof;
      char ch = (char)(buffer[cursor]);
      if (ch == '\n') linenum++;
      cursor++;
      return ch;
   }

   public char peek (int i) {
     if (cursor+i >= filelength)
        return eof;
     return (char)(buffer[cursor+i]);
   }

   public char peek () {
     return peek(0);
   }

   // Find line number corresponding to this character position
   public int getlineof (int cursor) {
      int line=1;
      for (int i=0; i<cursor; i++)
         if (buffer[i] == '\n') line++;
      return line;
   }

   // Find column corresponding to this character position
   public int getcolumnof (int cursor) {
      int lastline = 0;
      for (int i=0; i<cursor; i++)
         if (buffer[i] == '\n') lastline = i;
      return cursor-lastline;
   }

   // Get given line, as a string
   public String getline (int line) {
      int i = 0, j;
      if (line == 0) {
         StringBuffer sb = new StringBuffer();
         while (buffer[i] != '\n' && buffer[i] != '\r') {
            sb.append((char)(buffer[i]));
            i++;
         }
         return sb.toString();
      }
      int ln = 1;
      while (ln != line && i < buffer.length) {
         if (buffer[i] == '\n') ln++;
         i++;
      }
      if (i == buffer.length)
         return null;  // line doesn't exist
      for (j=i+1; j < buffer.length; j++)
         if (buffer[j] == '\n') {
            StringBuffer sb = new StringBuffer();
            for (int k=i; k<j; k++)
               sb.append((char)(buffer[k]));
            return sb.toString();
         }
      return null;  // can't happen
   }
}

