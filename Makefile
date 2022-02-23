CXX=g++
# Linker flags need to be separated from the CXXFLAGS since
# they need to be at the end of the command
LINK_FLAGS=$(shell sdl2-config --libs)
CXXFLAGS=-I./libs/SDL2/include -std=c++17
src_dir=./src
build_dir=./build

bin=main
srcs=main.cpp playfield.cpp
headers=playfield.h
objs=main.o playfield.o

all: $(patsubst %, build/%, $(objs))
	$(CXX) $(CXXFLAGS) -o $(bin) $(patsubst %, build/%, $(objs)) $(LINK_FLAGS)

$(patsubst %, build/%, $(objs)): build/%.o: $(src_dir)/%.cpp $(src_dir)/$(headers)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f build/*.o
	rm $(bin)

.phony: all clean
