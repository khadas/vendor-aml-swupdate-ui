.PHONYH: clean

$(info "OUT_DIR : $(OUT_DIR)")

SRCS := $(wildcard *.cpp)
OBJ := $(patsubst %.cpp, %.o, $(SRCS))

CFLAGS_EXTRA += $(shell $(PKG_CONFIG) --cflags directfb)

all: libdirectfb_ui.a
libdirectfb_ui.a:$(OBJ)
	$(AR) -cvr $(OUT_DIR)/$@ $(patsubst %, $(OUT_DIR)/%, $^)

%.o: %.cpp
	$(CXX) $(CFLAGS) $(CFLAGS_EXTRA) -c -O3 -g $^ -o $(OUT_DIR)/$@

clean:
	rm -rf $(OUT_DIR)/*.o $(OUT_DIR)/*.a