TARGET=arithmetic_test
all:$(TARGET)

SRC=$(shell find . -name *.cc)
OBJ=$(SRC:./%.cc=%.o)

INC = \
	  -I../

LIB = \
	  ../libarithmetic.a

$(TARGET): $(OBJ)
	make -C ../
	g++ -o $@ $^ $(LIB)

.cc.o:
	g++ -c $^ $(INC) -fPIC -g

.PHONY: clean
clean:
	make clean -C ../
	rm -f $(TARGET) *.o
