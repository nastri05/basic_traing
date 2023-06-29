OBJECTS = send_mmap read_mmap
all :$(OBJECTS)
send_mmap:send_mmap.c
	gcc $^ -lrt -o $@
read_mmap:read_mmap.c
	gcc $^ -lrt -o $@
clean:
	rm -f *.o