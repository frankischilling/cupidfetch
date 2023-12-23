CUPID_LIBS=-Ilibs
CUPID_DEV=-Wall -pedantic

all: clean cupidfetch

cupidfetch: src/*.c libs/ini.c
	$(CC) -o cupidfetch $^ $(CFLAGS) $(LDFLAGS) $(CUPID_LIBS) $(LIBS) $(LD_LIBS)

dev: src/*.c libs/ini.c
	$(CC) -o cupidfetch $^ $(CUPID_DEV) $(CFLAGS) $(LDFLAGS) $(CUPID_LIBS) $(LIBS) $(LD_LIBS)

asan: src/*.c libs/ini.c
	$(CC) -o cupidfetch $^ -fsanitize=address $(CUPID_DEV) $(CFLAGS) $(LDFLAGS) $(CUPID_LIBS) $(LIBS) $(LD_LIBS)

ubsan: src/*.c libs/ini.c
	$(CC) -o cupidfetch $^ -fsanitize=undefined $(CUPID_DEV) $(CFLAGS) $(LDFLAGS) $(CUPID_LIBS) $(LIBS) $(LD_LIBS)

clean:
	rm -f cupidfetch *.o



