
build: header.o

header.o: header.S
	$(HIDE)$(CC) -E  $(CFLAGS) $< > header.i
	$(HIDE)$(AS) $(ASFLAGS) -o $@ header.i
