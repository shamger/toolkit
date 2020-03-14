TARGET=libarithmetic.a
all:$(TARGET)

SRC=$(shell find . -name *.cc)
OBJ=$(SRC:./%.cc=%.o)

$(TARGET): $(OBJ)
	ar crv $@ $^

.PHONY: clean
clean:
	rm -f *.a *.o
