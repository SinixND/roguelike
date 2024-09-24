#######################################
### Makefile structure
# Jump-table for sections
#######################################

# LBL_Info

# LBL_Makeflags

# LBL_EnvironmentVariables
# LBL_ProjectDirectories
# LBL_FileExtensions

# LBL_BinaryFiles
# LBL_SourceFiles
# LBL_ObjectFiles
# LBL_DependencyFiles
# LBL_OtherDirectories_ProjectSpecific

# LBL_Libraries
# LBL_LibraryDirectories
# LBL_LibraryDirectories_ProjectSpecific
# LBL_LibraryFlags

# LBL_IncludeDirectories
# LBL_IncludeDirectories_ProjectSpecific
# LBL_IncludeFlags

# LBL_Toolchain
# LBL_CompileFlags
# LBL_LinkFlags

# LBL_Rules
# LBL_BuildRules


#######################################
### Information
# LBL_Info
#######################################

# Makefile to build arbitrary projects that adhere to the following requirements:
# - All source files are in ./src or its subfolders
# - All header files are in ./include or ./src or their subfolders
# Host OS: linux or termux
# Target platforms: linux (default), web (with emscripten) or windows (with mingw32)


#######################################
### Makeflags
# LBL_Makeflags
#######################################

### Automatically added flags to make command
MAKEFLAGS 				:= --no-print-directory #-j


#######################################
### Environment variables
# LBL_EnvironmentVariables
#######################################

### Default host OS
OS 						?= linux

ifdef TERMUX_VERSION
    OS 					:= termux
endif

### Default build mode (debug/release)
BUILD_MODE 				?= debug

### Default target platform (linux/web/windows)
PLATFORM 				?= linux


# LBL_ProjectDirectories
### Here the (latest) binary file will be output
BIN_DIR_ROOT 			:= ./bin

### Here go all source files (with the $(SRC_EXT) extension) and project specific header files
SRC_DIR 				:= ./src

### Here go all (project-)independet header files
INC_DIR 				:= ./include

### Here go all (project-)independet header files
LIB_DIR 				:= ./lib

### Here the object files will be outputted
BUILD_DIR_ROOT 			:= ./build

### Define folder for web content to export
WEB_DIR 				:= ./web

### Define folder for resource files
RESOURCE_DIR 			:= ./resources


# LBL_FileExtensions
### Set the targets file extension
ifeq ($(PLATFORM),linux)
    BIN_EXT 			:=
endif
ifeq ($(PLATFORM),windows)
    BIN_EXT 			:= .exe
endif
ifeq ($(PLATFORM),web)
    BIN_EXT				:= .html
endif

### Set the used file extension for source files, usually either .c or .cpp
SRC_EXT 				:= .cpp

### Set the used file extension for object files, usually .o
### Distinction to be made for use of '-MJ $@.json' flag in linux debug builds (clang)
OBJ_EXT_LIN 			:= .o
OBJ_EXT_WIN				:= .obj

OBJ_EXT					:= $(OBJ_EXT_LIN)

ifeq ($(PLATFORM),windows)
    OBJ_EXT 			:= $(OBJ_EXT_WIN)
endif

### Set the used file extension for dependency files, usually .d (header/source connection)
DEP_EXT 				:= .d

ifeq ($(PLATFORM),windows)
    DEP_EXT 			:= .dep
endif



# LBL_BinaryFiles
### Name that the created binary should have
BIN 					:= main

### Here the debug/release binaries will be output
    BIN_DIR 			= $(BIN_DIR_ROOT)/$(BUILD_MODE)

ifeq ($(PLATFORM),web)
    BIN_DIR 			= $(WEB_DIR)
endif


# LBL_SourceFiles
### Get subdirs in ./src
SRC_DIRS 				= $(shell find . -wholename "*$(SRC_DIR)*" -type d)

### List all source files found in source file directory w/ path from ./;
SRCS 					:= $(shell find $(SRC_DIR) -wholename "*$(SRC_EXT)" -type f)
#
### List all source files found in source file directory w/o path;
SRC_FILES 					= $(notdir $(SRCS))

### Strip file extensions to get a list of sourcefile labels
### (patsubst pattern,replacement,target)
SRC_NAMES 				= $(patsubst %$(SRC_EXT),%,$(SRC_FILES))


# LBL_ObjectFiles
ifeq ($(BUILD_MODE),debug)
    BUILD_DIR 			= $(BUILD_DIR_ROOT)/debug
else
    BUILD_DIR 			= $(BUILD_DIR_ROOT)/release
endif

### Make list of object files need for linker command by changing ending of all source files to .o;
### IMPORTANT for linker dependency, so they are found as compile rule
OBJS 					= $(patsubst %,$(BUILD_DIR)/%$(OBJ_EXT),$(SRC_NAMES))


# LBL_DependencyFiles
### Make list of dependency files
DEPS 					= $(patsubst $(BUILD_DIR)/%$(OBJ_EXT),$(BUILD_DIR)/%$(DEP_EXT),$(OBJS))


# LBL_OtherDirectories
### Define /usr directory
ifeq ($(OS),termux)
    USR					:= $(PREFIX)
else
    USR 				:= /usr
endif
ifeq ($(PLATFORM),windows)
    USR					:= /usr/x86_64-w64-mingw32
endif

# LBL_OtherDirectories_ProjectSpecific
RAYLIB_SRC_DIR 			= $(USR)/lib/raylib/src


#######################################
### Libraries
# LBL_Libraries
#######################################

### Label used libraries so the respective -l flags (eg. -lraylib)
LIBRARIES 				:= raylib
ifeq ($(PLATFORM),windows)
    LIBRARIES 			+= opengl32 gdi32 winmm
endif
ifeq ($(OS),termux)
    LIBRARIES 			+= #log
endif


# LBL_LibraryDirectories
### Here go library files
LIB_DIRS 				= $(shell find . -wholename "*$(LIB_DIR)*" -type d)

### Set the locations of system libraries
ifeq ($(PLATFORM),web)
else
    LIB_DIRS 			+= $(USR)/lib

    ifeq ($(OS),linux)
        LIB_DIRS 		+= $(USR)/local/lib
    endif
endif



# LBL_LibraryDirectories_ProjectSpecific
ifeq ($(PLATFORM),windows)
    LIB_DIRS 			+= $(RAYLIB_SRC_DIR)
else
    ifeq ($(BUILD_MODE),debug)
        ifeq ($(PLATFORM),web)
            LIB_DIRS 	+= $(RAYLIB_SRC_DIR)/debug/web
        else
            LIB_DIRS 	+= $(RAYLIB_SRC_DIR)/debug
        endif
    else
        ifeq ($(PLATFORM),web)
            LIB_DIRS 	+= $(RAYLIB_SRC_DIR)/release/web
        else
            LIB_DIRS 	+= $(RAYLIB_SRC_DIR)/release
        endif
    endif
endif


# LBL_LibraryFlags
### Make library flags by prefixing every provided path with -L; this might take a while for the first time, but will NOT be repeated every time
LIB_FLAGS 				:= $(addprefix -L,$(LIB_DIRS))


#######################################
### Includes
# LBL_IncludeDirectories
#######################################

### Recursively add project include folder
INC_DIRS 				:= $(shell find . -wholename "*$(INC_DIR)*" -type d)

### Recursively add project src folder
INC_DIRS 				+= $(SRC_DIRS)

### Get the locations of system header files; ignore for emscripten
ifeq ($(PLATFORM),web)
	### Emscripten must not use system includes
    INC_DIRS			+=
else
    INC_DIRS 			+= $(USR)/include
    
    ifeq ($(OS),linux)
        INC_DIRS 		+= $(USR)/local/include
    endif
endif


# LBL_IncludeDirectories_ProjectSpecific
SYS_INC_DIRS 			+= $(RAYLIB_SRC_DIR)


# LBL_IncludeFlags
### Make include flags by prefixing every provided path
INC_FLAGS 				= $(addprefix -I,$(INC_DIRS))
INC_FLAGS 				+= $(addprefix -isystem,$(SYS_INC_DIRS))


#######################################
### Toolchain
# LBL_Toolchain
#######################################

ifeq ($(PLATFORM),linux)
    CXX 				:= clang++
endif
ifeq ($(PLATFORM),windows)
    CXX 				:= /bin/x86_64-w64-mingw32-g++
endif
ifeq ($(PLATFORM),web)
    CXX 				:= em++
endif


# LBL_CompileFlags
### Set compile flags
# -MMD 				provides dependency information (header files) for make in .d files 
# -pg 				ADD FOR gprof analysis TO BOTH COMPILE AND LINK COMMAND!!
# -MJ 				CLANG ONLY: compile-database
# -MP 				Multi-threaded compilation
# -Wfatal-errors 	Stop at first error

### Default flags
CXX_FLAGS 				:= -std=c++20 -MMD -MP

### Build specific flags 
ifeq ($(OS),termux)
    CXX_FLAGS 			+= -DTERMUX
endif
ifeq ($(PLATFORM),web)
    CXX_FLAGS 			+= -Os -Wall -DEMSCRIPTEN -DPLATFORM_WEB
else
    ifeq ($(BUILD_MODE),debug)
        CXX_FLAGS 			+= -g -ggdb -O0 -Wall -Wextra -Wshadow -Werror -Wpedantic -pedantic-errors -Wfatal-errors -DDEBUG -MJ $@.json

        ifeq ($(OS),linux)
            CXX_FLAGS 		+= -pg
        endif
    else
        CXX_FLAGS 			+= -O2 -DNDEBUG
    endif
endif


# LBL_LinkFlags
### Set link flags
LD_FLAGS 				:= -lpthread #-fsanitize=address

ifeq ($(PLATFORM),web)
    LD_FLAGS 			+= --preload-file resources/ -sUSE_GLFW=3
    ifeq ($(BUILD_MODE),debug)
        LD_FLAGS 			+= --shell-file $(RAYLIB_SRC_DIR)/shell.html
    else
        LD_FLAGS 			+= --shell-file $(RAYLIB_SRC_DIR)/minshell.html
    endif
endif
ifeq ($(PLATFORM),windows)
    LD_FLAGS 			+= -static -static-libgcc -static-libstdc++
endif

### Make link flags by prefixing every provided library with -l (should work for most libraries due to convention); probably pkg-config makes duplicates...
LD_FLAGS 				+= $(addprefix -l,$(LIBRARIES))


#######################################
### Rules
# LBL_Rules
#######################################

### Set VPATH as std dir to find sources in subdirs
# Should not be needed: https://www.cmcrossroads.com/article/basics-vpath-and-vpath
# VPATH 					:= $(shell find ./$(SRC_DIR) -type d):$(shell find ./$(INC_DIR) -type d)#:$(shell find ./$(BUILD_DIR) -type d)#:$(shell find . -type d)
# better:
vpath %$(SRC_EXT) $(SRC_DIRS)

### Non-file (.phony)targets (aka. rules)
.PHONY: all build clean debug dtb init publish release run web windows all_rules

### Default rule by convention
all: debug release

all_rules: 
	@$(MAKE) init 
	@$(MAKE) debug 
	@$(MAKE) release 
	@$(MAKE) web 
	@$(MAKE) windows 
	@$(MAKE) publish 
	@$(MAKE) run

### Build binary with current config
build: $(BIN_DIR)/$(BIN)$(BIN_EXT)
	@cp $(BIN_DIR)/$(BIN)$(BIN_EXT) $(BIN_DIR_ROOT)

### Clear dynamically created directories
clean:
	$(info )
	$(info === Clean ===)
	@rm -rf $(BIN_DIR_ROOT)/debug/*
	@rm -rf $(BIN_DIR_ROOT)/release/*
	@rm -rf $(BUILD_DIR_ROOT)/debug/*
	@rm -rf $(BUILD_DIR_ROOT)/release/*
	@rm -rf $(WEB_DIR)/*

### Debug build
debug: 
	$(info )
	$(info === Debug build ===)
	@$(MAKE) BUILD_MODE=debug build
	@$(MAKE) -s dtb

### Build compile_commands.json
dtb: 
	$(shell sed -e '1s/^/[\n/' -e '$$s/,$$/\n]/' $(BUILD_DIR)/*.o.json > compile_commands.json)

### Create necessary folders
init:
	$(info )
	$(info === Init ===)
	@mkdir -p $(BIN_DIR_ROOT)/debug
	@mkdir -p $(BIN_DIR_ROOT)/release
	@mkdir -p $(BUILD_DIR_ROOT)/debug
	@mkdir -p $(BUILD_DIR_ROOT)/release
	@mkdir -p $(WEB_DIR)

### Rule for complete compilation, ready to publish
publish: clean
	$(info )
	$(info === Publish ===)
	@$(MAKE) -j release web windows

### Rule for release build process with binary as prerequisite
release: 
	$(info )
	$(info === Release build ===)
	@$(MAKE) BUILD_MODE=release build

### Run binary file after building
run: 
	$(BIN_DIR_ROOT)/$(BIN)$(BIN_EXT)

### Rule for web build process
web: 
	$(info )
	$(info === Web build ===)
	@$(MAKE) BUILD_MODE=release PLATFORM=web build

### Rule for windows build process
windows: 
	$(info )
	$(info === Windows build ===)
	@$(MAKE) BUILD_MODE=release PLATFORM=windows build


# LBL_BuildRules
### $@ (target, left of ":")
### $< (first dependency, first right of ":")
### $^ (all dependencies, all right of ":")

# === COMPILER COMMAND ===
### MAKE object files FROM source files; "%" pattern-matches (need pair of)
$(BUILD_DIR)/%$(OBJ_EXT_LIN): %$(SRC_EXT) 
	$(info )
	$(info === Compile: BUILD_MODE=$(BUILD_MODE), PLATFORM=$(PLATFORM) ===)
	$(CXX) -o $@ -c $< $(CXX_FLAGS) $(INC_FLAGS) -MJ $@.json 

$(BUILD_DIR)/%$(OBJ_EXT_WIN): %$(SRC_EXT) 
	$(info )
	$(info === Compile: BUILD_MODE=$(BUILD_MODE), PLATFORM=$(PLATFORM) ===)
	$(CXX) -o $@ -c $< $(CXX_FLAGS) $(INC_FLAGS)

# === LINKER COMMAND ===
### MAKE binary file FROM object files
$(BIN_DIR_ROOT)/$(BUILD_MODE)/$(BIN)$(BIN_EXT): $(OBJS)
	$(info )
	$(info === Link: BUILD_MODE=$(BUILD_MODE), PLATFORM=$(PLATFORM) ===)
	$(CXX) -o $@ $^ $(CXX_FLAGS) $(LIB_FLAGS) $(LD_FLAGS)

# === WEB BUILD COMMAND ===
### MAKE html file FROM source files
$(WEB_DIR)/$(BIN)$(BIN_EXT): $(SRS) 
	$(info )
	$(info === Build: BUILD_MODE=$(BUILD_MODE), PLATFORM=$(PLATFORM) ===)
	$(CXX) -o $(BIN_DIR)/$(BIN)$(BIN_EXT) $(SRCS) $(CXX_FLAGS) $(INC_FLAGS) $(LIB_FLAGS) $(LD_FLAGS)

### "-" surpresses error for initial missing .d files
-include $(DEPS)
