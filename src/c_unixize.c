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
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <unistd.h>

#include "c_lfiles.h"
#include "c_opts.h"
#include "c_unixize.h"
#include "u_utils.h"

int
main
(int			argc,
 const char*	argv[])
{
	struct lfiles_s* og_files;
	struct lfiles_s* new_files;
	struct lfiles_s* og_files_head;
	struct lfiles_s* new_files_head;
	struct opts_s opts;
	int nargc;
	char** nargv;
	static char subpath[MAXPATHLEN];

	if (c_get_opts(&opts, argc, argv) == FALSE) {
		return (0);
	}
	if (chdir((const char*)opts.dir) == -1) {
		u_dump_errno_path(opts.dir);
		return (1);
	}
	og_files = c_lfiles_gather();
	if (og_files == NULL) {
		return (0);
	}
	new_files = c_lfiles_duplicate(&og_files);
	if (og_files == NULL) {
		c_lfiles_clear(&og_files);
		return (1);
	}
	/* c_subst(&opts, og_files); */
	og_files_head = og_files;
	new_files_head = new_files;
	while (og_files != NULL && new_files != NULL) {
		if (opts.hidden == FALSE && og_files->filename[0] == '.') {
			og_files = og_files->next;
			new_files = new_files->next;
			continue;
		}
		if (opts.recursive == TRUE && og_files->filetype == DT_DIR) {
			if (chdir((const char*)og_files->filename) == -1) {
				u_dump_errno_path(og_files->filename);
			}
			else {
				nargv = u_get_nargv(&opts);
				if (nargv != NULL) {
					nargc = 0;
					while (argv[nargc + 1] != NULL) {
						nargc++;
					}
					main(nargc, (const char**)nargv);
					u_del_nargv(nargv);
				}
				chdir("../");
			}
		}
		if (opts.verbose == TRUE) {
			dprintf(
				STDOUT_FILENO,
				"'%s' -> '%s'\n",
				og_files->filename,
				new_files->filename
		   );
		}
		/* rename(); */
		og_files = og_files->next;
		new_files = new_files->next;
	}
	c_lfiles_clear(&og_files_head);
	c_lfiles_clear(&new_files_head);
	return (0);
}

/*
 * Files prefixes index
 * --------------------
 * c_  -> core program related
 * u_  -> utils related
 */
