
all: lib app

lib:
	gcc -c ./libstatic/hmxcms_linkedlist.c -o ./libstatic/libhmxcms_linkedlist.o
	ar rcs ./libstatic/libhmxcms_linkedlist.a ./libstatic/libhmxcms_linkedlist.o

	gcc -c ./libstatic/hmxcms_monitor.c -o ./libstatic/libhmxcms_monitor.o
	ar rcs ./libstatic/libhmxcms_monitor.a ./libstatic/libhmxcms_monitor.o

	gcc -c libstatic/hmxcms_lib.c -o libstatic/libhmxcms_lib.o
	ar rcs libstatic/libhmxcms_lib.a libstatic/libhmxcms_lib.o

app:
	gcc hmxcms_server.c -Ilibstatic/ -Llibstatic -Llibstatic -lhmxcms_monitor -lhmxcms_linkedlist -o server -lrt -ggdb
	gcc receive.c -Ilibstatic/ -Llibstatic -lhmxcms_lib -o receive -lrt
	gcc receive2.c -Ilibstatic/ -Llibstatic -lhmxcms_lib -o receive2 -lrt
	gcc send.c -Ilibstatic/ -Llibstatic -lhmxcms_lib -o send -lrt
	gcc send1.c -Ilibstatic/ -Llibstatic -lhmxcms_lib -o send1 -lrt
clean:
	rm -rf ./libstatic/*.a server receive send send1 receive2
	rm -rf ./libstatic/libhmxcms_monitor.o ./libstatic/libhmxcms_linkedlist.o
