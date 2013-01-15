/* error.h
 * Copyright (C) 2004 Mathieu Guindon, Julien Keable
 * This file is part of Drone.
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
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

// Copyright (C) 2004 Jean-Sébastien Senécal (senecal@iro.umontreal.ca)
// Copyright (C) 2004 Ronan Collobert (collober@idiap.ch)
//                
// This file is part of Drone.
//
// All rights reserved.
// 
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
// 1. Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
// 3. The name of the author may not be used to endorse or promote products
//    derived from this software without specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
// IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
// OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
// IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
// INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
// NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
// THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#ifndef ERROR_INCLUDED
#define ERROR_INCLUDED

#include <stdarg.h>

// # Macro definitions #####################################################

// Debug levels.

//! No debug.
#define DEBUG_LEVEL_NODEBUG -1

//! Only catch errors.
#define DEBUG_LEVEL_ERROR   0

//! Catch both errors and warnings.
#define DEBUG_LEVEL_WARNING 1

//! Catch errors, warnings and notices.
#define DEBUG_LEVEL_NOTICE  2


// Default debug level.
#ifndef DEBUG_LEVEL
#define DEBUG_LEVEL DEBUG_LEVEL_ERROR
#endif

// Quick condition checkers.
#define DEBUG_ERROR   (DEBUG_LEVEL >= DEBUG_LEVEL_ERROR)
#define DEBUG_WARNING (DEBUG_LEVEL >= DEBUG_LEVEL_WARNING)
#define DEBUG_NOTICE  (DEBUG_LEVEL >= DEBUG_LEVEL_NOTICE)

// Redefine __STRING just to make sure.
#ifndef __STRING
#define __STRING(x) #x
#endif

// This macro is absolutely NOT SAFE! NEVER USE IT ALONE!!!
// Use ASSERT_ERROR, ASSERT_WARNING and ASSERT_NOTICE instead, below.
#define __TRIGGER_ASSERT(expr, func) \
((expr) ? static_cast<void>(0) : func("assertion failed : " __STRING(expr)) );
#define __DUMMY_ASSERT (static_cast<void>(0));

//#define dummymsg static_cast<void>(0);

// # Message functions #####################################################

// Dummy methods (used for empty macros, see up there).
inline void dummymsg(const char* , ...) {}
inline void assertdummymsg(bool, const char* , ...) {}

//! Prints a message.
void message(const char* msg, ...);

//! Like printf.
void print(const char* msg, ...);

//! Prints an error message. The program will exit.
void errormsg(const char* msg, ...);

//! Prints a warning message. The program will not.
void warningmsg(const char* msg, ...);

//! Prints a notice, usually intended for deep information at the programmer's intent.
void noticemsg(const char* msg, ...);

// # Assertions and messages functions #####################################

//! Error messages/assertion.
#if DEBUG_ERROR
// XXX This was conflicting...
//#define ERROR errormsg
#define error errormsg
void ASSERT_ERROR_MESSAGE(bool expr, const char* msg, ...);
#define ASSERT_ERROR(expr) __TRIGGER_ASSERT(expr, errormsg)
#else
#define error dummymsg
#define ASSERT_ERROR(expr) __DUMMY_ASSERT
#define ASSERT_ERROR_MESSAGE assertdummymsg
#endif

//! Warning messages/assertion.
#if DEBUG_WARNING
#define WARNING warningmsg
void ASSERT_WARNING_MESSAGE(bool expr, const char* msg, ...);
#define ASSERT_WARNING(expr) __TRIGGER_ASSERT(expr, warningmsg)
#else
#define WARNING dummymsg
#define ASSERT_WARNING(expr) __DUMMY_ASSERT
#define ASSERT_WARNING_MESSAGE assertdummymsg
#endif

//! Notice messages/assertion.
#if DEBUG_NOTICE
#define NOTICE noticemsg
void ASSERT_NOTICE_MESSAGE(bool expr, const char* msg, ...);
#define ASSERT_NOTICE(expr) __TRIGGER_ASSERT(expr, noticemsg)
#else
#define NOTICE dummymsg
#define ASSERT_NOTICE(expr) __DUMMY_ASSERT
#define ASSERT_NOTICE_MESSAGE assertdummymsg
#endif


#endif
