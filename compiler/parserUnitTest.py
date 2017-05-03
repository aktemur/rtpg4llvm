import os
import glob

def testCase(file):
    print "Testing ", file
    #os.system("cat "+file)
    os.system("java Main "+file+" tests/fragmentout.java > /dev/null")
    os.system("diff tests/fragmentout.java "+file+".out")
    os.system("rm tests/fragmentout.java")
    return

def testAll():
    for infile in glob.glob(os.path.join('tests/', 'in*.java') ):
        testCase(infile)    
    return

def main():
    testAll()
    return

if __name__ == "__main__":
    main()
