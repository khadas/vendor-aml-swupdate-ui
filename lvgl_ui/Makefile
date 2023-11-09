.PHONYH: clean

all: $(OUTPUT)/liblvgl_ui.a

$(OUTPUT)/liblvgl_ui.a:$(OUTPUT)/aml_lvgl_ui.o
	$(AR) -cvr $@ $^

$(OUTPUT)/aml_lvgl_ui.o:aml_lvgl_ui.c
	$(CC) $(CFLAGS) $(LDFLAGS) -c $^ -o $@

clean:
	rm -rf *.o *.a
