.PHONY: run
LIB_DYNAMIC = lib/
all: app run
run:
	LD_LIBRARY_PATH=${LIB_DYNAMIC} ./app

app:lib/libbar.so
	gcc app.c -Ilib -Llib -lbar -rdynamic -o $@
lib/libbar.so:lib/libbar.o
	gcc -shared lib/libbar.o  -o lib/libbar.so
lib/libbar.o:
	gcc -c -fpic lib/bar.c  -o lib/libbar.o
# with -rdynamic to export callback symbol:
	
clean:
	rm -f lib/libbar.so
	rm -f lib/libbar.o
	rm -f app