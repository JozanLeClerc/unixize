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
#include "c_subst.h"
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
 const bool_t	hyphen,
 const bool_t	preserve)
{
	char *p;
	const char sep   = (hyphen == FALSE) ? ('_') : ('-');
	const char c_sep = (hyphen == FALSE) ? ('-') : ('_');

	p = (char*)filename;
	while (*p != 0x00) {
		if (*p == c_sep && preserve == FALSE) {
			*p = sep;
			c_classic_subst(filename, hyphen, preserve);
		}
		if (*p == sep && *(p + 1) == sep) {
			memmove(p, p + 1, (strlen(p + 1) + 1) * sizeof(char));
			c_classic_subst(filename, hyphen, preserve);
		}
		if (*p == '.' && *(p + 1) == '.') {
			memmove(p, p + 1, (strlen(p + 1) + 1) * sizeof(char));
			c_classic_subst(filename, hyphen, preserve);
		}
		if (*p == ' ') {
			*p = sep;
			c_classic_subst(filename, hyphen, preserve);
		}
		if (
			isalnum(*p) == 0 &&
			u_ischarset(*p, C_CHARSET_VALID) == FALSE
		) {
			memmove(p, p + 1, (strlen(p + 1) + 1) * sizeof(char));
			c_classic_subst(filename, hyphen, preserve);
		}
		p++;
	}
}

static void
c_num_prefix_subst
(char			filename[],
 const bool_t	hyphen)
{
	char *p;
	char *p_probe;
	const char sep = (hyphen == FALSE) ? ('_') : ('-');

	p = filename;
	while (*p != 0x00) {
		if (isdigit(*p) == 0) {
			if (*p != '.') {
				return;
			}
			else {
				p_probe = p + 1;
				while (*p_probe != 0x00 && *p_probe != '.') {
					p_probe++;
				}
				if (*p_probe != 0x00) {
					*p = sep;
				}
				return;
			}
		}
		p++;
	}
}

static void
c_specific_subst(char filename[])
{
	char *p;

	p = filename;
	while (*p != 0x00) {
		if ((*p == 'c' || *p == 'C')) {
			if (*(p + 1) == '+' && *(p + 2) == '+') {
				*(p) = 'c';
				*(p + 1) = 'x';
				*(p + 2) = 'x';
			}
		}
		p++;
	}
}

static void
c_unicode_subst(char filename[])
{
	unsigned char *p;

	p = (unsigned char*)filename;
	while (*p != 0x00) {
		if (*p == 0xc2) {
			if (*(p + 1) == 0xb9) {
				*p = '1';
				memmove(
					p + 1,
					p + 2,
					(strlen((const char*)p + 2) + 1) * sizeof(char)
				);
				c_unicode_subst(filename);
			}
			if (*(p + 1) == 0xb2) {
				*p = '2';
				memmove(
					p + 1,
					p + 2,
					(strlen((const char*)p + 2) + 1) * sizeof(char)
				);
				c_unicode_subst(filename);
			}
			if (*(p + 1) == 0xb3) {
				*p = '3';
				memmove(
					p + 1,
					p + 2,
					(strlen((const char*)p + 2) + 1) * sizeof(char)
				);
				c_unicode_subst(filename);
			}
		}
		if (*p == 0xc3) {
			if (u_isucharset((unsigned char)*(p + 1), C_CHARSET_A) == TRUE) {
				*p = 'a';
				memmove(
					p + 1,
					p + 2,
					(strlen((const char*)p + 2) + 1) * sizeof(char)
				);
				c_unicode_subst(filename);
			}
			if (u_isucharset((unsigned char)*(p + 1), C_CHARSET_O) == TRUE) {
				*p = 'o';
				memmove(
					p + 1,
					p + 2,
					(strlen((const char*)p + 2) + 1) * sizeof(char)
				);
				c_unicode_subst(filename);
			}
			if (u_isucharset((unsigned char)*(p + 1), C_CHARSET_AE) == TRUE) {
				*p = 'a';
				*(p + 1) = 'e';
				c_unicode_subst(filename);
			}
			if (u_isucharset((unsigned char)*(p + 1), C_CHARSET_N) == TRUE) {
				*p = 'n';
				memmove(
					p + 1,
					p + 2,
					(strlen((const char*)p + 2) + 1) * sizeof(char)
				);
				c_unicode_subst(filename);
			}
			if (u_isucharset((unsigned char)*(p + 1), C_CHARSET_DH) == TRUE) {
				*p = 'd';
				*(p + 1) = 'h';
				c_unicode_subst(filename);
			}
			if (u_isucharset((unsigned char)*(p + 1), C_CHARSET_C) == TRUE) {
				*p = 'c';
				memmove(
					p + 1,
					p + 2,
					(strlen((const char*)p + 2) + 1) * sizeof(char)
				);
				c_unicode_subst(filename);
			}
			if (u_isucharset((unsigned char)*(p + 1), C_CHARSET_E) == TRUE) {
				*p = 'e';
				memmove(
					p + 1,
					p + 2,
					(strlen((const char*)p + 2) + 1) * sizeof(char)
				);
				c_unicode_subst(filename);
			}
			if (u_isucharset((unsigned char)*(p + 1), C_CHARSET_U) == TRUE) {
				*p = 'u';
				memmove(
					p + 1,
					p + 2,
					(strlen((const char*)p + 2) + 1) * sizeof(char)
				);
				c_unicode_subst(filename);
			}
			if (u_isucharset((unsigned char)*(p + 1), C_CHARSET_I) == TRUE) {
				*p = 'i';
				memmove(
					p + 1,
					p + 2,
					(strlen((const char*)p + 2) + 1) * sizeof(char)
				);
				c_unicode_subst(filename);
			}
			if (u_isucharset((unsigned char)*(p + 1), C_CHARSET_Y) == TRUE) {
				*p = 'y';
				memmove(
					p + 1,
					p + 2,
					(strlen((const char*)p + 2) + 1) * sizeof(char)
				);
				c_unicode_subst(filename);
			}
			if (u_isucharset((unsigned char)*(p + 1), C_CHARSET_TH) == TRUE) {
				*p = 't';
				*(p + 1) = 'h';
				c_unicode_subst(filename);
			}
			if (*(p + 1) == 0x9f) {
				*p = 's';
				*(p + 1) = 'z';
				c_unicode_subst(filename);
			}
		}
		if (*p == 0xce) {
			if (*(p + 1) == 0xbc) {
				*p = 'u';
				memmove(
					p + 1,
					p + 2,
					(strlen((const char*)p + 2) + 1) * sizeof(char)
				);
				c_unicode_subst(filename);
			}
		}
		p++;
	}
}

static void
c_subst_current
(char					new_fname[],
 const char				og_fname[],
 const bool_t			hyphen,
 const bool_t			preserve,
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
	c_num_prefix_subst(new_fname, hyphen);
	c_specific_subst(new_fname);
	c_unicode_subst(new_fname);
	c_classic_subst(new_fname, hyphen, preserve);
}

struct lfiles_s*
c_subst_filenames
(struct lfiles_s*		og_head,
 const bool_t			hyphen,
 const bool_t			preserve,
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
		c_subst_current(tmp, origin->filename, hyphen, preserve, cxx);
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
