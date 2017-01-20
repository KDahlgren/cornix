HDIR =../include
IDIR=/afs/cats.ucsc.edu/users/j/kmdahlgr/software/rocksdb/build/include
CXX=g++
CXXFLAGS=-std=c++11 -Wall -g -I$(IDIR) -I$(HDIR)
#CXXFLAGS=-std=c++11 -Wall -g -I$(IDIR)

ODIR=../obj
#LDIR =../lib

LFLAGS=-L/afs/cats.ucsc.edu/users/j/kmdahlgr/software/rocksdb/build/lib -L/afs/cats.ucsc.edu/users/j/kmdahlgr/software/bzip2/build/lib -L/afs/cats.ucsc.edu/users/j/kmdahlgr/software/snappy/build/lib
LIBS=-lrocksdb -lbz2 -lsnappy -lz -lpthread -lrt

_DEP = 
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_HEADERS = cornix_crud.h cornix_includes.h cornix_agg.h cornix_parser.h cornix_utils.h cornix_ops.h cornix_time.h cornix_booleans.h styx.h
HEADERS = $(patsubst %,$(HDIR)/%,$(_HEADERS))

_OBJ = cornix_driver.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

EXEC=driver

$(ODIR)/%.o: %.h $(HEADERS)
	$(CXX) -c -o $@ $< $(CXXFLAGS)

$(ODIR)/%.o: %.cpp $(DEPS)
	$(CXX) -c -o $@ $< $(CXXFLAGS)

$(EXEC): $(OBJ)
	g++ -o $@ $^ $(CXXFLAGS) $(LFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(EXEC) $(ODIR)/*.o *~ core $(IDIR)/*~ $(HDIR)/*~
