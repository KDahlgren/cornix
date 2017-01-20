#IDIR =../include
CXX=g++
#CXXFLAGS=-I$(IDIR)
CXXFLAGS=-I/afs/cats.ucsc.edu/users/j/kmdahlgr/software/rocksdb/build/include/

#ODIR=obj
#LDIR =../lib

LFLAGS = -L/afs/cats.ucsc.edu/users/j/kmdahlgr/software/rocksdb/build/lib
LIBS=-lrocksdb

#_DEPS = hellomake.h
#DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

#_OBJ = hellomake.o hellofunc.o 
#OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

#$(ODIR)/%.o: %.c $(DEPS)
#	$(CC) -c -o $@ $< $(CFLAGS)


all: $(OBJ)
	g++ -o $@ $^ $(CXXFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~
