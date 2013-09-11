## Copyright (C) 1999 Lars Brinkhoff

bin_PROGRAMS =	read_vt220 read_linux_console read_xev \
		write_vt220 write_xsendevent write_xtest

read_vt220_SOURCES = read_vt220.c vt220.c write.c string.c
read_linux_console_SOURCES = read_linux_console.c write.c string.c
read_xev_SOURCES = read_xev.c write.c string.c
write_vt220_SOURCES = write_vt220.c read.c string.c
write_xsendevent_SOURCES = write_xsendevent.c x.c tab.c read.c string.c
write_xsendevent_LDADD = -L/usr/X11R6/lib -lX11
write_xtest_SOURCES = write_xtest.c x.c tab.c read.c string.c
write_xtest_LDADD = -L/usr/X11R6/lib -lX11 -lXtst

noinst_HEADERS = protocol.h vt220.h x.h
