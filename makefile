build: 
		gcc *.c -o pinpon `sdl2-config --cflags` `sdl2-config --libs`
