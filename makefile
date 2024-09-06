CPPFLAGS = -Iinclude -Wall

src = $(wildcard src/*.c)
obj = $(patsubst src/%.c, build/%.o, $(src))
headers = $(wildcard include/*.h)

main: $(obj)
	$(CC) $(obj) -o main 

build/%.o: src/%.c ${headers}
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

clean:
	rm build/*.o lvl-ip
