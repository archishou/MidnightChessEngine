CXX = g++
CXXFLAGS = -O3 -std=c++17 -Wall -Wextra -DNDEBUG
INCLUDES = -Isrc
DEPFLAGS = -MMD -MP

SRCDIR = src
TESTDIR = tests

TMP_SRC_ROOT_DIR = tmp-src
TMP_TESTS_ROOT_DIR = tmp-tests

SRC_TARGET := midnight
TEST_TARGET := midnight-tests

# recursive wildcard
rwildcard=$(wildcard $1$2) $(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2))

# Detect Windows
ifeq ($(OS), Windows_NT)
    MKDIR    := mkdir
    uname_S  := Windows
    SUFFIX   := .exe
else
ifeq ($(COMP), MINGW)
    MKDIR    := mkdir
    uname_S  := Windows
    SUFFIX   := .exe
else
    MKDIR   := mkdir -p
    LDFLAGS := -pthread
    uname_S := $(shell uname -s)
    SUFFIX  :=
endif
endif

CXXFLAGS := -O3 -std=c++17  $(INCLUDES) -Wall
LDFLAGS  := -flto

SRC_DIRECTORIES := $(shell find $(SRCDIR) -type d)
TMP_SRC_DIRS := $(addprefix $(TMP_SRC_ROOT_DIR)/,$(SRC_DIRECTORIES))

OBJECTS_SRC   := $(patsubst %.cpp,$(TMP_SRC_ROOT_DIR)/%.o,$(SRC_FILES))
DEPENDS_SRC   := $(patsubst %.cpp,$(TMP_SRC_ROOT_DIR)/%.d,$(SRC_FILES))

.PHONY: clean all tests FORCE

all: $(SRC_TARGET)

$(SRC_TARGET): $(OBJECTS_SRC)
	$(CXX) $(CXXFLAGS) -MMD -MP -o $@ $^ $(LDFLAGS)

$(TMP_SRC_ROOT_DIR)/%.o: %.cpp | $(TMP_SRC_ROOT_DIR)
	$(CXX) $(CXXFLAGS) -MMD -MP -c $< -o $@ $(LDFLAGS)

$(TMP_SRC_ROOT_DIR):
	$(MKDIR) $(TMP_SRC_DIRS)

clean:
	rm -rf $(TMP_SRC_DIRS)

-include $(DEPENDS_SRC)