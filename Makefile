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
# 2020-11-02 21:43
# Joe
#
# GNU Makefile

.DEFAULT_GOAL	:= asan
SHELL			:= /bin/sh

DESTDIR			 = /usr/local
SRCS_DIR		 = src/
OBJS_DIR		 = obj/

CC				 = cc
CFLAGS			 = -std=c89
CFLAGS			+= -Wall
CFLAGS			+= -Wextra
CFLAGS			+= -Werror
CFLAGS			+= -pedantic

RM				 = rm -rf
MKDIR			 = mkdir -p

SRCS_NAME		 = c_lfiles
SRCS_NAME		+= c_opts
SRCS_NAME		+= c_subst
SRCS_NAME		+= c_unixize
SRCS_NAME		+= u_utils

SRCS			 = $(addprefix ${SRCS_DIR}, $(addsuffix .c, ${SRCS_NAME}))
INCS			 = $(addprefix ${SRCS_DIR}, $(addsuffix .h, ${SRCS_NAME}))
OBJS			 = $(patsubst ${SRCS_DIR}%.c, ${OBJS_DIR}%.c.o, ${SRCS})

TARGET			 = unixize

${OBJS_DIR}%.c.o: ${SRCS_DIR}%.c ${INCS} Makefile
	@${MKDIR} ${OBJS_DIR}
	${CC} -c ${CFLAGS} -o $@ $<

${TARGET}: ${OBJS}
	${CC} ${CFLAGS} -o ${TARGET} ${OBJS}

opti: CFLAGS += -march=ivybridge -O2 -pipe
opti: all

all: ${TARGET}

debug: CFLAGS += -g3
debug: all

asan: CFLAGS += -g3
asan: CFLAGS += -fsanitize=address
asan: all

msan: CFLAGS += -g3
msan: CFLAGS += -fsanitize=memory
msan: CFLAGS += -fsanitize-memory-track-origins
msan: all

clean:
	${RM} ${OBJS_DIR}
	${RM} ${TARGET}

.PHONY:	all clean debug asan msan
