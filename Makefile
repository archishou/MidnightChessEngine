EXE           = midnight
 
SOURCES      := src/main.cpp src/move_generation/position.cpp src/move_generation/tables.cpp src/move_generation/types.cpp src/move_search/tables/transposition_table.cpp src/utils/helpers.cpp src/utils/clock.cpp src/move_search/tables/history_table.cpp src/move_search/tables/lmr_table.cpp src/move_search/move_ordering/move_ordering.cpp src/engine.cpp src/move_search/reductions.cpp src/move_search/search.cpp src/move_search/types.cpp src/move_search/pvs.cpp
 
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
 
$(EXE) : $(SOURCES)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $(OUT) $(SOURCES)
 
clean: