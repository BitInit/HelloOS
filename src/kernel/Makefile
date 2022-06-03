CFLAGS = -c -I../lib/include --static -fno-pie -fno-builtin -fno-stack-protector -m64
OBJS = setup.o

all: clean build

build: $(OBJS)

%.o: %.c
	@gcc $(CFLAGS) -o $@ $<

clean: 
	@rm -rf *.o