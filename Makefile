#######################################
### CORE
#######################################
BIN 			:= main
LIBRARIES 		:= raylib

MAKEFLAGS 		:= --no-print-directory
LD_FLAGS 		:= -lpthread #-fsanitize=address

BIN_DIR 		:= bin
OBJ_DIR 		:= build
SRC_DIR 		:= src
LIB_DIR 		:= lib
INC_DIR 		:= include

BIN_EXT 		:= 
SRC_EXT 		:= .cpp .c


#######################################
### DEFAULTS
#######################################
BUILD 			:= debug
# Options: unix web windows
PLATFORM 		:= unix
FATAL 			:= false


#######################################
### HOST OS
#######################################
# Linux is default
CC 				:= clang
CXX 			:= clang++
C_FLAGS 		:= -MMD -MP
CXX_FLAGS 		:= -std=c++20 $(C_FLAGS)
# -MMD 				provides dependency information (header files) for make in .d files 
# -pg 				ADD FOR gprof analysis TO BOTH COMPILE AND LINK COMMAND!!
# -MJ 				CLANG ONLY: compile-database
# -MP 				Multi-threaded compilation
# -Wfatal-errors 	Stop at first error

USR_DIR 		:= /usr

ifdef TERMUX_VERSION
CXX_FLAGS 		+= -DTERMUX
USR_DIR 		:= $(PREFIX)
ifeq ($(PLATFORM),unix)
LIBRARIES 		+= log
endif
endif


#######################################
### PROJECT SPECIFIC
#######################################
### RAYLIB
ASSETS_DIR 				:= assets
RAYLIB_SRC_DIR 			:= $(USR_DIR)/local/lib/raylib/src
INC_DIRS_SYS 			+= $(RAYLIB_SRC_DIR)

ifeq ($(PLATFORM),windows)
LIB_DIRS 			+= $(RAYLIB_SRC_DIR)
else
ifeq ($(PLATFORM),web)
ifeq ($(BUILD),debug)
LIB_DIRS 	+= $(RAYLIB_SRC_DIR)/lib/web/debug
else
LIB_DIRS 	+= $(RAYLIB_SRC_DIR)/lib/web/release
endif
else
ifeq ($(BUILD),debug)
LIB_DIRS 	+= $(RAYLIB_SRC_DIR)/lib/desktop/debug
else
LIB_DIRS 	+= $(RAYLIB_SRC_DIR)/lib/desktop/release
endif
endif
endif

ifeq ($(PLATFORM),web)
# LD_FLAGS 			+= --preload-file $(ASSETS_DIR)/ -sUSE_GLFW=3
ifeq ($(BUILD),debug)
LD_FLAGS 			+= --shell-file $(RAYLIB_SRC_DIR)/shell.html
else
LD_FLAGS 			+= --shell-file $(RAYLIB_SRC_DIR)/minshell.html
endif
endif


#######################################
### BUILD CONFIGS
#######################################
ifeq ($(BUILD),debug)
CXX_FLAGS 		+= -g -ggdb -O0 -Wall -Wextra -Wshadow -Werror -Wpedantic -pedantic-errors -DDEBUG
endif

ifeq ($(BUILD),release)
CXX_FLAGS 		+= -O2 -DNDEBUG
endif

ifeq ($(FATAL),true)
CXX_FLAGS 		+= -Wfatal-errors
endif


#######################################
### TARGET PLATFORM
#######################################
### WEB/EMSCRIPTEN
ifeq ($(PLATFORM),web)
CC 				:= emcc
C_FLAGS			+= -Os -DEMSCRIPTEN -DPLATFORM_WEB
CXX 			:= em++
CXX_FLAGS		+= -Os -DEMSCRIPTEN -DPLATFORM_WEB
BIN_EXT			:= .html
USR_DIR			:= 
endif

### WINDOWS
ifeq ($(PLATFORM),windows)
CC 				:= x86_64-w64-mingw32-gcc
CXX 			:= x86_64-w64-mingw32-g++
BIN_EXT			:= .exe
USR_DIR			:= $(USR_DIR)/x86_64-w64-mingw32
LD_FLAGS 		+= -static -static-libgcc -static-libstdc++
endif

#######################################
### Automatic variables
#######################################
SRCS 			:= $(foreach e,\
					$(shell find $(SRC_DIR) -type f),\
					$(filter $(addprefix %,$(SRC_EXT)),\
						$e))
OBJS 			:= $(SRCS:%=$(OBJ_DIR)/$(BUILD)/$(PLATFORM)/%.o)
DEPS 			:= $(OBJS:.o=.d)

USR_DIRS 		:= $(USR_DIR) $(patsubst %,%/local,$(USR_DIR))
LIB_DIRS 		+= $(patsubst %,%/lib,$(USR_DIRS))
LIB_DIRS 		+= $(shell find $(LIB_DIR) -type d)
INC_DIRS 		:= $(shell find $(SRC_DIR) $(INC_DIR) -type d)
# INC_DIRS_SYS	+= $(LIB_DIRS)

INC_FLAGS 		:= $(addprefix -I,$(INC_DIRS))
INC_FLAGS 		+= $(addprefix -isystem,$(INC_DIRS_SYS))
LIB_FLAGS 		:= $(addprefix -L,$(LIB_DIRS))
LD_FLAGS 		+= $(addprefix -l,$(LIBRARIES))


#######################################
### Targets
#######################################
.PHONY: all analyze build cdb clean debug host init publish release run web windows

all: debug release

analyze:
	@mkdir -p $(OBJ_DIR)/cppcheck
	@cppcheck \
		--quiet \
		--enable=all \
		--suppress=missingIncludeSystem \
		--suppress=missingInclude \
		--suppress=selfAssignment \
		--suppress=cstyleCast \
		--suppress=unmatchedSuppression \
		--inconclusive \
		--check-level=exhaustive \
		--error-exitcode=1 \
		--cppcheck-build-dir=$(OBJ_DIR)/cppcheck \
		--template=gcc \
		-I include/ \
		-I src/ \
		src/

build: $(BIN_DIR)/$(BUILD)/$(PLATFORM)/$(BIN)$(BIN_EXT)

cdb:
	$(info )
	$(info === Build compile_commands.json ===)
	@compiledb -n make

clean:
	$(info )
	$(info === CLEAN ===)
	rm -r $(OBJ_DIR)/* $(BIN_DIR)/*

debug: cdb
	@$(MAKE) BUILD=debug build
	@$(MAKE) analyze

host:
	http-server -o . -c-1

init:
	mkdir -p assets include lib src
	touch src/main.cpp

publish: 
	@$(MAKE) debug release web windows -j

release: 
	@$(MAKE) BUILD=release build

run:
	$(BIN_DIR)/$(BUILD)/$(PLATFORM)/$(BIN)$(BIN_EXT)

web:
	@$(MAKE) BUILD=release PLATFORM=web build

windows:
	@$(MAKE) BUILD=release PLATFORM=windows build


#######################################
### Rules
#######################################
# === COMPILER COMMAND ===
# cpp files
$(OBJ_DIR)/$(BUILD)/$(PLATFORM)/%.cpp.o : %.cpp
	$(info )
	$(info === Compile: PLATFORM=$(PLATFORM), BUILD=$(BUILD) ===)
	@mkdir -p $(dir $@)
	$(CXX) -o $@ -c $< $(CXX_FLAGS) $(INC_FLAGS)

# c files
$(OBJ_DIR)/$(BUILD)/$(PLATFORM)/%.c.o : %.c
	$(info )
	$(info === Compile: PLATFORM=$(PLATFORM), BUILD=$(BUILD) ===)
	@mkdir -p $(dir $@)
	$(CC) -o $@ -c $< $(C_FLAGS) $(INC_FLAGS)


# === LINKER COMMAND ===
# unix bin
$(BIN_DIR)/$(BUILD)/$(PLATFORM)/$(BIN)$(BIN_EXT): $(OBJS)
	$(info )
	$(info === Link: PLATFORM=$(PLATFORM), BUILD=$(BUILD) ===)
	@mkdir -p $(dir $@)
	$(CXX) -o $@ $^ $(CXX_FLAGS) $(LIB_FLAGS) $(LD_FLAGS) 


### "-" surpresses error for initial missing .d files
-include $(DEPS)

info:
	$(info )
	$(info === INFO ===)
	@echo BIN : $(BIN)
	@echo SRC_DIR : $(SRC_DIR)
	@echo INC_DIR : $(INC_DIR)
	@echo LIB_DIR : $(LIB_DIR)
	@echo OBJ_DIR : $(OBJ_DIR)
	@echo BIN_DIR : $(BIN_DIR)
	@echo SRC_EXT : $(SRC_EXT)
	@echo INC_EXT : $(INC_EXT)
	@echo LIB_EXT : $(LIB_EXT)
	@echo BIN_EXT : $(BIN_EXT)
	@echo BUILD : $(BUILD)
	@echo PLATFORM : $(PLATFORM)
	@echo CXX : $(CXX)
	@echo MAKEFLAGS : $(MAKEFLAGS)
	@echo CXX_FLAGS : $(CXX_FLAGS)
	@echo SRCS : $(SRCS)
	@echo OBJS : $(OBJS)
	@echo DEPS : $(DEPS)
	@echo USR_DIR : $(USR_DIR)
	@echo USR_DIRS : $(USR_DIRS)
	@echo INC_DIRS : $(INC_DIRS)
	@echo INC_DIRS_SYS : $(INC_DIRS_SYS)
	@echo LIB_DIRS : $(LIB_DIRS)
	@echo INC_FLAGS : $(INC_FLAGS)
	@echo LIB_FLAGS : $(LIB_FLAGS)
	@echo LD_FLAGS : $(LD_FLAGS)
	@echo ASSETS_DIR : $(ASSETS_DIR)
