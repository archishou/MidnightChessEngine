# compiler
CC := g++
# linker flags
LDFLAGS := -flto
# source files
SRC_DIR := src
SRCS := $(wildcard $(SRC_DIR)/**/*.cpp) $(wildcard $(SRC_DIR)/*.cpp) main.cpp
# compiler flags
CFLAGS := -std=c++17 -Wall -Wextra -pedantic -O3 -I$(SRC_DIR)
# object files
OBJ_DIR := obj/**/
OBJS := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRCS))
# executable
EXEC := my_program

# test files
TEST_DIR := tests
TEST_SRCS := $(wildcard $(TEST_DIR)/**/*.cpp) $(wildcard $(TEST_DIR)/*.cpp)
TEST_OBJS := $(patsubst $(TEST_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(TEST_SRCS))
TEST_EXEC := test

# build rules
$(EXEC): $(OBJS)
	$(CC) $(LDFLAGS) $^ -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(TEST_EXEC): $(OBJS) $(TEST_OBJS)
	$(CC) $(LDFLAGS) $^ -o $@

$(OBJ_DIR)/%.o: $(TEST_DIR)/%.cpp | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

.PHONY: all
all: $(EXEC) $(TEST_EXEC)

.PHONY: release
release: $(EXEC)

.PHONY: tests
tests: $(TEST_EXEC)
	./$(TEST_EXEC)

.PHONY: clean
clean:
	rm -rf $(OBJ_DIR) $(EXEC) $(TEST_EXEC)
