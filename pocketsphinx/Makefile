TARGET=test

all:
	gcc -o $(TARGET) test.c \
		-DMODELDIR=\"`pkg-config --variable=modeldir pocketsphinx`\" \
		`pkg-config --cflags --libs pocketsphinx sphinxbase`
clean:
	rm -f $(TARGET)
