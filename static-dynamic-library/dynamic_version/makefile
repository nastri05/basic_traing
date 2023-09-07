CC =gcc
.PHONY: dynamic_local main_dynamic
#.PHONY: $(OBJ_dynamic)
#FILE_SO = libtuan.so.1.0
#OBJ_dynamic = main_dynamic
#LIB_DYNAMIC = lib/dynamic
#FILE = test.c
SOURCE_FILE = lib/tuan.c
MAIN_FILE = test.c
SONAME = lib/dynamic/libtuan.so.1
REAL_NAME = lib/dynamic/libtuan.so.1.1
LINKER_NAME = lib/dynamic/libtuan.so
OUTPUT_FILE = main_dynamic

dynamic_local:

#	$(CC) -c -fPIC lib/tuan.c -o $(LIB_DYNAMIC)/libtuan.o
#	$(CC) -shared $(LIB_DYNAMIC)/libtuan.o -o $(LIB_DYNAMIC)/$(FILE_SO)
#	sudo install -m 755 lib/tuan.h /usr/include
#	sudo install -m 755 lib/dynamic/$(FILE_SO) /usr/lib/
#	$(CC) $(FILE) -ltuan -o $@
	$(CC) -c -fPIC $(SOURCE_FILE) -o lib/dynamic/libtuan.o
	$(CC) -shared -Wl,-soname,$(SONAME) -o $(REAL_NAME) lib/dynamic/libtuan.o
	ln -s ${REAL_NAME} ${SONAME}
	$(CC) $(MAIN_FILE) -I. -L. -ladd -o ${OUTPUT_FILE} 
	ln -s ${SONAME} ${LINKER_NAME}

clean:
	
	rm -f lib/dynamic/libtuan.o
	#sudo rm /usr/lib/$(FILE_SO) /usr/include/tuan.h
	rm -f lib/dynamic/$(FILE_SO)
	#rm -f main_dynamic