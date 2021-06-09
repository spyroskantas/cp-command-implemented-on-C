all: fcp

quic: fcp.c
	gcc fcp.c copyFunctions.c removeFunctions.c minsc.c -o fcp
clean:
	rm -f fcp *.o
