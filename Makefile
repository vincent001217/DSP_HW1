.PHONY: all clean

CFLAGS+= -std=c99
LDFLAGS+=-lm     # link to math library

TARGET=test_hmm train test

all: $(TARGET)
# type make/make all to compile test_hmm

clean:
	$(RM) $(TARGET)   # type make clean to remove the compiled file
