
CC = g++
CPP = g++ -E
CXXFLAGS = -g -Wall

SOURCES = main.cpp utils.cpp request.cpp response.cpp signals.cpp
OBJS = $(SOURCES:.cpp=.o)


dd:$(OBJS)
	$(CC) -o $@ $^

clean:
	rm *.o

.depend:
	$(CPP) -MM $(SOURCES) > .depend

depend:
	rm -f .depend
	$(MAKE) .depend

include .depend
