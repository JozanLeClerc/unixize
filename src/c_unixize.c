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
 * unixize: src/c_unixize.c
 * 2020-11-02 22:14
 * Joe
 *
 * This is the main function and entrypoint of the program.
 */

#include <sys/param.h>

#include <dirent.h>
#include <errno.h>
#include <locale.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <unistd.h>

#include "c_lfiles.h"
#include "c_opts.h"
#include "c_subst.h"
#include "c_unixize.h"
#include "u_utils.h"

int
main
(int			argc,
 const char*	argv[])
{
	struct lfiles_s* new_files;
	struct lfiles_s* new_files_head;
	struct lfiles_s* og_files;
	struct lfiles_s* og_files_head;
	struct opts_s opts;
	int nargc;
	int tmp;
	char** nargv;
	char og_file[MAXPATHLEN];
	char new_file[MAXPATHLEN];
	static int ret = 0;
	static char path[MAXPATHLEN] = "";

	if (c_get_opts(&opts, argc, argv) == FALSE) {
		return (0);
	}
	if (
		argv[0][0] != 'r'
		) {
		strlcpy(path, opts.dir, MAXPATHLEN - 1);
		path[strlen(path) + 1] = 0x00;
		path[strlen(path)] = '/';
	}
	og_files = c_lfiles_gather(path);
	if (og_files == NULL) {
		return (0);
	}
	new_files = c_subst_filenames(
		og_files,
		opts.hyphen,
		opts.preserve,
		opts.cxx
		);
	if (new_files == NULL) {
		c_lfiles_clear(&og_files);
		return (1);
	}
	og_files_head = og_files;
	new_files_head = new_files;
	while (og_files != NULL && new_files != NULL) {
		if (opts.hidden == FALSE && og_files->filename[0] == '.') {
			og_files = og_files->next;
			new_files = new_files->next;
			continue;
		}
		if (opts.recursive == TRUE && og_files->filetype == DT_DIR) {
			u_inc_path(path, og_files->filename);
			nargv = u_get_nargv(&opts);
			if (nargv != NULL) {
				nargc = 0;
				while (argv[nargc + 1] != NULL) {
					nargc++;
				}
				tmp = main(nargc, (const char**)nargv);
				ret = (tmp != 0) ? (tmp) : (ret);
				u_del_nargv(nargv);
			}
			u_dec_path(path);
		}
		if (
			strncmp(
				og_files->filename,
				new_files->filename,
				strlen(new_files->filename) + 1
				) != 0 &&
			new_files->filename[0] != 0x00
			) {
			if (opts.verbose == TRUE) {
				dprintf(
					STDOUT_FILENO,
					"'%s%s' -> '%s%s'\n",
					path,
					og_files->filename,
					path,
					new_files->filename
					);
			}
			if (opts.pretend == FALSE) {
				sprintf(og_file, "%s%s", path, og_files->filename);
				sprintf(new_file, "%s%s", path, new_files->filename);
				if (rename(og_file, new_file) == -1) {
					dprintf(
						STDERR_FILENO,
						"unixize: rename %s to %s: %s\n",
						og_files->filename,
						new_files->filename,
						strerror(errno)
						);
					ret = 2;
				}
			}
		}
		else if (opts.rverbose == TRUE) {
			dprintf(
				STDOUT_FILENO,
				"Untouched: '%s%s'\n",
				path,
				og_files->filename
				);
		}
		og_files = og_files->next;
		new_files = new_files->next;
	}
	c_lfiles_clear(&og_files_head);
	c_lfiles_clear(&new_files_head);
	return (ret);
}

/*
 * Files prefixes index
 * --------------------
 * c_  -> core program related
 * u_  -> utils related
 */
