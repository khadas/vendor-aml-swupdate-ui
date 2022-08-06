.PHONYH: clean

all: liblvgl_ui.a

liblvgl_ui.a:aml_lvgl_ui.o
	$(AR) -cvr $@ $^

aml_lvgl_ui.o:%.o:%.c
	$(CC) $(CFLAGS) -O3 $(LDFLAGS) -g -c $^ -o $@

clean:
	rm -rf *.o *.a
