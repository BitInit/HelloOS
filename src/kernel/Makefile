CFLAGS = -c -I../include -I../include \
	-I./include \
	--static -fno-pie -fno-builtin -fno-stack-protector -m64
KERNLE_OBJS = kernel.o mm.o

all: clean build

build: $(KERNLE_OBJS)

%.o: %.c
	@gcc $(CFLAGS) -o $@ $<

clean: 
	@rm -rf *.o