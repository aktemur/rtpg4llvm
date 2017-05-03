
import java.io.*;
import java.util.ArrayList;

public class Main {

    public static void main (String[] args) {

        NameTable names = new NameTable();
        //      IdentInfo valueii = names.put("Value");
        //      eval.value = valueii.hashedname();
        //      IdentInfo sliceii = names.put("Slice");
        //      eval.slice = sliceii.hashedname();
        //      IdentInfo treenodeii = names.put("TreeNode");
        //      eval.treenode = treenodeii.hashedname();
        //      IdentInfo networkhelperii = names.put("NetworkHelper");
        //      eval.networkhelper = networkhelperii.hashedname();
        // Can put names in name table before creating a TokenList object
        TokenList.names = names;

        String infile, outfile;
        if (args.length != 2)
            System.out.println("Usage: Main <inputfile> <outputfile>");
        infile = args[0];
        outfile = args[1];
        CharStream cs;
        try { cs = new CharStream(FileToBytes.getBytes(infile), infile); }
        catch (IOException ioe) {
            System.out.println("Unable to input program from file "+infile);
            return;
        } 
        // Token category is given on first line, as a comment
        // (so that tokenizer will ignore it)
        String line0 = cs.getline(0);
        int syntaxcat = 0; // 0=expr, 1=stmt, 2=compunit
        if (line0.equals("/*expr*/")) syntaxcat = 0;
        else if (line0.equals("/*stmt*/")) syntaxcat = 1;
        else if (line0.equals("/*compunit*/")) syntaxcat = 2;
        TokenList L;
        try {
            L = new Lexer(cs).lexer();
        }
        catch (LexError le) {
            showLexError(le, infile);
            return;
        }
        Parser p = new Parser(L, cs);
        CST t = null;
        AST a = null;
        try {
            switch (syntaxcat) {
            case 0: t = p.parseexpression(); break;
            case 1: t = p.parsestatementList(); break;
            case 2: t = p.parsecompilationUnit(); break;
            }
            System.out.println("Main.java:\n"+t);
            a = CSTtoAST.translate(t);
        }
        catch (SyntaxError se) {
            showparseerror(se, cs, infile);
            return;
        }

        try {     
            FileOutputStream output = new FileOutputStream(outfile);
            PrintStream file = new PrintStream(output);
            System.out.println(a);
            String genCode = Codegen.codegen(a);
            file.println(genCode);
        }
        catch(Exception e)
        {
            System.out.println("Main.java: could not load file!");
            e.printStackTrace();
        }

    }

    public static void showLexError(LexError le, String file) {
        String msg = le.msg;
        CharStream cs = le.cs;
        char ch = le.ch;
        StringBuffer sb = le.sb;

        String errmsg = "Lex error: " + msg;
        errmsg += "\nat line " + cs.linenum + ", column "
                + cs.getcolumnof(cs.cursor)
                + ", character " + ch + " (ascii " + (int)ch + "), "
                + file;
        if (sb.length() > 0) {
            int startpos = cs.cursor - sb.length();
            int line = cs.getlineof(startpos),
                    col = cs.getcolumnof(startpos);
            if (line == cs.linenum)
                errmsg += "\n(token started at column "+ col+")";
            else
                errmsg += "\n(token started at line " + line
                + ", column "+ col+")";
        }
        System.out.println(errmsg);
    }

    public static void showparseerror (SyntaxError se,
            CharStream cs, String file)
    {
        int startpos = se.startpos;
        String nonterm = se.nonterm;
        Token prevtoken = se.prevtoken;
        System.out.print("Syntax error while parsing " + nonterm
                + " starting at line " + cs.getlineof(startpos)
                + ", column "+ cs.getcolumnof(startpos));
        if (se.msg == null)
            System.out.println(".");
        else
            System.out.println(": " + se.msg);
        if (prevtoken != null) {
            int loc = prevtoken.endloc;
            int thisline = cs.getlineof(loc),
                    thiscol = cs.getcolumnof(loc);
            System.out.println("Current location is line " + thisline
                    + ", column " + thiscol + ":");
            String line = cs.getline(thisline);
            line = line.substring(0, thiscol) + "#"
                    + line.substring(thiscol, line.length()-1);
            System.out.println(line);
        }
    }

}
