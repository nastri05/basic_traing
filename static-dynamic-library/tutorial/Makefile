SOURCE_FILE=add.c
# main.cpp includes `add.h`, whose implementation is `add.cpp`
MAIN_FILE=main.c
SONAME=libadd.so.1
REAL_NAME=libadd.so.1.1.0
LINKER_NAME=libadd.so
OUTPUT_FILE=a.out

all:
	sudo gcc -shared -fPIC -Wl,-soname,${SONAME} -o ${REAL_NAME} ${SOURCE_FILE}
	ln -s ${REAL_NAME} ${SONAME}
	ln -s ${SONAME} ${LINKER_NAME}	
	#gcc main.c -I. -L. -ladd -o ${OUTPUT_FILE}
	#sudo install -m 755 add.h /usr/include
	#sudo install -m 755 $(SONAME) /usr/lib/
	 
   # Same as `ldconfig -n .`, creates a symbolic link
	
   #./a.out: error while loading shared libraries: libadd.so.1: cannot open 
   # shared object file: No such file or directory
   
	#LD_LIBRARY_PATH=. ./${OUTPUT_FILE}
clean:
	rm ${SONAME} ${REAL_NAME} ${LINKER_NAME}
	sudo rm /usr/lib/$(SONAME) /usr/include/add.h