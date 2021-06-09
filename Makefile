all: quic

quic: quic.c
	gcc quic.c copyFunctions.c removeFunctions.c minsc.c -o quic
clean:
	rm -f quic *.o
