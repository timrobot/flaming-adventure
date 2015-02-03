intro = intro-hello intro-sendrecv

all:
	mpicc intro-hello.c -o intro-hello
	mpicc intro-sendrecv.c -o intro-sendrecv
	mpicc intro-sort.c -o intro-sort

run:
	mpiexec -n 16 ./intro-sendrecv
