default: msan

SHELL		:= /bin/sh

DESTDIR		 = /usr/local
SRCS_DIR	 = src/
OBJS_DIR	 = obj/

CC			 = cc
CFLAGS		 = -std=c89
CFLAGS		+= -Wall
CFLAGS		+= -Wextra
CFLAGS		+= -Werror
CFLAGS		+= -pedantic

RM			 = rm -f
MKDIR		 = mkdir -p

SRCS_NAME	 = c_unixize

SRCS		 = $(addprefix ${SRCS_DIR}, $(addsuffix .c, ${SRCS_NAME}))
INCS		 = $(addprefix ${SRCS_DIR}, $(addsuffix .h, ${SRCS_NAME}))
OBJS		 = $(patsubst ${SRCS_DIR}%.c, ${OBJS_DIR}%.c.o, ${SRCS})

TARGET		 = unixize

${OBJS_DIR}%.o: ${SRCS_DIR}%.c ${INCS} Makefile
	@${MKDIR} ${OBJS_DIR}
	${CC} -c ${CFLAGS} $@ $<

all: ${OBJS}
	${CC} ${CFLAGS} -o ${TARGET} ${OBJS}

debug: CFLAGS += -glldb
debug: all

asan: CFLAGS += -glldb
asan: CFLAGS += -fsanitize=address
asan: all

msan: CFLAGS += -glldb
msan: CFLAGS += -fsanitize=memory
msan: CFLAGS += -fsanitize-memory-track-origins
msan: all

clean:
	${RM} ${OBJS_DIR}
	${RM} ${TARGET}

.PHONY:	all clean debug asan msan
