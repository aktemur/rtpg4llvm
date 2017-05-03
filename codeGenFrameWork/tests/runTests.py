import os
import glob
import sys

LIBS = "`llvm-config --libs core engine native jit ipo instrumentation bitreader`"

def testCase(file):
    print "Testing ", file, "  "
    os.system("rm *.java")
    os.system("export CLASSPATH=$CLASSPATH:../../compiler/ ; "+
              "python ../../compiler/preprocess.py "+file+" > /dev/null")
    compileCmd = "clang++ -O0 "+LIBS+" `llvm-config --ldflags` `llvm-config --cppflags` -o "+file+".out  ../build/*.o "+file+"-pp.cpp"
    #print compileCmd
    os.system(compileCmd)      
    runCmd = "./" + file + ".out"
    #print runCmd
    if(os.system(runCmd) != 0):
        print "  !!! Problem executing the .out file."
    return

def testAll():
    for infile in glob.glob('*.cpp'):
      testCase(infile[:-4]) # get rid of the extension '.cpp' at the end    
    return

def main():
    os.system("rm *-pp.cpp *.out ../main.o")
    if (len(sys.argv) > 1):
      for infile in sys.argv[1:]:
        testCase(infile)
    else:
        testAll()
    #os.system("rm *.dSYM")
    return

if __name__ == "__main__":
    main()
