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
 * unixize: src/c_subst.h
 * 2020-11-08 02:48
 * Joe
 *
 * This is the main function and entrypoint of the program.
 */

#ifndef __C_SUBST_H__
#define __C_SUBST_H__

#include "c_unixize.h"

#define C_CHARSET_VALID	"_-."
#define C_CHARSET_A \
	"\x80\x81\x82\x83\x84\xa0\xa1\xa2\xa3\xa4"
#define C_CHARSET_AA \
	"\x85\xa5"
#define C_CHARSET_O \
	"\x92\x93\x94\x95\x96\x98\xb2\xb3\xb4\xb5\xb6\xb8"
#define C_CHARSET_AE \
	"\x86\xa6"
#define C_CHARSET_N \
	"\x91\xb1"
#define C_CHARSET_DH \
	"\x90\xb0"
#define C_CHARSET_TH \
	"\x9e\xbe"
#define C_CHARSET_C \
	"\x87\xa7"
#define C_CHARSET_E \
	"\x88\x89\x8a\x8b\xa8\xa9\xaa\xab"
#define C_CHARSET_U \
	"\x99\x9a\x9b\x9c\xb9\xba\xbb\xbc"
#define C_CHARSET_I \
	"\x8c\x8d\x8e\x8f\xac\xab\xac\xad"
#define C_CHARSET_Y \
	"\x9d\xbd\xbf"

struct lfiles_s* c_subst_filenames(struct lfiles_s*, const bool_t,
	const bool_t, const unsigned char);

#endif /* ifndef __C_SUBST_H__ */
