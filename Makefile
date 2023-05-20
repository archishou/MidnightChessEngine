EXE           = midnight
 
SOURCES      := $(wildcard src/*.cpp) $(wildcard src/*/*.cpp) $(wildcard src/*/*/*.cpp)
SOURCES 	 := $(filter-out src/tuner.cpp, $(SOURCES))

ifeq ($(MAKECMDGOALS),tune)
	SOURCES += $(wildcard texel-tuner/*.cpp)
	SOURCES := $(filter-out src/main.cpp, $(SOURCES))
	EXE = midnight-tune
endif

ifeq ($(MAKECMDGOALS),tests)
	SOURCES += $(wildcard tests/*.cpp) $(wildcard tests/*/*.cpp)
	SOURCES := $(filter-out src/main.cpp, $(SOURCES))
	EXE = midnight-tests
endif

CXXFLAGS     := -O3 -Isrc -flto -std=c++20 -march=native -Wall -Wextra -pedantic -DNDEBUG
LDFLAGS      :=
 
CXX          := clang++
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

# Change const-expr limit to generate magic tables at compile time.
CONSTEXPR_LIMIT := 900000000
ifeq ($(CXX),clang++)
	CXXFLAGS += -fconstexpr-steps=$(CONSTEXPR_LIMIT)
else
	CXXFLAGS += -fconstexpr-ops-limit=$(CONSTEXPR_LIMIT)
endif

ifneq (,$(findstring clang,$(shell $(CXX) --version)))
    ifneq ($(DETECTED_OS),Darwin)
        LDFLAGS += -fuse-ld=lld
    endif
endif
 
OUT := $(EXE)$(SUFFIX)
 
all: $(EXE)
tests: $(EXE)
tune: $(EXE)
$(EXE) : $(SOURCES)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $(OUT) $(SOURCES)
 
clean: