CXX = g++
CXXFLAGS = -O3 -std=c++17 -Wall -Wextra -DNDEBUG
INCLUDES = -Isrc
DEPFLAGS = -MMD -MP
SRCDIR = src
TESTDIR = test
TMPDIR = tmp
TARGET    := engine
NATIVE       := -march=native

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

ifeq ($(build), debug)
    CXXFLAGS := -g3 -O3  $(INCLUDES) -std=c++17 -Wall -Wextra -pedantic
endif

ifeq ($(build), release)
    CXXFLAGS := -O3 -std=c++17  $(INCLUDES) -Wall -Wextra -pedantic -DNDEBUG
    LDFLAGS  := -lpthread -static -static-libgcc -static-libstdc++ -Wl,--no-as-needed
    NATIVE   := -march=x86-64-avx2
endif

# Different native flag for macOS
ifeq ($(uname_S), Darwin)
    NATIVE =
    LDFLAGS =
endif

ALL_FILES := $(call rwildcard,$(SRCDIR)/,*.cpp)
SRC_FILES := $(filter-out $(SRCDIR)/main.cpp, $(ALL_FILES))

SRC_DIRECTORIES := $(shell find $(SRCDIR) -type d)
TMP_DIRS := $(addprefix $(TMPDIR)/,$(SRC_DIRECTORIES))

OBJECTS   := $(patsubst %.cpp,$(TMPDIR)/%.o,$(SRC_FILES))
DEPENDS   := $(patsubst %.cpp,$(TMPDIR)/%.d,$(SRC_FILES))

.PHONY: clean all tests FORCE

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(NATIVE) -MMD -MP -o $@ $^ $(LDFLAGS)

$(TMPDIR)/%.o: %.cpp | $(TMPDIR)
	$(CXX) $(CXXFLAGS) $(NATIVE) -MMD -MP -c $< -o $@ $(LDFLAGS)

$(TMPDIR):
	$(MKDIR) $(TMP_DIRS)

clean:
	rm -rf $(TMPDIR)

-include $(DEPENDS)