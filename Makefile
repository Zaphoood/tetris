cc=g++ -I./libs/SDL2/include
sdl2_flags=$(shell sdl2-config --libs)
src_dir=./src
build_dir=./build

out=main
srcs=main.cpp playfield.cpp
objs=main.o playfield.o

all: $(out)

$(out): $(patsubst %, build/%, $(objs))
	$(cc) -o $(out) $(patsubst %, build/%, $(objs)) $(sdl2_flags)

$(patsubst %, build/%, $(objs)): build/%.o: $(src_dir)/%.cpp
	$(cc) -c $< -o $@

clean:
	rm -f build/*.o

.phony:
	all
	clean
