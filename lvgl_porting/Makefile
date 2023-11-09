.PHONYH: all install clean

all: $(OUTPUT)/liblvgl_porting.a

$(OUTPUT)/liblvgl_porting.a:$(OUTPUT)/lv_port_disp.o $(OUTPUT)/lv_port_fs.o
	$(AR) -cvr $@ $^

$(OUTPUT)/lv_port_disp.o: lv_port_disp.c
	$(CC) $(CFLAGS) -O3 $(LDFLAGS) -g -c $^ -o $@

$(OUTPUT)/lv_port_fs.o: lv_port_fs.c
	$(CC) $(CFLAGS) -O3 $(LDFLAGS) -g -c $^ -o $@

clean:
	rm -rf *.o *.a
