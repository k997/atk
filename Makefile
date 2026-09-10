CC      = gcc
HIDAPI  = third_party/hidapi/src/hid.c
CFLAGS  = -I third_party/hidapi/include -static -lsetupapi

all: bin/atk-battery.exe bin/atk-battery-gui.exe

bin/atk-battery.exe: src/atk-battery.c $(HIDAPI) | bin
	$(CC) $^ -o $@ $(CFLAGS)

bin/atk-battery-gui.exe: src/atk-battery-gui.c $(HIDAPI) | bin
	$(CC) $^ -o $@ $(CFLAGS) -lgdi32 -mwindows -fexec-charset=GBK

bin:
	mkdir -p bin

clean:
	rm -f bin/*.exe

.PHONY: all clean
