OBJS = myPLDA.o Doc.o
DEPS = myPLDA.h Doc.h 
VPATH = src
CC = g++
DEBUG = -g
CFLAGS = -fopenmp -Wall -c $(DEBUG)
LFLAGS = -fopenmp -lgsl -Wall $(DEBUG)

%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)
%.o: %.cc $(DEPS)	
	$(CC) -c -o $@ $< $(CFLAGS)
myPLDA : $(OBJS)
	$(CC) $(LFLAGS) $(OBJS) -o myPLDA
clean:
	\rm *.o *~ myPLDA

