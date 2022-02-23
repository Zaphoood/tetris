CXX=g++
sdl2_flags=$(shell sdl2-config --libs)
CXXFLAGS=-I./libs/SDL2/include -std=c++17 $(sdl2_flags)
src_dir=./src
build_dir=./build

bin=main
srcs=main.cpp playfield.cpp
objs=main.o playfield.o

all: $(patsubst %, build/%, $(objs))
	$(CXX) $(CXXFLAGS) -o $(bin) $(patsubst %, build/%, $(objs))

$(patsubst %, build/%, $(objs)): build/%.o: $(src_dir)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f build/*.o
	rm $(bin)

.phony: all clean
