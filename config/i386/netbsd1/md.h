/*
 * i386/netbsd1/md.h
 * NetBSD i386 configuration information.
 *
 * Copyright (c) 1996, 1997
 *	Transvirtual Technologies, Inc.  All rights reserved.
 *
 * Copyright (c) 2003, 2004
 *      Kaffe.org contributors. See ChangeLog for details.
 *
 * See the file "license.terms" for information on usage and redistribution 
 * of this file. 
 */

#ifndef __i386_netbsd1_md_h
#define __i386_netbsd1_md_h

#include "i386/common.h"
#include "i386/sysdepCallMethod.h"
#include "i386/threads.h"

#if defined(HAVE_SYS_RESOURCE_H)
#include <sys/resource.h>
#endif
#if defined(HAVE_UNISTD_H)
#include <unistd.h>
#endif

/*
 * Redefine stack pointer offset.
 */
#undef SP_OFFSET
#define SP_OFFSET 2

#define SIGNAL_ARGS(sig, sc) int sig, int __code, struct sigcontext *sc
#define SIGNAL_CONTEXT_POINTER(scp) struct sigcontext *scp
#define GET_SIGNAL_CONTEXT_POINTER(sc) (sc)
#define SIGNAL_PC(scp) (scp)->sc_pc
#define STACK_POINTER(scp) (scp)->sc_sp

#if defined(TRANSLATOR)
#include "jit-md.h"
#endif

#if defined(HAVE_GETRLIMIT)
#define KAFFEMD_STACKSIZE

static inline size_t mdGetStackSize()
{
  struct rlimit rl;

  if (getrlimit(RLIMIT_STACK, &rl) < 0)
    return -1;
  else
    return (rl.rlim_max >= RLIM_INFINITY) ? rl.rlim_cur : rl.rlim_max;
}
#endif

#endif

