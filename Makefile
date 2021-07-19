CC = ../../gbdk/bin/lcc
CFLAGS = -Isrc/include -Wl-j -Wm-yS -Wl-kplayer -Wl-lhUGEDriver.lib
RELEASE = 1
#DEBUG = 1

ifdef RELEASE
CFLAGS += -Wf'--max-allocs-per-node 50000'
endif

ifdef DEBUG
CFLAGS += -Wf--debug -Wf--nolospre -Wl-m -Wl-w -Wl-y
endif

all:	clean rom

%.gb:
	$(CC) $(CFLAGS) -o $@ src/*.c

clean:
	rm -rf rhythm.*

rom: rhythm.gb
