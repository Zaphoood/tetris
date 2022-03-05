CXX=g++
# Linker flags need to be separated from the CXXFLAGS since
# they need to be at the end of the command
LINK_FLAGS=$(shell sdl2-config --libs)
#CXXFLAGS=-I./libs/SDL2/include -std=c++17
CXXFLAGS=-std=c++17
src_dir=./src
build_dir=./build

bin := main.out
srcs := $(wildcard $(src_dir)/*.cpp)
objs := $(patsubst $(src_dir)/%.cpp,$(build_dir)/%.o,$(srcs))
headers := $(src_dir)/constants.h

all: $(objs)
	$(CXX) $(CXXFLAGS) -o $(bin) $^ $(LINK_FLAGS)

$(build_dir)/%.o: $(src_dir)/%.cpp $(headers)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f build/*.o
	rm $(bin)

.phony: all clean
