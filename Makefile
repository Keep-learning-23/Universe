target = ./bin/Universe
depent = ./build/main.o ./build/sphere.o ./build/torus.o ./build/model.o \
./build/utils.o
library = -lglfw3 -Wl,--copy-dt-needed-entries -lGLEW -lsoil2-debug 
CC = g++
$(target) : $(depent)
	$(CC) $(depent) $(library) -o $(target)
./build/%.o : ./src/%.cpp
	$(CC) -c $< -o $@
clean:
	rm $(depent) $(target)