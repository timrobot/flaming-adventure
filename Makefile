sdl_example: sdl_test.cpp
	g++ sdl_test.cpp -o sdl_test -lSDL

clean:
	rm sdl_test
