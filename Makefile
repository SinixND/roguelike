### Basic funcionality of makefile:
### $(BINARY) depends on $(OBJS) to be build/linked
### Make looks for a rule to build $(OBJS)
### @ (as a prefix to a cli command): suppress cli output; use make -n to debug commands

### Label used libraries so the respective -l flags (eg. -lraylib)
LIBRARIES 				:= raylib
WEB_LIBRARIES			:= $(LIBRARIES)
WIN_LIBRARIES 			:= $(LIBRARIES) opengl32 gdi32 winmm

ifdef TERMUX_VERSION
LIBRARIES 				+= #log
else
LIBRARIES 				+= gtest benchmark
endif

### Set library directories as needed
RAYLIB_DIR 				:= /usr/lib/raylib/src
EMSCRIPTEN_DIR			:= /usr/lib/emscripten/cache/sysroot/include
WIN_RAYLIB_DIR 			:= /usr/x86_64-w64-mingw32/lib/raylib/src
ifdef TERMUX_VERSION
RAYLIB_DIR 				:= $(PREFIX)/lib/raylib/src
EMSCRIPTEN_DIR 			:= $(PREFIX)/lib/emscripten/cache/sysroot/include
endif

### Set compile flags
# -MMD			provides dependency information (header files) for make in .d files 
# -pg			ADD FOR gprof analysis TO BOTH COMPILE AND LINK COMMAND!!
# -MJ			clang only: compile-database
# -MP			Multi-threaded compilation

### Default flags
CXX_FLAGS				:= -std=c++20 -MMD -MP
CFLAGS 					:=

### Recipe specific flags 
debug: CXX_FLAGS 		+= -g -ggdb -Wall -Wextra -Wshadow -Werror -Wpedantic -pedantic-errors -O0 #-Wfatal-errors
debug: CFLAGS			+= -DDEBUG
ifndef TERMUX_VERSION
debug: CXX_FLAGS 		+= -pg
endif

release: CXX_FLAGS 		+= -O2
release: CFLAGS			+= -DNDEBUG

web: CXX_FLAGS			+= -Os -Wall 
web: CFLAGS				+= --preload-file resources/ --shell-file $(RAYLIB_DIR)/minshell.html -sUSE_GLFW=3 -DEMSCRIPTEN -DPLATFORM_WEB

windows: CFLAGS			+= -static -static-libgcc -static-libstdc++

benchmark: CXX_FLAGS 	+= -O2
benchmark: CFLAGS		+= -DNDEBUG

### Set the projects label, used for the binary (eg. main.exe, root .cpp file needs same name)
BINARY 					:= main

### Automatically added flags to make command
MAKEFLAGS 				:= -j --no-print-directory

#######################################
### Set the used compiler to g++ or clang++
CXX 					:= clang++
WIN_CXX 				:= /bin/x86_64-w64-mingw32-g++

### Set the binary file extension
BINARY_EXT 				:= exe
WIN_BINARY_EXT 			:= windows.exe

### Set the used file extension for c-files, usually either .c or .cpp
SRC_EXT 				:= cpp
### Set the used file extension for object files, usually .o
OBJ_EXT 				:= o
WIN_OBJ_EXT 			:= win.o
### Set the used file extension for dependency files, usually .d (header/source connection)
DEP_EXT 				:= d
WIN_DEP_EXT 			:= win.d

### Set the respective folders from structure
### Set VPATH as std dir to look for compile targets
VPATH 					:= $(shell find . -type d)
### Here go all source files (with the $(SRC_EXT) extension) and project specific header files
SRC_DIRS 				:= ./src
### Here go prject independend header files
LOC_INC_DIR 			:= ./include
### Here go external header files
EXT_INC_DIR 			:= ./include/external
### Here go library files
LOC_LIB_DIR 			:= ./lib
### Here the object files will be outputted
OBJ_DIR 				:= ./build
OBJ_DIR_DEBUG 			:= ./build/debug
OBJ_DIR_RELEASE 		:= ./build/release
### Here the binary file will be outputted
BIN_DIR 				:= ./bin
BIN_DIR_DEBUG 			:= ./bin/debug
BIN_DIR_RELEASE 		:= ./bin/release
### Define folder for test files
TEST_DIR 				:= ./test
### Define folder for web content to export
WEB_DIR 				:= ./web

### Set the locations of header files
SYS_INC_DIR 			:= /usr/local/include /usr/include
WIN_SYS_INC_DIR 		:= /usr/x86_64-w64-mingw32/include
ifdef TERMUX_VERSION
	SYS_INC_DIR := $(PREFIX)/include
endif
EXT_INC_DIRS 			:= $(shell find $(EXT_INC_DIR) -wholename "**" -type d)
LOC_INC_DIRS 			:= $(shell find . -wholename "*include*" -type d)
LOC_INC_DIRS 			+= $(shell find . -wholename "*src*" -type d)

### Set the locations of all possible libraries used
SYS_LIB_DIR 			:= /usr/local/lib /usr/lib
WIN_SYS_LIB_DIR 		:= /usr/x86_64-w64-mingw32/lib
ifdef TERMUX_VERSION
	SYS_LIB_DIR := $(PREFIX)/lib
endif
LOC_LIB_DIRS 			:= $(shell find $(LOC_LIB_DIR) -type d)


### Set linker flags
LD_FLAGS 				:= -lpthread #-fsanitize=address

#######################
### DONT EDIT BELOW ###
#######################

### Make linker flags by prefixing every provided library with -l (should work for most libraries due to convention); probably pkg-config makes duplicates...
LD_FLAGS 				+= $(addprefix -l,$(LIBRARIES))
WEB_LD_FLAGS 			:= $(addprefix -l,$(WEB_LIBRARIES))
WIN_LD_FLAGS 			:= $(addprefix -l,$(WIN_LIBRARIES))

### Make library flags by prefixing every provided path with -L; this might take a while for the first time, but will NOT be repeated every time
SYS_LIB_FLAGS 			:= $(addprefix -L,$(SYS_LIB_DIR))
SYS_LIB_FLAGS			+= $(addprefix -L,$(RAYLIB_DIR))
WEB_LIB_FLAGS			+= $(addprefix -L,$(RAYLIB_DIR)/web)
WIN_SYS_LIB_FLAGS 		:= $(addprefix -L,$(WIN_SYS_LIB_DIR))
LOC_LIB_FLAGS 			:= $(addprefix -L,$(LOC_LIB_DIRS))

LIB_FLAGS 				:= $(SYS_LIB_FLAGS) $(LOC_LIB_FLAGS)
WIN_LIB_FLAGS 			:= $(WIN_SYS_LIB_FLAGS) $(LOC_LIB_FLAGS)

### Make include flags by prefixing every provided path with -I
LOC_INC_FLAGS 			:= $(addprefix -I,$(LOC_INC_DIRS))
SYS_INC_FLAGS 			:= $(addprefix -I,$(SYS_INC_DIR))
WIN_SYS_INC_FLAGS		:= $(addprefix -I,$(WIN_SYS_INC_DIR))
EXT_INC_FLAGS 			:= $(addprefix -isystem,$(EXT_INC_DIRS))
SYS_INC_FLAGS 			+= $(addprefix -isystem,$(RAYLIB_DIR))
WEB_SYS_INC_FLAGS 		+= $(addprefix -isystem,$(RAYLIB_DIR))

INC_FLAGS 				:= $(SYS_INC_FLAGS) $(EXT_INC_FLAGS) $(LOC_INC_FLAGS)
WIN_INC_FLAGS 			:= $(WIN_SYS_INC_FLAGS) $(EXT_INC_FLAGS) $(LOC_INC_FLAGS)

### List all source files found in source file directory;
SRCS 					:= $(shell find $(SRC_DIRS) -wholename "*$(SRC_EXT)" -type f)
SRC_NAMES 				:= $(shell find $(SRC_DIRS) -wholename "*$(SRC_EXT)" -type f -printf "%f\n")
### Strip file extensions to get a list of sourcefile labels
SRC_NAMES 				:= $(patsubst %.$(SRC_EXT),%,$(SRC_NAMES))

### Make list of object files need for linker command by changing ending of all source files to .o;
### (patsubst pattern,replacement,target)
### IMPORTANT for linker dependency, so they are found as compile rule
OBJS_DEBUG 				:= $(patsubst %,$(OBJ_DIR_DEBUG)/%.$(OBJ_EXT),$(SRC_NAMES))
OBJS_RELEASE 			:= $(patsubst %,$(OBJ_DIR_RELEASE)/%.$(OBJ_EXT),$(SRC_NAMES))
WIN_OBJS 				:= $(patsubst %,$(OBJ_DIR_RELEASE)/%.$(WIN_OBJ_EXT),$(SRC_NAMES))
TEST_OBJS 				:= $(TEST_DIR)/test.$(OBJ_EXT) $(patsubst $(OBJ_DIR)/$(BINARY).o,,$(OBJS))
BM_OBJS 				:= $(TEST_DIR)/benchmark.$(OBJ_EXT) $(patsubst $(OBJ_DIR)/$(BINARY).o,,$(OBJS))
### Make list of dependency files
DEPS_DEBUG 				:= $(patsubst $(OBJ_DIR_DEBUG)/%.$(OBJ_EXT),$(OBJ_DIR_DEBUG)/%.$(DEP_EXT),$(OBJS))
DEPS_RELEASE 			:= $(patsubst $(OBJ_DIR_RELEASE)/%.$(OBJ_EXT),$(OBJ_DIR_RELEASE)/%.$(DEP_EXT),$(OBJS))
WIN_DEPS 				:= $(patsubst $(OBJ_DIR_RELEASE)/%.$(WIN_OBJ_EXT),$(OBJ_DIR_RELEASE)/%.$(WIN_DEP_EXT),$(WIN_OBJS))

### Non-file (.phony)targets (aka. rules)
.PHONY: all init clean debug release run_debug run_release web windows publish dtb
ifndef TERMUX_VERSION
.PHONY: test benchmark
endif


### Default rule by convention
all: debug release
ifndef TERMUX_VERSION
all: #test
endif


### Create necessary folders
init:
	@mkdir -p $(BIN_DIR_DEBUG)
	@mkdir -p $(BIN_DIR_RELEASE)
	@mkdir -p $(OBJ_DIR_DEBUG)
	@mkdir -p $(OBJ_DIR_RELEASE)
	@mkdir -p $(EXT_INC_DIR)
	@mkdir -p $(LOC_LIB_DIR)
	$(info )
	$(info === Initialized folders ===)


### Clear dynamically created directories
clean:
	@rm -rf $(OBJ_DIR)/*.o
	@rm -rf $(OBJ_DIR)/*.d
	@rm -rf $(OBJ_DIR_DEBUG)/*.o
	@rm -rf $(OBJ_DIR_DEBUG)/*.d
	@rm -rf $(OBJ_DIR_DEBUG)/*.json
	@rm -rf $(OBJ_DIR_RELEASE)/*.o
	@rm -rf $(OBJ_DIR_RELEASE)/*.d
	@rm -rf $(BIN_DIR)/*.exe
	@rm -rf $(BIN_DIR_DEBUG)/*.exe
	@rm -rf $(BIN_DIR_RELEASE)/*.exe
	$(info )
	$(info === Cleaned folders ===)


debug: $(BIN_DIR_DEBUG)/$(BINARY).$(BINARY_EXT)
	$(info )
	$(info === Debug build done ===)
	@$(MAKE) -s dtb


### Rule for release build process with binary as prerequisite
release: $(BIN_DIR_RELEASE)/$(BINARY).$(BINARY_EXT)
	$(info )
	$(info === Release build done ===)


ifndef TERMUX_VERSION
#build: $(TEST_DIR)/test.$(BINARY_EXT) $(TEST_DIR)/benchmark.$(BINARY_EXT)
endif


### Run binary file after building
run_debug: debug
	@$(BIN_DIR_DEBUG)/$(BINARY).$(BINARY_EXT)


run_release: release
	@$(BIN_DIR_RELEASE)/$(BINARY).$(BINARY_EXT)


### Rule for web build process
web:
	emcc -o web/$(BINARY).html $(SRCS) $(CXX_FLAGS) $(WEB_LIB_FLAGS) $(WEB_LD_FLAGS) $(LOC_INC_FLAGS) $(WEB_SYS_INC_FLAGS) $(CFLAGS)


### Rule for windows build process
windows: $(BIN_DIR_RELEASE)/$(BINARY).$(WIN_BINARY_EXT)
	$(info )
	$(info === Build for windows done ===)


### Rule for complete compilation, ready to publish
publish: clean
	$(MAKE) release web windows


dtb: 
	@$(shell sed -e '1s/^/[\n/' -e '$$s/,$$/\n]/' $(OBJ_DIR_DEBUG)/*.o.json > compile_commands.json)


### Exclude main object file to avoid multiple definitions of main
test: $(BIN_DIR)/test.$(BINARY_EXT)
	@$(BIN_DIR)/test.$(BINARY_EXT)


benchmark: $(BIN_DIR)/benchmark.$(BINARY_EXT)
	@$(BIN_DIR)/benchmark.$(BINARY_EXT)


# === COMPILER COMMANDS ===
### MAKE object files FROM source files; "%" pattern-matches (need pair of)
$(OBJ_DIR_DEBUG)/%.$(OBJ_EXT): %.$(SRC_EXT) 
### $< (first dependency, first right of ":")
### $@ (target, left of ":")
	$(info )
	$(info === Compile main ===)
#	$(CXX) -o $@ -c $< $(CXX_FLAGS) $(INC_FLAGS)
	$(CXX) -o $@ -c $< -MJ $@.json $(CXX_FLAGS) $(INC_FLAGS) $(CFLAGS)

$(OBJ_DIR_RELEASE)/%.$(OBJ_EXT): %.$(SRC_EXT) 
### $< (first dependency, first right of ":")
### $@ (target, left of ":")
	$(info )
	$(info === Compile main ===)
	$(CXX) -o $@ -c $< $(CXX_FLAGS) $(INC_FLAGS) $(CFLAGS)

### COMPILE WINDOWS
$(OBJ_DIR_RELEASE)/%.$(WIN_OBJ_EXT): %.$(SRC_EXT)
	$(info )
	$(info === Compile windows ===)
	$(WIN_CXX) -o $@ -c $< $(CXX_FLAGS) $(WIN_INC_FLAGS) -I$(RAYLIB_DIR) $(CFLAGS)

### COMPILE TEST
$(TEST_DIR)/test.$(OBJ_EXT): test.$(SRC_EXT)
	$(info )
	$(info === Compile test ===)
	$(CXX) -o $@ -c $< $(CXX_FLAGS) $(INC_FLAGS)

### COMPILE BENCHMARK
$(TEST_DIR)/benchmark.$(OBJ_EXT): benchmark.$(SRC_EXT)
	$(info )
	$(info === Compile benchmark ===)
	$(CXX) -o $@ -c $< $(CXX_FLAGS) $(INC_FLAGS)


# === LINKER COMMANDS ===
### MAKE binary file FROM object files
$(BIN_DIR_DEBUG)/$(BINARY).$(BINARY_EXT): $(OBJS_DEBUG)
### $@ (target, left of ":")
### $^ (all dependencies, all right of ":")
	$(info )
	$(info === Link main ===)
	$(CXX) -o $@ $^ $(CXX_FLAGS) $(LIB_FLAGS) $(LD_FLAGS)

$(BIN_DIR_RELEASE)/$(BINARY).$(BINARY_EXT): $(OBJS_RELEASE)
### $@ (target, left of ":")
### $^ (all dependencies, all right of ":")
	$(info )
	$(info === Link main ===)
	$(CXX) -o $@ $^ $(CXX_FLAGS) $(LIB_FLAGS) $(LD_FLAGS)

### LINK WINDOWS
$(BIN_DIR_RELEASE)/$(BINARY).$(WIN_BINARY_EXT): $(WIN_OBJS)
	$(info )
	$(info === Link windows ===)
	$(WIN_CXX) -o $@ $^ $(WIN_LD_FLAGS) $(WIN_LIB_FLAGS) -L$(WIN_RAYLIB_DIR) $(WIN_LD_FLAGS) $(CFLAGS)

### LINK TEST
$(BIN_DIR)/test.$(BINARY_EXT): $(TEST_OBJS)
	$(info )
	$(info === Link test ===)
	$(CXX) -o $@ $^ $(CXX_FLAGS) $(LIB_FLAGS) $(LD_FLAGS)

### LINK BENCHMARK
$(BIN_DIR)/benchmark.$(BINARY_EXT): $(BM_OBJS)
	$(info )
	$(info === Link benchmark ===)
	$(CXX) -o $@ $^ $(CXX_FLAGS) $(LIB_FLAGS) $(LD_FLAGS)


### "-" surpresses error for initial missing .d files
-include $(DEPS_DEBUG)
-include $(DEPS_RELEASE)
