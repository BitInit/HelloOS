
head_64.o: head_64.S
	$(HIDE)$(CC) -E  $(CFLAGS) $< > head_64.i
	$(HIDE)$(AS) $(ASFLAGS) -o $@ head_64.i