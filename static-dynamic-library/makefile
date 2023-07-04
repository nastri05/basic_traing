CC =gcc
CC_LIB = ar
OBJ_static = app_static_local
LIB_STATIC = lib/static
OBJ_dynamic = app_dynamic_local
LIB_DYNAMIC = lib/dynamic
FILE = app.c

static_local:$(OBJ_static)
	./$^
$(OBJ_static):lib/static/libfoo.a
	$(CC) $(FILE) -Ilib -L$(LIB_STATIC) -lfoo -o $@

lib/static/libfoo.a:
	$(CC) -c lib/foo.c -o $(LIB_STATIC)/libfoo.o
	$(CC_LIB) rcs $(LIB_STATIC)/libfoo.a $(LIB_STATIC)/libfoo.o

dynamic_local:$(OBJ_dynamic)
	LD_LIBRARY_PATH=$(LIB_DYNAMIC) ./$^
	
$(OBJ_dynamic):$(LIB_DYNAMIC)/libfoo.so
	$(CC) $(FILE) -Ilib -L$(LIB_DYNAMIC) -lfoo -o $@

$(LIB_DYNAMIC)/libfoo.so:
	$(CC) -c lib/foo.c -o $(LIB_DYNAMIC)/libfoo.o
	$(CC) -shared $(LIB_DYNAMIC)/libfoo.o -o $(LIB_DYNAMIC)/libfoo.so

clean:
	rm -f lib/static/libfoo.o
	rm -f lib/static/libfoo.a
	rm -f $(OBJ_static)
	rm -f $(LIB_DYNAMIC)/libfoo.so
	rm -f $(LIB_DYNAMIC)/libfoo.o
	rm -f $(OBJ_dynamic)
	