#
#Makefile
#
.PHONYH: clean

$(info "OUT_DIR : $(OUT_DIR)")

SRCS := $(wildcard *.c)
OBJ := $(patsubst %.c, %.o, $(SRCS))

all: libcommon.a

libcommon.a:$(OBJ)
	$(info "input : $(patsubst %, $(SUB_OUT_DIR)/%, $^)")
	$(info "output : $(OUT_DIR)/$@")
	$(AR) -cvr $(OUT_DIR)/$@ $(patsubst %, $(OUT_DIR)/%, $^)

%.o: %.c
	$(info "input : $^")
	$(info "output : $(OUT_DIR)/$@")
	$(CC) $(CFLAGS) $(LDFLAGS) -c $^ -o $(OUT_DIR)/$@

clean:
	rm -rf $(OUT_DIR)/*.o $(OUT_DIR)/*.a
