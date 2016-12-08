APPLE2_CL	:= $(CC65_HOME)/bin/cl65
APPLE2_CC	:= $(CC65_HOME)/bin/cc65
APPLE2_SRC	:= src/file_io.c src/mli.asm src/main.c
APPLE2_MAP  := hires.map
APPLE2_CFLAGS	:= -Oirs -v -t apple2
APPLE2_OUT	:= bin/loader.a2

all:	directories apple2

directories:
	    mkdir -p bin

apple2: $(APPLE2_SRC)
		$(APPLE2_CL) -m $(APPLE2_MAP) -o $(APPLE2_OUT) $? $(APPLE2_CFLAGS)

clean:	$(SRC)
	rm -f $(APPLE2_MAP) src/*.o src/*.s & rm -r bin/
