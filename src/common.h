/* ResidualVM - A 3D game interpreter
 *
 * ResidualVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the AUTHORS
 * file distributed with this source distribution.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */

#ifndef _COMMON_H_
#define _COMMON_H_

#ifndef _WIN32
#include "config.h"
#endif

#ifdef MACOSX
#define UNIX
#endif

#define NUM_MAX_CAMERA_IN_ROOM 20

#define HAS_YM3812 1

#ifndef USE_GL
#define USE_GL
#endif

#ifdef HAVE_CONFIG_H
#undef HAVE_CONFIG_H
#include "common/endian.h"
#define HAVE_CONFIG_H
#else
#include "common/endian.h"
#endif

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#ifdef WIN32
#include <search.h>
#endif

#ifdef _WIN32
#include <windows.h>
#include <assert.h>
#endif

#ifdef _DEBUG
#define ASSERT(exp) assert(exp)
#else
#define ASSERT(exp)
#endif

#ifdef _DEBUG
#define ASSERT_PTR(exp) assert(exp)
#else
#define ASSERT_PTR(exp)
#endif

#ifdef USE_GL
#include <GL/gl.h>      // Header File For The OpenGL32 Library
#include <GL/glu.h>     // Header File For The GLu32 Library
//#include <gl\glaux.h>   // Header File For The Glaux Library
#endif

#include "SDL.h"

//////////////// GAME SPECIFIC DEFINES

//#define NUM_MAX_OBJ         300
#define NUM_MAX_ACTOR       50
#define NUM_MAX_TEXT        40
#define NUM_MAX_MESSAGE     5
#define INVENTORY_SIZE      30


// 250
#define NUM_MAX_TEXT_ENTRY  1000

//////////////////

#include "cvars.h"

//////////////////
#define TYPE_MASK 0x1D1

#define ANIM_ONCE             0
#define ANIM_REPEAT           1
#define ANIM_UNINTERRUPTABLE  2
#define ANIM_RESET            4

// temp triangulation stuff
//#include <bool.h>
//#include <geometry.h>
int triangulate_polygon(int ncontours, int cntr[], double(*vertices)[2], int (*triangles)[3]);
#include "room.h"
#include "vars.h"
#include "main.h"
#include "file_access.h"
#include "screen.h"
#include "video_mode.h"
#include "pak.h"
#include "unpack.h"
#include "tatou.h"
#include "thread_code.h"
#include "renderer.h"
#include "input.h"
#include "version.h"
#include "cos_table.h"
#include "hqr.h"
#include "ourtime.h"
#include "font.h"
#include "aitd_box.h"
#include "save.h"
#include "anim.h"
#include "anim_action.h"
#include "actor_list.h"
#include "main_loop.h"
#include "inventory.h"
#include "startup_menu.h"
#include "system_menu.h"
#include "floor.h"
#include "object.h"
#include "zv.h"
#include "music.h"
#include "fmopl.h"

// debugger
#ifdef INTERNAL_DEBUGGER
#include "debugger.h"
#endif

// scripting
#include "track.h"
#include "life.h"
#include "life_2.h"
#include "eval_var.h"

////

// Disable byte-swapping for now.
#ifdef BIG_ENDIAN
#undef BIG_ENDIAN
#endif

FORCEINLINE uint16 READ_BE_U16(void *ptr) {
#ifdef BIG_ENDIAN
	return *(u16 *)ptr;
#else
	return (((uint8 *)ptr)[1] << 8) | ((uint8 *)ptr)[0];
#endif
}

FORCEINLINE int16 READ_BE_S16(void *ptr) {
	return (int16)READ_BE_S16(ptr);
}

FORCEINLINE uint32 READ_BE_U32(void *ptr) {
#ifdef BIG_ENDIAN
	return *(u32 *)ptr;
#else
	return (((uint8 *)ptr)[3] << 24) | (((uint8 *)ptr)[2] << 16) | (((uint8 *)ptr)[1] << 8) | ((uint8 *)ptr)[0];
#endif
}
#include "common/forbidden.h"
#endif
