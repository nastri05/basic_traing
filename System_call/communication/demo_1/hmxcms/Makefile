.PHONY: clean

clean:
	rm -rf ./libdynamic/libhmxcms_lib.o ./libdynamic/libhmxcms_linkedlist.o ./libdynamic/hmxcms_monitor.o 
	rm -rf ./libdynamic/*.so server receive send1 send
	
dynamic: lib_dynamic app_dynamic

lib_dynamic:
	mkdir -p libdynamic/
	$(CC) -c -fPIC lib/hmxcms_lib.c -o libdynamic/libhmxcms_lib.o
	$(CC) -c -fPIC lib/hmxcms_linkedlist.c -o libdynamic/libhmxcms_linkedlist.o
	$(CC) -c -fPIC lib/hmxcms_monitor.c -o libdynamic/hmxcms_monitor.o
	$(CC) -shared libdynamic/libhmxcms_lib.o libdynamic/libhmxcms_linkedlist.o libdynamic/hmxcms_monitor.o -o libdynamic/libhmxcms.so
app_dynamic:
	$(CC) hmxcms_server.c -Ilib -Llibdynamic -lhmxcms -o server -pthread -lrt 
	$(CC) send1.c -Ilib -Llibdynamic -lhmxcms -o send1 -pthread -lrt
	$(CC) receive.c -Ilib -Llibdynamic -lhmxcms -o receive -pthread -lrt

#LD_LIBRARY_PATH=./