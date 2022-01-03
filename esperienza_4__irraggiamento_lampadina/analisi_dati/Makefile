
CXX=g++
CFLAGS=-g -Wall

LIB=-I/usr/local/include -L/usr/local/lib/ -lErrorAnalysis -lLabTools
RLIB=`root-config --glibs --cflags`

BIN=analisi_MOD2

all: $(BIN)
	./$(BIN)

$(BIN): $(BIN).cpp
	$(CXX) $(CFLAGS) $< -o $@ $(RLIB) $(LIB)

clean: 
	$(RM) -rf $(BIN) *.dSYM
