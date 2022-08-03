.PHONYH: all install clean

OBJ=aml_swupdate_ui.o dfbapp.o dfbimage.o

CFLAGS += $(shell $(PKG_CONFIG) --cflags directfb)
LDFLAGS += $(shell $(PKG_CONFIG) --libs directfb) -l++dfb

all: swupdateui

swupdateui: $(OBJ)
	$(CXX) $^ $(LDFLAGS) -o $@

$(OBJ):%.o:%.cpp
	$(CXX) -c $(CFLAGS) -g $^ -o $@

clean:
	rm -f *.o swupdateui
