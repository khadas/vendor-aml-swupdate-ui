.PHONYH: all install clean

$(info "OUT_DIR : $(OUT_DIR)")

SRCS := $(wildcard *.c)
OBJ := $(patsubst %.c, %.o, $(SRCS))

all: liblvgl_porting.a

liblvgl_porting.a:$(OBJ)
	$(AR) -cvr $(OUT_DIR)/$@ $(patsubst %, $(OUT_DIR)/%, $^)

%.o: %.c
	$(CC) $(CFLAGS) -O3 $(LDFLAGS) -g -c $^ -o $(OUT_DIR)/$@

clean:
	rm -rf $(OUT_DIR)/*.o $(OUT_DIR)/*.a
