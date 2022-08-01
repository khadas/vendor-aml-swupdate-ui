.PHONYH: all install clean

all: liblvgl_porting.a

liblvgl_porting.a:lv_port_disp.o lv_port_fs.o
	$(AR) -cvr $@ $^

lv_port_disp.o lv_port_fs.o:%.o:%.c
	$(CC) $(CFLAGS) -O3 $(LDFLAGS) -g -c $^ -o $@

clean:
	rm -rf *.o *.a
