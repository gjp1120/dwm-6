# dwm-session - dynamic window manager
# See LICENSE file for copyright and license details.

include config.mk

SRC = dwm-session.c
OBJ = ${SRC:.c=.o}

all: options dwm-session

options:
	@echo dwm-session build options:
	@echo "CFLAGS   = ${CFLAGS}"
	@echo "LDFLAGS  = ${LDFLAGS}"
	@echo "CC       = ${CC}"

.c.o:
	@echo CC $<
	@${CC} -c ${CFLAGS} $<

${OBJ}: config.mk

dwm-session: ${OBJ}
	@echo CC -o $@
	@${CC} -o $@ ${OBJ} ${LDFLAGS}

clean:
	@echo cleaning
	@rm -f dwm-session ${OBJ} dwm-session-${VERSION}.tar.gz *~ \#*#

dist: clean
	@echo creating dist tarball
	@mkdir -p dwm-session-${VERSION}
	@cp -R Makefile config.mk \
		${SRC} dwm-session-${VERSION}
	@tar -cf dwm-session-${VERSION}.tar dwm-session-${VERSION}
	@gzip dwm-session-${VERSION}.tar
	@rm -rf dwm-session-${VERSION}

install: all
	@echo installing executable file to ${DESTDIR}${PREFIX}/bin
	@mkdir -p ${DESTDIR}${PREFIX}/bin
	@cp -f dwm-session ${DESTDIR}${PREFIX}/bin
	@chmod 755 ${DESTDIR}${PREFIX}/bin/dwm-session

uninstall:
	@echo removing executable file from ${DESTDIR}${PREFIX}/bin
	@rm -f ${DESTDIR}${PREFIX}/bin/dwm-session

.PHONY: all options clean dist install uninstall
