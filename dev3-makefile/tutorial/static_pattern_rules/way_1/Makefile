objects = foo.o bar.o all.o
all: $(objects)

# These files compile via implicit rules
foo.o: foo.c
bar.o: bar.c
all.o: all.c
# if 3 line above not used then %.c not not file 
all.c:
	echo "int main() { return 0; }" > all.c

%.c:
	echo $@
	touch $@

clean:
	rm -f *.c *.o all