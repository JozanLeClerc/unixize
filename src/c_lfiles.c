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
 * Copyright © 2020 Joe
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
 * unixize: src/c_lfiles.c
 * 2020-11-05 18:05
 * Joe
 *
 * In this files are functions to handle the files linked list.
 */

#include <sys/param.h>

#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "c_unixize.h"
#include "u_utils.h"

void
c_lfiles_add_back
(struct lfiles_s**	head,
 struct lfiles_s*	new)
{
	struct lfiles_s* tmp;

	if (*head == NULL) {
		*head = new;
		return;
	}
	tmp = *head;
	while (tmp->next != NULL) {
		tmp = tmp->next;
	}
	tmp->next = new;
}

void
c_lfiles_clear(struct lfiles_s** head)
{
	struct lfiles_s* renext;
	struct lfiles_s* tmp;

	if (head == NULL) {
		return;
	}
	tmp = *head;
	while (tmp != NULL) {
		renext = tmp->next;
		u_memdel((void*)&tmp->filename);
		u_memdel((void*)&tmp);
		tmp = renext;
	}
	*head = NULL;
}

struct lfiles_s*
c_lfiles_new
(const char		filename[],
 unsigned char	filetype)
{
	struct lfiles_s* link;

	link = (struct lfiles_s*)malloc(sizeof(struct lfiles_s));
	if (link == NULL) {
		return (NULL);
	}
	link->filename = strdup(filename);
	if (link->filename == NULL) {
		u_memdel((void*)&link);
		return (NULL);
	}
	link->filetype = filetype;
	link->next = NULL;
	return (link);
}

struct lfiles_s*
c_lfiles_gather(void)
{
	DIR* dirp;
	struct dirent* dp;
	struct lfiles_s* head;
	struct lfiles_s* link;

	head = NULL;
	link = NULL;
	dirp = opendir(".");
	if (dirp == NULL) {
		u_dump_errno();
		return (NULL);
	}
	while ((dp = readdir(dirp)) != NULL) {
		if (
			strncmp(dp->d_name, ".", 2 * sizeof(char)) == 0 ||
			strncmp(dp->d_name, "..", 3 * sizeof(char)) == 0
		) {
			continue;
		}
		link = c_lfiles_new(dp->d_name, dp->d_type);
		if (link == NULL) {
			u_dump_errno();
			c_lfiles_clear(&head);
			return (NULL);
		}
		c_lfiles_add_back(&head, link);
	}
	closedir(dirp);
	return (head);
}
