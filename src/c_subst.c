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
 * unixize: src/c_subst.c
 * 2020-11-08 02:50
 * Joe
 *
 * This is were we subsitute the new names and return them into a
 * new lfiles_s linked list.
 */

#include <sys/param.h>

#include <ctype.h>
#include <stddef.h>
#include <string.h>

#include "c_lfiles.h"
#include "c_unixize.h"
#include "u_utils.h"

#include <stdio.h>
#include <unistd.h>

static void
c_ext_subst
(char					filename[],
 const unsigned char	cxx)
{
	const size_t len = strlen(filename);

	if (cxx == 0 || len < 4) {
		return;
	}
	if (
		filename[len - 4] == '.' &&
		filename[len - 3] == 't' &&
		filename[len - 2] == 'x' &&
		filename[len - 1] == 't'
	) {
		filename[len - 4] = 0x00;
	}
	else if (
		filename[len - 4] == '.' &&
		filename[len - 3] == 'a' &&
		filename[len - 2] == 's' &&
		filename[len - 1] == 'm'
	) {
		memcpy(filename + len - 3, "S", 2 * sizeof(char));
	}
	else if (
		filename[len - 4] == '.' &&
		filename[len - 3] == 'c' &&
		filename[len - 2] == 'p' &&
		filename[len - 1] == 'p'
	) {
		if (cxx == 1) {
			memcpy(filename + len - 3, "cc", 3 * sizeof(char));
		}
		else if (cxx == 2) {
			memcpy(filename + len - 3, "cxx", 4 * sizeof(char));
		}
	}
	else if (
		filename[len - 4] == '.' &&
		filename[len - 3] == 'h' &&
		filename[len - 2] == 'p' &&
		filename[len - 1] == 'p'
	) {
		if (cxx == 1) {
			memcpy(filename + len - 3, "hh", 3 * sizeof(char));
		}
		else if (cxx == 2) {
			memcpy(filename + len - 3, "hxx", 4 * sizeof(char));
		}
	}
}

static void
c_classic_subst
(char			filename[],
 const bool_t	hyphen)
{
	char *p;

	p = (char*)filename;
	while (*p != 0x00) {
		if (hyphen == TRUE && *p == '_') {
			*p = '-';
			c_classic_subst(filename, hyphen);
		}
		if (hyphen == FALSE && *p == '-') {
			*p = '_';
			c_classic_subst(filename, hyphen);
		}
		if (*p == ' ') {
			*p = (hyphen == FALSE) ? ('_') : ('-');
			c_classic_subst(filename, hyphen);
		}
		p++;
	}
}

static void
c_subst_current
(char					new_fname[MAXPATHLEN],
 const char				og_fname[],
 const bool_t			hyphen,
 const unsigned char	cxx)
{
	unsigned char* p;

	strlcpy(new_fname, og_fname, MAXPATHLEN);
	p = (unsigned char*)new_fname;
	while (*p != 0x00) {
		*p = tolower(*p);
		p++;
	}
	c_ext_subst(new_fname, cxx);
	/* c_unicode_subst(new_fname, hyphen); */
	/* c_exascii_subst(new_fname, hyphen); */
	c_classic_subst(new_fname, hyphen);
}

struct lfiles_s*
c_subst_filenames
(struct lfiles_s*		og_head,
 const bool_t			hyphen,
 const unsigned char	cxx)
{
	struct lfiles_s* dup_head;
	struct lfiles_s* link;
	struct lfiles_s* origin;
	char tmp[MAXPATHLEN];

	dup_head = NULL;
	link = NULL;
	origin = og_head;
	while (origin != NULL) {
		c_subst_current(tmp, origin->filename, hyphen, cxx);
		link = c_lfiles_new(tmp, origin->filetype);
		if (link == NULL) {
			u_dump_errno();
			c_lfiles_clear(&dup_head);
			return (NULL);
		}
		c_lfiles_add_back(&dup_head, link);
		origin = origin->next;
	}
	return (dup_head);
}
