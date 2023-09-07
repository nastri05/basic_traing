objects = hmxcon_clientA hmxcon_server hmxcms_linkedlist
# CC= gcc
CC = /opt/trendchip/buildroot-gcc493_glibc222_arm32_32bServer/usr/bin/arm-buildroot-linux-gnueabi-gcc
all: hmxcon_clientA hmxcon_server
hmxcon_clientA:
	$(CC) $@.c -o $@ -lrt 
hmxcon_server:service.o
	gcc hmxcon_server.c -Ilib -Llib -pthread -lrt -lservice -o hmxcon_server 
service.o:
	gcc -c -fPIC service.c -o lib/libservice.o
	gcc -shared lib/libservice.o -o lib/libservice.so
	# sudo install -m 755 inc/service.h /usr/include/
	# sudo install -m 755 lib/libservice.so /usr/lib/
run:
	LD_LIBRARY_PATH=lib/ ./hmxcon_server &
clean: 
	rm -rf $(objects) 
	# sudo rm /usr/lib/libservice.so /usr/include/service.h
hmxcms_linkedlist:
	$(CC) $@.c  -g -o $@