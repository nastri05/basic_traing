objects = a
CC= gcc
all: $(objects)
$(object): %.o :%.c

%.c::
	touch $@
%.c::
	echo "int main() { return 0; }" > $@

clean: 
	rm -f *.o