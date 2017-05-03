CLANG=clang++
OPT_LEVEL=-O0
CXXFLAGS=`llvm-config --cxxflags`
LDFLAGS=`llvm-config --ldflags`
BUILD_DIR=build
LIB_TARGET=$(BUILD_DIR)/rtpg4llvm.a
HEADERS=$(wildcard *.h)
SOURCE=$(wildcard *.cpp)
BARE_OBJS:=$(SOURCE:.cpp=.o)
OBJS:=$(foreach obj, $(BARE_OBJS), $(BUILD_DIR)/$(obj))

all: $(BUILD_DIR) $(OBJS)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

clean:
	rm -f $(OBJS)
	rmdir $(BUILD_DIR)

$(BUILD_DIR)/%.o: %.cpp $(HEADERS)
	$(CLANG) -g $(OPT_LEVEL) -c $(CXXFLAGS) $< -o $@
