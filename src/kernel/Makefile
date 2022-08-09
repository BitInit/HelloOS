CFLAGS = -c -I../include -I./mm\
	-I./include \
	--static -mcmodel=large -fno-pie -fno-builtin -fno-stack-protector -m64
KERNLE_OBJS = kernel.o mm/mm.o

all: clean build

build: $(KERNLE_OBJS)

%.o: %.c
	@gcc $(CFLAGS) -o $@ $<

clean: 
	@rm -rf *.o mm/*.o