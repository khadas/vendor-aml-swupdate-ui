#
#Makefile
#
.PHONYH: clean

all: $(OUTPUT)/libcommon.a

$(OUTPUT)/libcommon.a:$(OUTPUT)/event_ui.o
	$(AR) -cvr $@ $^

$(OUTPUT)/event_ui.o:event_ui.c
	$(CC) $(CFLAGS) $(LDFLAGS) -c $^ -o $@

clean:
	rm -rf *.o *.a

