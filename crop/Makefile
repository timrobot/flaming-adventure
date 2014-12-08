CCFLAGS = `pkg-config --cflags opencv`
LIBS = `pkg-config --libs opencv` -lSDL

all: crop

crop: crop.o
	g++ $(CCFLAGS) -o $@ $< $(LIBS)

crop.o: crop.cpp
	g++ $(CCFLAGS) -o $@ -c $<

clean:
	rm -f *o crop
