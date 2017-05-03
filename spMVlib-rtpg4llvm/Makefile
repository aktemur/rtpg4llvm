RTPG4LLVM_DIR=../codeGenFrameWork
PARSER_DIR=../compiler
JAVACLASSPATH=$(PARSER_DIR)
CLANG=clang++
OPT_LEVEL=-O0 -g
CXXFLAGS=`llvm-config --cxxflags` -I`llvm-config --src-root` -I`llvm-config --obj-root`/lib/Target/X86/
LDFLAGS=`llvm-config --ldflags`
LIBS=`llvm-config --libs all`
BUILD_DIR=.
TARGET=spMVgen
HEADERS=$(wildcard *.h)
SOURCE=$(filter-out $(wildcard *-pp.cpp), $(wildcard *.cpp))
PP_SOURCE:=$(SOURCE:.cpp=-pp.cpp)
BARE_OBJS:=$(PP_SOURCE:.cpp=.o)
OBJS:=$(foreach obj, $(BARE_OBJS), $(BUILD_DIR)/$(obj))

all: $(TARGET)

pp: $(PP_SOURCE)

$(TARGET): $(OBJS) $(RTPG4LLVM_DIR)/build/*.o
	$(CLANG) $(RTPG4LLVM_DIR)/build/*.o $(OBJS) -o $(TARGET) $(LDFLAGS) $(LIBS) 

clean:
	rm -f $(OBJS) $(TARGET) $(PP_SOURCE) *.java

%-pp.cpp: %.cpp $(HEADERS)
	export CLASSPATH=$(JAVACLASSPATH); python $(PARSER_DIR)/preprocess.py $* > /dev/null

$(BUILD_DIR)/%.o: %.cpp
	$(CLANG) $(OPT_LEVEL) -c -I$(RTPG4LLVM_DIR) $(CXXFLAGS) $< -o $@

