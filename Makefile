################################################################################
#*                                                    =====    ===============*#
#*   File     : Makefile                              ======  ================*#
#*   Author   : Joe                                   ======  ================*#
#*   Date     : 2020-11-02                            ======  ====   ====   ==*#
#*   Info     : GNU Makefile                          ======  ===     ==  =  =*#
#*                                                    ======  ===  =  ==     =*#
#*                                                    =  ===  ===  =  ==  ====*#
#*                                                    =  ===  ===  =  ==  =  =*#
#*                                                    ==     =====   ====   ==*#
################################################################################

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

clean:
	${RM} ${OBJS_DIR}
	${RM} ${TARGET}
