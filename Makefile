.PHONYH: clean
CFLAGS += -g -O3

OUT_DIR ?= $(CURDIR)
$(info "OUT_DIR : $(OUT_DIR)")

all: swupdateui

ifeq ($(CONFIG_LVGL_APP), y)
LDFLAGS += -llvgl -llv_drivers -lswupdate
LDFLAGS += -L $(OUT_DIR)/

EXTRA_MAKE = "OUT_DIR=${OUT_DIR}"

swupdateui: main.o liblvgl_ui.a liblvgl_porting.a libcommon.a
	$(CXX) $(patsubst %, $(OUT_DIR)/%, $^) -lpthread $(LDFLAGS) -o $(OUT_DIR)/$@

liblvgl_ui.a:
	$(MAKE) $(EXTRA_MAKE) -C lvgl_ui

liblvgl_porting.a:
	$(MAKE) $(EXTRA_MAKE) -C lvgl_porting

else
LDFLAGS += $(shell $(PKG_CONFIG) --libs directfb) -l++dfb
swupdateui: main.o libdirectfb_ui.a libcommon.a
	$(CXX) $(patsubst %, $(OUT_DIR)/%, $^) -o $(OUT_DIR)/$@ $(LDFLAGS) -lswupdate

libdirectfb_ui.a:
	$(MAKE) $(EXTRA_MAKE) -C directfb_ui

endif

libcommon.a:
	$(MAKE) $(EXTRA_MAKE) -C common

main.o: main.cpp
	$(CXX) $(CFLAGS) -O3 $(LDFLAGS) -c $< -o $(OUT_DIR)/$@

clean:
	rm -f $(OUT_DIR)/*.o $(OUT_DIR)/*.a  $(OUT_DIR)/*.so $(OUT_DIR)/swupdateui
	$(MAKE) $(EXTRA_MAKE) -C common clean
	$(MAKE) $(EXTRA_MAKE) -C lvgl_ui clean
	$(MAKE) $(EXTRA_MAKE) -C lvgl_porting clean
	$(MAKE) $(EXTRA_MAKE) -C directfb_ui clean
