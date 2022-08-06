.PHONYH: clean

OBJ=aml_directfb_ui.o dfbapp.o dfbimage.o

CFLAGS_EXTRA += $(shell $(PKG_CONFIG) --cflags directfb)

all: libdirectfb_ui.a
libdirectfb_ui.a:$(OBJ)
	$(AR) -cvr $@ $^

$(OBJ):%.o:%.cpp
	$(CXX) $(CFLAGS) $(CFLAGS_EXTRA) -c -O3 -g $^ -o $@

clean:
	rm -rf *.o *.a
