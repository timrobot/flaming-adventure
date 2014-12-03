CFLAGS=-fPIC -g -Wall `pkg-config --cflags opencv`
LIBS = `pkg-config --libs opencv`
INCLUDE = -I/usr/local/include/libfreenect -I/usr/include/libusb-1.0
FREE_LIBS = -L/usr/local/lib -lfreenect -pthread
TARGET = test

all: $(TARGET)

$(TARGET): $(TARGET).o kinect.o
	$(CXX) $(INCLUDE) $(CFLAGS) $(TARGET).o kinect.o -o $(TARGET) $(LIBS) $(FREE_LIBS)

$(TARGET).o: $(TARGET).cpp
	$(CXX) -c $(CFLAGS) $(TARGET).cpp -o $(TARGET).o $(INCLUDE)

kinect.o: kinect.cpp
	$(CXX) -c $(CFLAGS) kinect.cpp -o kinect.o $(INCLUDE)

clean:
	rm -rf *.o $(TARGET)
