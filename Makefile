CXX = g++
CXXFLAGS = -O3 -std=c++17 -Wall -Wextra -DNDEBUG
INCLUDES = -Isrc
DEPFLAGS = -MMD -MP
SRCDIR = src
TESTDIR = tests
TMPDIR = tmp
TARGET := midnight

# recursive wildcard
rwildcard=$(wildcard $1$2) $(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2))

#windows find, assumes no spaces in dir names
define wfind
$(foreach d,$1,$(wildcard $d**/) $(call wfind,$(wildcard $d**/)))
endef


# Detect Windows
ifeq ($(OS), Windows_NT)
    MKDIR    := mkdir
    uname_S  := Windows
    SUFFIX   := .exe
    SRC_DIRECTORIES := $(call wfind,$(SRCDIR)/)
	SRC_DIRECTORIES += $(call wfind,$(TESTDIR)/)
else
ifeq ($(COMP), MINGW)
    MKDIR    := mkdir
    uname_S  := Windows
    SUFFIX   := .exe
    SRC_DIRECTORIES := $(call wfind,$(SRCDIR)/)
    SRC_DIRECTORIES += $(call wfind,$(TESTDIR)/)
else
    MKDIR   := mkdir -p
    LDFLAGS := -pthread
    uname_S := $(shell uname -s)
    SUFFIX  :=
    SRC_DIRECTORIES := $(shell find $(SRCDIR) -type d)
    SRC_DIRECTORIES += $(shell find $(TESTDIR) -type d)
endif
endif

CXXFLAGS := -O3 -std=c++17  $(INCLUDES) -Wall -Wextra -pedantic -DNDEBUG
LDFLAGS  := -flto

SRC_FILES := $(call rwildcard,$(SRCDIR)/,*.cpp)

ifeq ($(MAKECMDGOALS),tests)
	SRC_FILES := $(call rwildcard,$(SRCDIR)/,*.cpp)
	SRC_FILES += $(call rwildcard,$(TESTDIR)/,*.cpp)
	SRC_FILES := $(filter-out $(SRCDIR)/main.cpp, $(SRC_FILES))
	TARGET    := midnight-tests
endif


TMP_DIRS := $(addprefix $(TMPDIR)/,$(SRC_DIRECTORIES))

OBJECTS   := $(patsubst %.cpp,$(TMPDIR)/%.o,$(SRC_FILES))
DEPENDS   := $(patsubst %.cpp,$(TMPDIR)/%.d,$(SRC_FILES))

.PHONY: clean all tests FORCE

all: $(TARGET)
tests: $(TARGET)
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -MMD -MP -o $@ $^ $(LDFLAGS)

$(TMPDIR)/%.o: %.cpp | $(TMPDIR)
	$(CXX) $(CXXFLAGS) -MMD -MP -c $< -o $@ $(LDFLAGS)

$(TMPDIR):
	$(MKDIR) $(TMP_DIRS)

clean:
	rm -rf $(TMPDIR)

-include $(DEPENDS)