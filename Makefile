# ========================
# =====    ===============
# ======  ================
# ======  ================
# ======  ====   ====   ==
# ======  ===     ==  =  =
# ======  ===  =  ==     =
# =  ===  ===  =  ==  ====
# =  ===  ===  =  ==  =  =
# ==     =====   ====   ==
# ========================
#
# unixize: Makefile
# Fri Nov 27 22:53:29 CET 2020
# Joe
#
# GNU Makefile

.DEFAULT_GOAL	:= all
SHELL			:= /bin/sh

DESTDIR			:=
SRCS_DIR		:= src/
OBJS_DIR		:= obj/
MAN_DIR			:= man/

CC				?= cc
CFLAGS			+= -std=c89
CFLAGS			+= -Wall
CFLAGS			+= -Wextra
CFLAGS			+= -Werror
CFLAGS			+= -pedantic
CFLAGS			+= -march=native -O2 -pipe

MKDIR			:= mkdir -p
RM				:= rm -f
INSTALL			:= install
GZIP			:= gzip
GUNZIP			:= gunzip

SRCS_NAME		:= c_lfiles
SRCS_NAME		+= c_opts
SRCS_NAME		+= c_subst
SRCS_NAME		+= c_unixize
SRCS_NAME		+= u_utils

SRCS			 = $(addprefix ${SRCS_DIR}, $(addsuffix .c, ${SRCS_NAME}))
INCS			 = $(addprefix ${SRCS_DIR}, $(addsuffix .h, ${SRCS_NAME}))
OBJS			 = $(patsubst ${SRCS_DIR}%.c, ${OBJS_DIR}%.c.o, ${SRCS})

TARGET			:= unixize
MAN				:= unixize.1.gz

${OBJS_DIR}%.c.o: ${SRCS_DIR}%.c ${INCS} Makefile
	@${MKDIR} ${OBJS_DIR}
	${CC} -c ${CFLAGS} -o $@ $<

${TARGET}: ${OBJS}
	${CC} ${CFLAGS} -o ${TARGET} ${OBJS}

${MAN_DIR}${MAN}:
	${GZIP} ${MAN_DIR}${TARGET}.1

all: ${TARGET}

clean:
	${RM} -R ${OBJS} ${OBJS_DIR}
	${RM} ${TARGET}

install-bin: ${TARGET}
	${MKDIR} ${DESDIR}/usr/local/bin
	${INSTALL} -m0555 ${TARGET} ${DESTDIR}/usr/local/bin/${TARGET}

install-doc: ${MAN_DIR}${MAN}
	${MKDIR} ${DESDIR}/usr/local/man/man1
	${INSTALL} -m0444 ${MAN_DIR}${TARGET}.1.gz ${DESTDIR}/usr/local/man/man1/${TARGET}.1.gz
	${GUNZIP} ${MAN_DIR}${TARGET}.1.gz

install: install-bin install-doc


uninstall:
	${RM} ${DESTDIR}/usr/local/bin/${TARGET}
	${RM} ${DESTDIR}/usr/local/man/man1/${TARGET}.1.gz

debug: CFLAGS += -g3
debug: all

asan: CFLAGS += -g3
asan: CFLAGS += -fsanitize=address
asan: all

msan: CFLAGS += -g3
msan: CFLAGS += -fsanitize=memory
msan: CFLAGS += -fsanitize-memory-track-origins
msan: all

.PHONY:	all clean install uninstall debug asan msan
