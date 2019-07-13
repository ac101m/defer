# Output executables
DEFERRED_EXEC ?= bin/deferred
IMMEDIATE_EXEC ?= bin/immediate

# Directory controls
OBJ_DIR ?= build
SRC_DIR ?= src

# Compiler configuration
CXX = g++
INC_FLAGS := -Iinclude -Isrc
BASE_FLAGS ?= -MMD -MP -m64 -std=c++11 -Wall
FLAGS ?= $(INC_FLAGS) $(BASE_FLAGS) $(LD_FLAGS) -g
LD_FLAGS ?= -loptparse -lgltools -lglfw -lGLEW -lGL

# Sources which define main functions
MAIN_SRCS := $(shell find $(SRC_DIR) -maxdepth 1 -name *.cpp)
MAIN_OBJS := $(MAIN_SRCS:%=$(OBJ_DIR_RELEASE)/%.o)
MAIN_DEPS := $(MAIN_OBJS:.o=.d) $(MAIN_OBJS:.o=.d)

# "Subordinate" sources which do not define mains
SUB_SRCS := $(shell find $(SRC_DIR) -mindepth 2 -name *.cpp)
SUB_OBJS := $(SUB_SRCS:%=$(OBJ_DIR_RELEASE)/%.o)
SUB_DEPS := $(SUB_OBJS:.o=.d) $(SUB_OBJS:.o=.d)

# C++ object compilation
$(OBJ_DIR)/%.cpp.o: %.cpp
	@$(MKDIR_P) $(dir $@)
	$(CXX) $(FLAGS) -c $< -o $@

# Release build target
DEFERRED_OBJS := $(SUB_OBJS) $(OBJ_DIR)/src/deferred.cpp.o
deferred: move_shaders $(DEFERRED_OBJS)
	@$(MKDIR_P) $(dir $(DEFERRED_EXEC))
	$(CXX) $(DEFERRED_OBJS) -o $(DEFERRED_EXEC) $(LD_FLAGS)

# Debug build target
IMMEDIATE_OBJS := $(SUB_OBJS) $(OBJ_DIR)/src/immediate.cpp.o
immediate: move_shaders $(IMMEDIATE_OBJS)
	@$(MKDIR_P) $(dir $(IMMEDIATE_EXEC))
	$(CXX) $(IMMEDIATE_OBJS) -o $(IMMEDIATE_EXEC) $(LD_FLAGS)

# Simple target, collect glsl files in the shaders folder
GLSL_SRCS := $(shell find $(SRC_DIR) -name *.glsl)
SHADER_BIN_DIR := bin/shaders
move_shaders:
	@$(MKDIR_P) $(SHADER_BIN_DIR)
	cp $(GLSL_SRCS) $(SHADER_BIN_DIR)

# Make all targets
all: deferred immediate

# Clean, be careful with this
.PHONY: clean
clean:
	@$(RM) -rv $(OBJ_DIR)

# Include dependencies
-include $(SUB_DEPS) $(MAIN_DEPS)

# Make directory
MKDIR_P ?= mkdir -p
