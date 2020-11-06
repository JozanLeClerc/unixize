/*
 * ========================
 * =====    ===============
 * ======  ================
 * ======  ================
 * ======  ====   ====   ==
 * ======  ===     ==  =  =
 * ======  ===  =  ==     =
 * =  ===  ===  =  ==  ====
 * =  ===  ===  =  ==  =  =
 * ==     =====   ====   ==
 * ========================
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * Copyright (c) 2020 Joe
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the organization nor the
 *    names of its contributors may be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY JOE ''AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL JOE BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * unixize: src/c_opts.c
 * 2020-11-02 23:37
 * Joe
 *
 * This is where we handle command line options.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "c_opts.h"
#include "c_unixize.h"

static void
c_dump_usage(void)
{
	dprintf(
		STDERR_FILENO,
		C_USAGE_FMT,
		C_OPTS
	);
}

static void
c_ask_confirm(const char dir[])
{
	char c;

	if (strncmp(dir, ".", 2 * sizeof(char)) == 0) {
		dprintf(STDERR_FILENO, "unixize current directory? (y/n [n]) ");
	}
	else {
		dprintf(STDERR_FILENO, "unixize %s? (y/n [n]) ", dir);
	}
	scanf("%c", &c);
	if (c != 'y' && c != 'Y') {
		dprintf(STDERR_FILENO, "not unixized\n");
		exit(0);
	}
}

void
c_get_opts
(struct opts_s*	opts,
 int			argc,
 const char*	argv[])
{
	int opt;

	opts->confirm = FALSE;
	opts->hidden = FALSE;
	opts->hyphen = FALSE;
	opts->pretend = FALSE;
	opts->recursive = FALSE;
	opts->verbose = FALSE;
	while ((opt = getopt(argc, (char *const *)argv, C_OPTS)) != -1) {
		if (opt == 'a') {
			opts->hidden = TRUE;
		}
		else if (opt == 'h') {
			c_dump_usage();
			exit(0);
		}
		else if (opt == 'i') {
			opts->confirm = TRUE;
		}
		else if (opt == 'n') {
			opts->hyphen = TRUE;
		}
		else if (opt == 'p') {
			opts->pretend = TRUE;
		}
		else if (opt == 'R') {
			opts->recursive = TRUE;
		}
		else if (opt == 'v') {
			opts->verbose = TRUE;
		}
		else if (opt == '?') {
			c_dump_usage();
			exit(1);
		}
	}
	if (optind < argc && argv[optind] != NULL) {
		strncpy(opts->dir, argv[optind], PATH_MAX);
		if (opts->dir[strlen(opts->dir) - 1] == '/') {
			opts->dir[strlen(opts->dir) - 1] = 0x00;
		}
	}
	else if (argv[optind] == NULL) {
		strncpy(opts->dir, ".", 2 * sizeof(char));
	}
	if (opts->confirm == TRUE) {
		c_ask_confirm(opts->dir);
	}
	if (opts->pretend == TRUE) {
		opts->verbose = TRUE;
	}
}
