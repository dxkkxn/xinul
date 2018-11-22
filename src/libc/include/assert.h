/*
 * debug.h
 *
 * Copyright (C) 2003 Simon Nieuviarts
 *
 * Functions to help writing on the screen.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version. 
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef __DEBUG_H__
#define __DEBUG_H__

#include "stdio.h"

extern void panic(const char *fmt, ...) __attribute__((noreturn, format (printf, 1, 2)));

#define BUG() do { panic(__FILE__":%u: BUG !\n", __LINE__); } while (0)

#ifdef NDEBUG

#define assert(expr) ((void)0)

#else

#define assert(expr) \
	((void)((expr) ? 0 : \
		(panic(__FILE__":%u: failed assertion `"#expr"'\n", \
			__LINE__), 0)))

#endif

#endif
