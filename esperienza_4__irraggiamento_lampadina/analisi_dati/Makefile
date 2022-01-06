
CXX=g++
CFLAGS=-g -Wall

LIB=-I/usr/local/include -L/usr/local/lib/ -lErrorAnalysis -lLabTools
RLIB=`root-config --glibs --cflags`

BINS=analisi_MOD2

all: $(BINS)
	./$(BINS)

%: %.cpp
	$(CXX) $(CFLAGS) $^ -o $@ $(RLIB) $(LIB)

clean: 
	$(RM) -rf $(BINS) *.dSYM
