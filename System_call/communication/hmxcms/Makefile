

all :lib app

lib:
	gcc -c ./libstatic/hmxcms_linkedlist.c -o ./libstatic/libhmxcms_linkedlist.o
	ar rcs ./libstatic/libhmxcms_linkedlist.a ./libstatic/libhmxcms_linkedlist.o

	gcc -c libstatic/hmxcms_lib.c -o libstatic/libhmxcms_lib.o
	ar rcs libstatic/libhmxcms_lib.a libstatic/libhmxcms_lib.o
app:
	gcc hmxcms_server.c -Ilibstatic/ -Llibstatic -Llibstatic -lhmxcms_lib -lhmxcms_linkedlist -o server -pthread -lrt
	gcc hmxcms_client.c -Ilibstatic/ -Llibstatic -lhmxcms_lib -o client -lrt 
clean:
	rm -rf ./libstatic/*.a client server 
	rm -rf ./libstatic/libhmxcms_lib.o ./libstatic/libhmxcms_linkedlist.o

	
