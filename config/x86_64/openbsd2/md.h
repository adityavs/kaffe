/*
 * amd64/openbsd2/md.h
 * OpenBSD amd64 configuration information.
 *
 * Copyright (c) 2006
 *      Kaffe.org contributors. See ChangeLog for details.
 *
 * See the file "license.terms" for information on usage and redistribution 
 * of this file. 
 */

#ifndef __amd64_openbsd2_md_h
#define __amd64_openbsd2_md_h

#include "x86_64/common.h"
#include "x86_64/threads.h"

#if defined(HAVE_SYS_TIME_H)
#include <sys/time.h>
#endif
#if defined(HAVE_SYS_RESOURCE_H)
#include <sys/resource.h>
#endif
#if defined(HAVE_UNISTD_H)
#include <unistd.h>
#endif

#undef	SP_OFFSET
#define	SP_OFFSET	2
#undef  FP_OFFSET
#define FP_OFFSET	3

/* Define signal context macros for xprofiling */
#define SIGNAL_ARGS(sig, sc) int sig, int __code, struct sigcontext *sc
#define SIGNAL_CONTEXT_POINTER(scp) struct sigcontext *scp
#define GET_SIGNAL_CONTEXT_POINTER(sc) (sc)
#define SIGNAL_PC(scp) (scp)->sc_rip
#define STACK_POINTER(scp) (scp)->sc_rsp

#if defined(TRANSLATOR)
#include "jit-md.h"
#endif

#if defined(KAFFE_SYSTEM_UNIX_PTHREADS)

#define KAFFEMD_STACK_ERROR 0
#define KAFFEMD_STACK_INFINITE KAFFEMD_STACK_ERROR

#define KAFFEMD_STACKSIZE
extern size_t mdGetStackSize(void);

/* this is only used for the main thread and is ok for that */
/* this may change with rthreads when thats done */
static inline void mdSetStackSize(rlim_t limit)
{
  struct rlimit rl;

  getrlimit(RLIMIT_STACK, &rl);
  rl.rlim_cur = limit;
  setrlimit(RLIMIT_STACK, &rl);
}

#define KAFFEMD_STACKEND
extern void *mdGetStackEnd(void);

#else /* KAFFE_SYSTEM_UNIX_PTHREADS */

#include "kaffe-unix-stack.h"

#endif /* KAFFE_SYSTEM_UNIX_PTHREADS */

#endif
