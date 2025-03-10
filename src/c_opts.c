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

#include <sys/param.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "c_opts.h"
#include "c_unixize.h"
#include "u_utils.h"

static void
c_dump_usage(void)
{
	dprintf(STDERR_FILENO, C_USAGE_FMT);
}

static bool_t
c_prompt_confirm(void)
{
	char c;
	char first;
	char path[MAXPATHLEN];

	if (getcwd(path, MAXPATHLEN) == NULL) {
		u_dump_errno();
		dprintf(STDERR_FILENO, "unixize current directory? (y/n [n]) ");
	}
	else {
		dprintf(STDERR_FILENO, "unixize %s? (y/n [n]) ", path);
	}
	c = getchar();
	first = c;
	while (c != '\n' && c != EOF) {
		c = getchar();
	}
	if (first != 'y' && first != 'Y') {
		dprintf(STDERR_FILENO, "not unixized\n");
		return (FALSE);
	}
	return (TRUE);
}

static void
c_recursive_parse
(struct opts_s*	opts,
 const char*	argv[])
{
	char** ptr;

	ptr = (char**)argv + 1;
	while (*ptr != NULL) {
		if ((*ptr)[0] == 'a') {
			opts->hidden = TRUE;
		}
		else if ((*ptr)[0] == 'i') {
			opts->confirm = TRUE;
		}
		else if ((*ptr)[0] == 'k') {
			opts->preserve = TRUE;
		}
		else if ((*ptr)[0] == 'n') {
			opts->hyphen = TRUE;
		}
		else if ((*ptr)[0] == 'p') {
			opts->pretend = TRUE;
		}
		else if ((*ptr)[0] == 'r') {
			opts->rverbose = TRUE;
		}
		else if ((*ptr)[0] == 'R') {
			opts->recursive = TRUE;
		}
		else if ((*ptr)[0] == 'v') {
			opts->verbose = TRUE;
		}
		else if ((*ptr)[0] == 'e') {
			if ((*ptr)[1] == '1') {
				opts->cxx = 1;
			}
			else if ((*ptr)[1] == '2') {
				opts->cxx = 2;
			}
		}
		ptr++;
	}
}

bool_t
c_get_opts
(struct opts_s*	opts,
 int			argc,
 const char*	argv[])
{
	int opt;
	bool_t ret;

	opts->confirm = FALSE;
	opts->hidden = FALSE;
	opts->hyphen = FALSE;
	opts->preserve = FALSE;
	opts->pretend = FALSE;
	opts->rverbose = FALSE;
	opts->recursive = FALSE;
	opts->verbose = FALSE;
	opts->cxx = 0;
	while (
			argv[0][0] != C_RECURSIVE_CHAR &&
			(opt = getopt(argc, (char *const *)argv, C_OPTS)) != -1
		) {
		if (opt == 'a') {
			opts->hidden = TRUE;
		}
		else if (opt == 'h') {
			c_dump_usage();
			exit(EXIT_SUCCESS);
		}
		else if (opt == 'i') {
			opts->confirm = TRUE;
		}
		else if (opt == 'k') {
			opts->preserve = TRUE;
		}
		else if (opt == 'n') {
			opts->hyphen = TRUE;
		}
		else if (opt == 'p') {
			opts->pretend = TRUE;
		}
		else if (opt == 'r') {
			opts->rverbose = TRUE;
		}
		else if (opt == 'R') {
			opts->recursive = TRUE;
		}
		else if (opt == 'v') {
			opts->verbose = TRUE;
		}
		else if (opt == 'e') {
			if (u_ischarset(optarg[0], "012") == FALSE) {
				dprintf(
					STDERR_FILENO,
					C_C_OPT_FMT,
					optarg
				);
				exit(EXIT_FAILURE);
			}
			else {
				opts->cxx = optarg[0] - 48;
			}
		}
		else if (opt == '?') {
			c_dump_usage();
			exit(EXIT_FAILURE);
		}
	}
	if (
		argv[0][0] != C_RECURSIVE_CHAR &&
		argv[optind] == NULL
		) {
		c_dump_usage();
		exit(EXIT_FAILURE);
	}
	if (
		argv[0][0] != C_RECURSIVE_CHAR &&
		optind < argc &&
		argv[optind] != NULL
		) {
		strncpy(opts->dir, argv[optind], PATH_MAX);
		if (opts->dir[strlen(opts->dir) - 1] == '/') {
			opts->dir[strlen(opts->dir) - 1] = 0x00;
		}
	}
	else if (argv[0][0] == C_RECURSIVE_CHAR) {
		strlcpy(opts->dir, ".", 2 * sizeof(char));
		c_recursive_parse(opts, argv);
	}
	ret = TRUE;
	if (opts->confirm == TRUE) {
		ret = c_prompt_confirm();
	}
	return (ret);
}
