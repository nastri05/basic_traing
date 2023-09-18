EXE = hmxcms_receive2

.PHONY: all dynamic install $(EXE)
all dynamic install: $(EXE)

TC_CFLAGS_NO_CLMP=$(patsubst -DCLMP,,$(TC_CFLAGS))
ifneq ($(TC_CFLAGS_NO_CLMP),)
CFLAGS+=$(TC_CFLAGS_NO_CLMP)
endif

LIBS += -lpthread

# # For using humax API
# LIBS += -L$(HMX_LIB_INSTALL_DIR)

# For runtime linking
LIBS += -Wl,-rpath=./
# LIBS += -Wl,-rpath=$(HMX_FS_LIB)

#for using tcapi API
TCAPILIB = $(LIBINSTALL_LDFLAGS) $(TCAPI_LIB) $(HMXCMS_LIB)

# For linking (find dependent libs of linked libs)
LIBS += -Wl,-rpath-link=$(LIB_INSTALL_DIR)
LIBS += -Wl,-rpath-link=$(BSP_EXT_LIB)
# LIBS += -Wl,-rpath-link=$(HMX_LIB_INSTALL_DIR)

SOURCES = $(wildcard *.c)
OBJECTS = $(SOURCES:%.c=%.o)


ALLOWED_INCLUDE_PATHS +=    -I./ \
                            -I$(APP_TCAPILIB_DIR) \
							-I$(APP_HMXCMS_DIR) \
							-I/home/vqhung/vqh/develop/tclinux/apps/private/hmxcms_receive2 \
                            # -I$(LIB_FAL_HMX_DIR)/include 
                            # -I$(LIB_HMX_UTILS_DIR)/inc

CFLAGS += $(ALLOWED_INCLUDE_PATHS)
CFLAGS += -lrt -g -Werror -Wfatal-errors

# # Add bellow flags for dependent falhmx lib
# CFLAGS += -DCONFIG_HMX_FACTORY_SW -DSUPPORT_DM_LEGACY98

$(EXE): $(OBJECTS)
	$(CC) hmxcms_receive2.c $(LIBS) $(TCAPILIB) $(ALLOWED_INCLUDE_PATHS) -o $@ $(CFLAGS)

.PHONY: clean
clean:
	rm -f $(EXE)
	# @cd $(APP_HMX_STALE_WA_DIR) && find . -type f -name "*.o" -exec rm -f {} \;
	# @find . -type f -name "*.a"  -exec rm -f {} \;
	# @find . -type f -name "*.o"  -exec rm -f {} \;
	# @find . -type f -name "*.d*" -exec rm -f {} \;