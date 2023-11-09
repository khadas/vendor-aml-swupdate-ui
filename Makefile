.PHONYH: clean
CFLAGS += -g -O3

OUT_DIR ?= $(CURDIR)

all: $(OUT_DIR)/swupdateui

ifeq ($(CONFIG_LVGL_APP), y)
LDFLAGS += -llvgl -llv_drivers -lswupdate
$(OUT_DIR)/swupdateui: $(OUT_DIR)/main.o $(OUT_DIR)/liblvgl_ui.a $(OUT_DIR)/liblvgl_porting.a $(OUT_DIR)/libcommon.a
	$(CXX) $^ -lpthread $(LDFLAGS) -o $@

$(OUT_DIR)/liblvgl_ui.a:
	$(MAKE) -C lvgl_ui OUTPUT=$(OUT_DIR)

$(OUT_DIR)/liblvgl_porting.a:
	$(MAKE) -C lvgl_porting OUTPUT=$(OUT_DIR)

else
LDFLAGS += $(shell $(PKG_CONFIG) --libs directfb) -l++dfb
swupdateui: main.o directfb_ui/libdirectfb_ui.a common/libcommon.a
	$(CXX) $^ -o $@ $(LDFLAGS) -lswupdate

directfb_ui/libdirectfb_ui.a:
	$(MAKE) -C directfb_ui

endif

$(OUT_DIR)/libcommon.a:
	$(MAKE) -C common OUTPUT=$(OUT_DIR)

$(OUT_DIR)/main.o: main.cpp
	$(CXX) $(CFLAGS) -O3 $(LDFLAGS) -c $< -o $@

clean:
	rm -f *.o swupdateui
	$(MAKE) -C common clean
	$(MAKE) -C lvgl_ui clean
	$(MAKE) -C lvgl_porting clean
	$(MAKE) -C directfb_ui clean
