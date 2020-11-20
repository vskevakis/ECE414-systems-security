demo: simple-crypto.c simple-crypto.o demo.c demo.o simple-crypto.o
	gcc simple-crypto.c -c
	gcc demo.c -c
	gcc simple-crypto.o demo.o -o demo

demo-2: simple-crypto.c simple-crypto.o demo-2.c demo-2.o simple-crypto.o
	gcc simple-crypto.c -c
	gcc demo-2.c -c
	gcc simple-crypto.o demo-2.o -o demo-2

clean:
	rm -f simple-crypto.o demo.o demo-2.o ./demo ./demo-2