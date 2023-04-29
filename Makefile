EXE           = midnight
 
SOURCES      := $(wildcard src/*.cpp) $(wildcard src/*/*.cpp) $(wildcard src/*/*/*.cpp)

ifeq ($(MAKECMDGOALS),tests)
	SOURCES += $(wildcard tests/*.cpp) $(wildcard tests/*/*.cpp)
	SOURCES := $(filter-out src/main.cpp, $(SOURCES))
	EXE = midnight-tests
endif

CXXFLAGS     := -O3 -Isrc -flto -std=c++20 -march=native -Wall -Wextra -pedantic -DNDEBUG
LDFLAGS      :=
 
CXX          := g++
SUFFIX       :=
 
# Detect Windows
ifeq ($(OS), Windows_NT)
    DETECTED_OS := Windows
    SUFFIX   := .exe
    CXXFLAGS += -static
else
    DETECTED_OS := $(shell uname -s)
    CXXFLAGS += -pthread
endif

ifneq (,$(findstring clang,$(shell $(CXX) --version)))
    ifneq ($(DETECTED_OS),Darwin)
        LDFLAGS += -fuse-ld=lld
    endif
endif
 
OUT := $(EXE)$(SUFFIX)
 
all: $(EXE)
tests: $(EXE)
$(EXE) : $(SOURCES)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $(OUT) $(SOURCES)
 
clean: