.PHONY: clean
# CC = /opt/trendchip/buildroot-gcc493_glibc222_arm32_32bServer/usr/bin/arm-buildroot-linux-gnueabi-gcc
CC = gcc
FLAG =  -Wl,-rpath=libdynamic/

clean:
	rm -rf ./libdynamic/libhmxcms_lib.o ./libstatic/libhmxcms_log.o ./libstatic/libhmxcms_linkedlist.o ./libstatic/libhmxcms_monitor.o  
	rm -rf ./libstatic/libhmxcms_linkedlist.a ./libstatic/libhmxcms_monitor.a ./libstatic/libhmxcms_log.a
	rm -rf ./libdynamic/*.so server receive send_wan_gpon send_wan_lan receive_wan send_wan_lan receive_gpon
	
dynamic: lib_dynamic app_dynamic

lib_dynamic:
	mkdir -p libdynamic/
	make clean
	$(CC) -c -fPIC lib/hmxcms_lib.c -o libdynamic/libhmxcms_lib.o

	$(CC) -c lib/hmxcms_linkedlist.c -o libstatic/libhmxcms_linkedlist.o
	ar rcs ./libstatic/libhmxcms_linkedlist.a ./libstatic/libhmxcms_linkedlist.o

	$(CC) -c lib/hmxcms_monitor.c -o libstatic/libhmxcms_monitor.o
	ar rcs libstatic/libhmxcms_monitor.a libstatic/libhmxcms_monitor.o

	$(CC) -c lib/hmxcms_log.c -o libstatic/libhmxcms_log.o
	ar rcs libstatic/libhmxcms_log.a libstatic/libhmxcms_log.o

	$(CC) -shared libdynamic/libhmxcms_lib.o -o libdynamic/libhmxcms.so
app_dynamic:
	$(CC) hmxcms_server.c -g -Ilib -Llibdynamic -lhmxcms -Llibstatic -lhmxcms_log -lhmxcms_monitor -lhmxcms_linkedlist $(FLAG) -o server -pthread -lrt -ggdb
	$(CC) send_wan_gpon.c -Ilib -Llibdynamic -lhmxcms $(FLAG) -o send_wan_gpon -pthread -lrt
	$(CC) send_wan_lan.c -Ilib -Llibdynamic -lhmxcms $(FLAG) -o send_wan_lan -pthread -lrt

	$(CC) receive_lan.c -Ilib -Llibdynamic -lhmxcms $(FLAG) -o receive_lan -pthread -lrt
	$(CC) receive_wan.c -Ilib -Llibdynamic -lhmxcms $(FLAG) -o receive_wan -pthread -lrt
	$(CC) receive_gpon.c -Ilib -Llibdynamic -lhmxcms $(FLAG) -o receive_gpon -pthread -lrt
#LD_LIBRARY_PATH=./
#LD_LIBRARY_PATH=libdynamic/
