hb.out : hilbert.o hbDriver.o
	g++ -g hilbert.o hbDriver.o -o hb.out -lGL -lGLU -lglut

hilbert.o : hilbert.cpp hilbert.h
	g++ -g -c hilbert.cpp

hbDriver.o : hbDriver.cpp hilbert.h
	g++ -g -c hbDriver.cpp

clean :
	rm -f hb.out hilbert.o hbDriver.o
