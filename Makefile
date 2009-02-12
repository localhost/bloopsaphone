SRC = c/bloopsaphone.c c/notation.c
OBJ = ${SRC:.c=.o}

PREFIX = /usr/local
CC = gcc
CFLAGS = -Wall
DEBUG ?= 0
ECHO = /bin/echo
INCS = -Ic -I/opt/local/include
LIBS = -L/opt/local/lib -lm -lportaudio
RAGEL = ragel

RAGELV = `${RAGEL} -v | sed "/ version /!d; s/.* version //; s/ .*//"`

all: bloopsaphone

rebuild: clean bloopsaphone

bloopsaphone: bloopsawhat

bloopsawhat: ${OBJ} c/bloopsawhat.o
	@${ECHO} LINK bloopsawhat
	@${CC} ${CFLAGS} ${OBJ} c/bloopsawhat.o ${LIBS} -o bloopsawhat

c/notation.c: c/notation.rl
	@if [ "${RAGELV}" != "6.3" ]; then \
		if [ "${RAGELV}" != "6.2" ]; then \
			${ECHO} "** bloopsaphone may not work with ragel ${RAGELV}! try version 6.2 or 6.3."; \
		fi; \
	fi
	@${ECHO} RAGEL c/notation.rl
	@${RAGEL} c/notation.rl -C -o $@

%.o: %.c
	@${ECHO} CC $<
	@${CC} -c ${CFLAGS} ${INCS} -o $@ $<

clean:
	@${ECHO} cleaning
	@rm -f ${OBJ}
	@rm -f c/notation.c
	@rm -f bloopsawhat c/bloopsawhat.o bloopsaphone.a bloopsaphone.so

ruby: c/notation.c c/bloopsaphone.c
	@${ECHO} copying c TO ext/ruby
	@cp c/notation.c c/bloopsaphone.c c/bloopsaphone.h ext/ruby
	@${ECHO} RUBY extconf.rb
	@cd ext/ruby && ruby extconf.rb && make
	@${ECHO} ""
	@${ECHO} "To test: cd ext/ruby"
	@${ECHO} "Then:    ruby test.rb"
	@${ECHO} ""

.PHONY: all bloopsaphone clean rebuild ruby
