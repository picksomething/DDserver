
CC = g++ -g -Wall
CPP = g++ -E

SOURCES = main.cpp utils.cpp request.cpp #response.cpp
OBJS = $(SOURCES:.c=.o)


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
