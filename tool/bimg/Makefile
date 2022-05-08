CFLAGS = -I../../arch/x86/include -g
BIMG ?= bimg

all: clean build

build:
	@gcc $(CFLAGS) -o $(BIMG) svimg.c main.c

clean: 
	@rm -rf *.o $(BIMG)
