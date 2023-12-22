CUPID_LIBS=-Ilibs

all: clean cupidfetch

cupidfetch: main.c libs/ini.c
	$(CC) -o cupidfetch $^ $(CFLAGS) $(LDFLAGS) $(CUPID_LIBS) $(LIBS) $(LD_LIBS)

asan: main.c libs/ini.c
	$(CC) -o cupidfetch $^ -fsanitize=address $(CFLAGS) $(LDFLAGS) $(CUPID_LIBS) $(LIBS) $(LD_LIBS)

ubsan: main.c libs/ini.c
	$(CC) -o cupidfetch $^ -fsanitize=undefined $(CFLAGS) $(LDFLAGS) $(CUPID_LIBS) $(LIBS) $(LD_LIBS)

clean:
	rm -f cupidfetch *.o



