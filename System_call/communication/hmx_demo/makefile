all:
	gcc -c hmxcms_lib.c -o lib/libhmxcms_lib.o
	ar rcs lib/libhmxcms_lib.a lib/libhmxcms_lib.o

	gcc client.c -I/. -Llib/ -lhmxcms_lib -o client -lrt 
clean:
	rm -rf lib/libhmxcms_lib.a lib/libhmxcms_lib.o client 