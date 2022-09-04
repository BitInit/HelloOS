SOURCE := $(shell find . -name "*.c")
OBJS := $(SOURCE:%.c=%.o)
CFLAGS = -c -I../include --static \
	 -mcmodel=large -fno-pie -fno-builtin \
	 -fno-stack-protector -m64

build: $(OBJS)

$(OBJS):%.o:%.c
	gcc $(CFLAGS) -o $@ $<

clean:
	rm -rf $(OBJS)