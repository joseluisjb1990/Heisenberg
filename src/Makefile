CXX = g++

EXE = ww

CDEBUG = -g -Wall

CXXDEBUG = -g -Wall

CXXFLAGS = -O0  -std=c++11 $(CXXDEBUG)

CXXOBJ = bear bear_driver
SOBJ =  parser lexer

FILES = $(addsuffix .cc, $(CXXOBJ))

OBJS  = $(addsuffix .o, $(CXXOBJ))

CLEANLIST =  $(addsuffix .o, $(OBJ)) $(OBJS) \
			 bear_parser.tab.cc bear_parser.tab.hh \
			 location.hh position.hh \
			 stack.hh parser.o bear_parser.output\
			 lexer.o lexer.yy.cc $(EXE)\

.PHONY: all
all: bear

bear: $(FILES)
	$(MAKE) $(SOBJ)
	$(MAKE) $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(EXE) $(OBJS) parser.o lexer.o expresion.cc statement.cc definition.cc type.cc TablaSimbolos.cc Contenido.cc program.cc generator.cc quad.cc block.cc FlowGraph.cc $(LIBS)


parser: bear_parser.yy
	bison -d -v bear_parser.yy
	$(CXX) $(CXXFLAGS) -c -o parser.o bear_parser.tab.cc

lexer: bear_scanner.ll
	flex --outfile=lexer.yy.cc  $<
	$(CXX)  $(CXXFLAGS) -c lexer.yy.cc -o lexer.o


.PHONY: clean
clean:
	rm -rf $(CLEANLIST)
