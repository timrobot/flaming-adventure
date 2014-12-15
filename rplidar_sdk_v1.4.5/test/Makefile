RPLIDAR_SDK = $(shell pwd)/rplidar_sdk/
INCLUDE = -I$(RPLIDAR_SDK)
LIBS = -L$(RPLIDAR_SDK)

all:
	g++ test.cpp -o test $(LIBS) -lrplidar_sdk $(INCLUDE)
