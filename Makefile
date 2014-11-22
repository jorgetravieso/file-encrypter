all:	mycipher

mycipher: mycipher.cpp
	g++ -Wall $< -o $@

clean:
	rm -f mycipher *.o *~ core

