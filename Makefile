CXX = g++
INCLUDES = -Isrc
DEPFLAGS = -MMD -MP
SRCDIR = src
TESTDIR = tests
TMPDIR = tmp
EXE = midnight

LDFLAGS  := -flto
# recursive wildcard
rwildcard=$(wildcard $1$2) $(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2))

#windows find, assumes no spaces in dir names
define wfind
$(foreach d,$1,$(wildcard $d**/) $(call wfind,$(wildcard $d**/)))
endef

# Detect Windows
ifeq ($(OS), Windows_NT)
    MKDIR    := mkdir
    CMD_SEP  := &
    uname_S  := Windows
    SUFFIX   := .exe
    SRC_DIRECTORIES := $(call wfind,$(SRCDIR)/)
	SRC_DIRECTORIES += $(call wfind,$(TESTDIR)/)
	SRC_DIRECTORIES := $(subst /,\,$(SRC_DIRECTORIES))
	TMP_DIRS := $(addprefix $(TMPDIR)\,$(SRC_DIRECTORIES))
	LDFLAGS  += -fuse-ld=lld-link
else
ifeq ($(COMP), MINGW)
    MKDIR    := mkdir -p
    CMD_SEP  := &
    uname_S  := Windows
    SUFFIX   := .exe
    SRC_DIRECTORIES := $(call wfind,$(SRCDIR)/)
    SRC_DIRECTORIES += $(call wfind,$(TESTDIR)/)
	SRC_DIRECTORIES := $(subst /,\,$(SRC_DIRECTORIES))
    TMP_DIRS := $(addprefix $(TMPDIR)\,$(SRC_DIRECTORIES))
else
    MKDIR    := mkdir -p
    CMD_SEP  := ;
    uname_S  := $(shell uname -s)
    SUFFIX   :=
    SRC_DIRECTORIES := $(shell find $(SRCDIR) -type d)
    SRC_DIRECTORIES += $(shell find $(TESTDIR) -type d)
    TMP_DIRS := $(addprefix $(TMPDIR)/,$(SRC_DIRECTORIES))
endif
endif

CXXFLAGS := -O3 -std=c++20 -march=native $(INCLUDES) -Wall -Wextra -pedantic -DNDEBUG

SRC_FILES := $(call rwildcard,$(SRCDIR)/,*.cpp)

ifeq ($(MAKECMDGOALS),tests)
	SRC_FILES := $(call rwildcard,$(SRCDIR)/,*.cpp)
	SRC_FILES += $(call rwildcard,$(TESTDIR)/,*.cpp)
	SRC_FILES := $(filter-out $(SRCDIR)/main.cpp, $(SRC_FILES))
	EXE    = midnight-tests
endif

TARGET = $(addsuffix $(SUFFIX),$(EXE))

OBJECTS   := $(patsubst %.cpp,$(TMPDIR)/%.o,$(SRC_FILES))
DEPENDS   := $(patsubst %.cpp,$(TMPDIR)/%.d,$(SRC_FILES))

.PHONY: clean all tests FORCE

all: $(EXE)
tests: $(EXE)
$(EXE): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -MMD -MP -o $(TARGET) $^ $(LDFLAGS)

$(TMPDIR)/%.o: %.cpp | $(TMPDIR)
	$(CXX) $(CXXFLAGS) -MMD -MP -c $< -o $@ $(LDFLAGS)

# dumb workaround for mysterious issue with msys2
$(TMPDIR):
	$(foreach dir,$(TMP_DIRS),$(MKDIR) $(dir) ${CMD_SEP})

clean:
	rm -rf $(TMPDIR)

-include $(DEPENDS)
