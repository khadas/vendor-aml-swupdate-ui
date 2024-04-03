.PHONYH: clean

$(info "OUT_DIR : $(OUT_DIR)")

SRCS := $(wildcard *.c)
OBJ := $(patsubst %.c, %.o, $(SRCS))

all: liblvgl_ui.a

liblvgl_ui.a:$(OBJ)
	$(AR) -cvr $(OUT_DIR)/$@ $(patsubst %, $(OUT_DIR)/%, $^)

%.o: %.c
	$(CC) $(CFLAGS) $(LDFLAGS) -c $^ -o $(OUT_DIR)/$@

clean:
	rm -rf $(OUT_DIR)/*.o $(OUT_DIR)/*.a
