# TODO: name this shit
all: bin

bin:
	gcc -o name `sdl2-config --libs` -lSDL2_ttf -lSDL2_gfx -lpthread `find . -iregex ".*\.c"`
