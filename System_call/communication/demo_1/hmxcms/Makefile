.PHONY: lib
all :lib app
#CC = /opt/trendchip/buildroot-gcc493_glibc222_arm32_32bServer/usr/bin/arm-buildroot-linux-gnueabi-gcc
 CC =gcc
lib:
	gcc -c lib/hmxcms_linkedlist.c -o libstatic/libhmxcms_linkedlist.o
	ar rcs libstatic/libhmxcms_linkedlist.a libstatic/libhmxcms_linkedlist.o

	gcc -c lib/hmxcms_lib.c -o libstatic/libhmxcms_lib.o
	ar rcs libstatic/libhmxcms_lib.a libstatic/libhmxcms_lib.o
app:
	gcc hmxcms_server.c -Ilib -Llibstatic -Llibstatic -lhmxcms_lib -lhmxcms_linkedlist -o libstatic/server -pthread -lrt -ggdb
	gcc hmxcms_client.c -Ilib -Llibstatic -lhmxcms_lib -o libstatic/client -pthread -lrt
	gcc hmxcms_client_receive.c -Ilib -Llibstatic -lhmxcms_lib -o libstatic/client_receive -pthread -lrt
	gcc send1.c -Ilib -Llibstatic -lhmxcms_lib -o libstatic/send1 -pthread -lrt
	gcc receive1.c -Ilib -Llibstatic -lhmxcms_lib -o libstatic/receive1 -pthread -lrt
	gcc receive2.c -Ilib -Llibstatic -lhmxcms_lib -o libstatic/receive2 -pthread -lrt 
clean:
	rm -rf ./libstatic/*.a client libstatic/server libstatic/client_receive libstatic/send1 libstatic/receive1 libstatic/receive2
	rm -rf ./libstatic/libhmxcms_lib.o ./libstatic/libhmxcms_linkedlist.o
	rm -rf ./libdynamic/*.so client libdynamic/server libdynamic/client_receive libdynamic/send1 libdynamic/receive1 libdynamic/receive2
	rm -rf ./libdynamic/libhmxcms_lib.o ./libdynamic/libhmxcms_linkedlist.o
	
dynamic: lib_dynamic app_dynamic

lib_dynamic:
	$(CC) -c -fPIC lib/hmxcms_lib.c -o libdynamic/libhmxcms_lib.o
	$(CC) -c -fPIC lib/hmxcms_linkedlist.c -o libdynamic/libhmxcms_linkedlist.o
	$(CC) -shared libdynamic/libhmxcms_lib.o libdynamic/libhmxcms_linkedlist.o -o libdynamic/libhmxcms.so
app_dynamic:
	$(CC) hmxcms_server.c -Ilib -Llibdynamic -Llibdynamic -lhmxcms -o libdynamic/server -pthread -lrt 
	$(CC) hmxcms_client.c -Ilib -Llibdynamic -lhmxcms -o libdynamic/client -pthread -lrt
	$(CC) hmxcms_client_receive.c -Ilib -Llibdynamic -lhmxcms -o libdynamic/client_receive_2 -pthread -lrt

