#
#Makefile
#
.PHONYH: clean

OBJ=event_ui.o swupdate_ipc.o

all: libcommon.a

libcommon.a:$(OBJ)
	$(AR) -cvr $@ $^

$(OBJ):%.o:%.c
	$(CC) $(CFLAGS) -O3 $(LDFLAGS) -g -c $^ -o $@

clean:
	rm -rf *.o *.a

