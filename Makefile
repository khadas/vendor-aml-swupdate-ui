.PHONYH: all install clean

OBJ=aml_swupdate_ui.o dfbapp.o dfbimage.o

export PKG_CONFIG_PATH=$(TARGET_DIR)/../host/$(CROSSCOMPILE)/sysroot/usr/lib/pkgconfig
export PKG_CONFIG=$(TARGET_DIR)/../host/bin/pkg-config
CFLAGS += $(shell $(PKG_CONFIG) --cflags directfb)
LDFLAGS += $(shell $(PKG_CONFIG) --libs directfb) -l++dfb

all: swupdateui

swupdateui: $(OBJ)
	$(CXX) $^ $(LDFLAGS) -o $@

$(OBJ):%.o:%.cpp
	$(CXX) -c $(CFLAGS) -g $^ -o $@

clean:
	rm -f *.o swupdateui
