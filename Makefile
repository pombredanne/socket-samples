%.o: %.c socket.h
	$(CC) -c -o $@ $< $(CFLAGS)

single: socket.o single.o
	$(CC) -o $@ $^ $(CFLAGS)
