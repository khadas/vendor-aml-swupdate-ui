.PHONYH: clean

all: swupdateui

ifeq ($(CONFIG_DIRECTFB_APP), y)
OBJ=directfb_ui
LDFLAGS += $(shell $(PKG_CONFIG) --libs directfb) -l++dfb
swupdateui: main.o directfb_ui/libdirectfb_ui.a common/libcommon.a
	$(CXX) $^ -o $@ $(LDFLAGS)

directfb_ui/libdirectfb_ui.a:
	$(MAKE) -C directfb_ui

else
ifeq ($(CONFIG_LVGL_APP), y)
OBJ=lvgl_ui lvgl_porting
LDFLAGS += -llvgl -llv_drivers
swupdateui: main.o lvgl_ui/liblvgl_ui.a lvgl_porting/liblvgl_porting.a common/libcommon.a
	$(CXX) $^ -o $@ $(LDFLAGS)

lvgl_ui/liblvgl_ui.a:
	$(MAKE) -C lvgl_ui

lvgl_porting/liblvgl_porting.a:
	$(MAKE) -C lvgl_porting

endif
endif

common/libcommon.a:
	$(MAKE) -C common

main.o: main.cpp
	$(CXX) $(CFLAGS) -O3 $(LDFLAGS) -c $< -o $@

#swupdateui:${swupdateui_SRC}
#	$(CXX) $(CFLAGS) -O3 $(LDFLAGS) $^ -o $@
clean:
	rm -f *.o swupdateui
	$(MAKE) -C common clean
	$(MAKE) -C lvgl_ui clean
	$(MAKE) -C lvgl_porting clean
	$(MAKE) -C directfb_ui clean
