#
#Configured directories
#
#prefix	=	/usr/local
#exec_prefix	=	/usr/local
#libdir	=	$(exec_prefix)/lib
libdir=	$(LIB_INSTALL_DIR)
publiclibdir= ../../public/lib
#
#Install commands
#
INSTALL=	/usr/bin/install -c
INSTALL_DIR	=	$(INSTALL) -d
INSTALL_LIB	=	$(LIBTOOL)	$(INSTALL) -m 755

#CROSS=mips-linux-gnu-

#CC=$(CROSS)gcc -mips32r2 -msoft-float -muclibc
AR=$(CROSS)ar
LD=$(CROSS)ld 
STRIP=$(CROSS)strip
RANLIB=$(CROSS)ranlib
SIZE=$(CROSS)size
LIBDIR=$(CROSS_PATH)/lib

LN      =	/bin/ln -s
RM      =	/bin/rm -f


#CFLAGS= -mips1 -msoft-float 
CFLAGS +=-fPIC -Os -g  -Wall -shared 

ifneq ($(TC_CFLAGS), )
        CFLAGS+=$(TC_CFLAGS)
endif

.SUFFIXES:  .c .o
.c.o:
	$(CC) $(CFLAGS) -c $<

LIBOBJS =       libhmxcms.o
LIBTCAPI=       libhmxcms.so.1.4
TARGETS =       $(LIBTCAPI)
SOFTLINK=       libhmxcms.so

all:  $(TARGETS)

$(LIBOBJS): libhmxcms.h
libhmxcms.o:libhmxcms.h

libhmxcms.so.1.4: $(LIBOBJS)
	$(CC) $(CFLAGS) $(DSOFLAGS) -o libhmxcms.so.1.4 $(LIBOBJS)
	$(STRIP) --strip-unneeded $(LIBTCAPI)
	$(RM) $(SOFTLINK)
	$(LN) libhmxcms.so.1.4 libhmxcms.so

ifneq ($(strip $(TCSUPPORT_OPENWRT)),)
install:
	$(INSTALL_DIR) $(BUILDROOT)$(libdir)
	$(INSTALL_LIB) libhmxcms.so.1.4 $(BUILDROOT)$(libdir)
	$(RM) $(BUILDROOT)$(libdir)/libhmxcms.so
	$(LN) libhmxcms.so.1.4 $(BUILDROOT)$(libdir)/libhmxcms.so
	cp libhmxcms.h $(BUILDROOT)$(libdir)
	cp libhmxcms.h $(BUILDROOT)$(publiclibdir)

clean:
	$(RM) $(OBJS) $(TARGETS) $(LIBOBJS) $(SOFTLINK)
else
install:
	cp -a $(APP_HMXCMS_DIR)/libhmxcms.so*  $(APP_LIB_DIR)
	cp -a $(APP_HMXCMS_DIR)/libhmxcms.so*  $(BSP_EXT_LIB)
	cp  $(APP_HMXCMS_DIR)/libhmxcms.h  $(APP_LIB_INC)
	cp  $(APP_HMXCMS_DIR)/libhmxcms.h  $(BSP_EXT_INC)

clean:
	rm -rf $(APP_HMXCMS_DIR)/libhmxcms.*
	rm -rf $(APP_LIB_DIR)/libhmxcms.so*
	rm -rf $(BSP_EXT_LIB)/libhmxcms.so*
	rm -rf $(APP_LIB_INC)/libhmxcms.h
	rm -rf $(BSP_EXT_INC)/libhmxcms.h
endif
