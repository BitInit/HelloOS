BIN = bimg
CFLAGS = -I../../arch/include -g

all : clean build

build :
	@gcc $(CFLAGS) -o $(BIN) svimg.c main.c

clean : 
	@rm -rf *.o $(BIN)
