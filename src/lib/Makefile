CFLAGS = -c -I../include --static -fno-pie -fno-builtin -fno-stack-protector -m64
OBJS = printk.o string.o

all: clean build

build: $(OBJS)

%.o: %.c
	@gcc $(CFLAGS) -o $@ $<

clean: 
	@rm -rf *.o