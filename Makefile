CC = ../../gbdk/bin/lcc
CFLAGS = -Isrc/include
RELEASE = 1

ifdef RELEASE
CFLAGS += -Wf'--max-allocs-per-node 50000' -Wf'--peep-file peephole\gbz80.rul'
endif

ifdef DEBUG
CFLAGS += -Wf--debug -Wf--nolospre -Wl-m -Wl-w -Wl-y
LFLAGS += -Wf--debug -Wl-m -Wl-w -Wl-y -Wl-j
endif

all:	clean rom

%.gb:
	$(CC) $(CFLAGS) -o $@ src/rhythm.c src/routines.c src/song.c src/tiledata.c src/driver/hUGEDriver.obj.o

clean:
	rm -rf rhythm.*

rom: rhythm.gb
