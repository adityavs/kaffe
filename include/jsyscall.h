/*
 * jsyscall.h
 * Define the Java-to-system call interface.
 *
 * Copyright (c) 1996, 1997
 *	Transvirtual Technologies, Inc.  All rights reserved.
 *
 * See the file "license.terms" for information on usage and redistribution 
 * of this file. 
 */

#ifndef __jsyscall_h
#define __jsyscall_h

struct sockaddr;
struct timeval;
struct fd_set;
struct stat;
struct hostent;

/*
 * Thread-safe variants of several POSIXy calls.
 *
 * Output values, where given, are returned by reference as the last parameter.
 * All functions return zero on success, otherwise a non-zero value is 
 * returned which corresponds to errno.
 */
typedef struct SystemCallInterface {

	int	(*_open)(const char*, int, int, int *);
	int	(*_read)(int, void*, size_t, ssize_t *);
	int	(*_write)(int, const void*, size_t, ssize_t *);
	int	(*_lseek)(int, off_t, int, off_t *);
	int	(*_close)(int);
	int	(*_fstat)(int, struct stat*);
	int	(*_stat)(const char*, struct stat*);

	int	(*_mkdir)(const char*, int);
	int	(*_rmdir)(const char*);
	int	(*_rename)(const char*, const char*);
	int	(*_remove)(const char*);

	int	(*_socket)(int, int, int, int*);
	int	(*_connect)(int, struct sockaddr*, int, int timeout);
	int	(*_bind)(int, struct sockaddr*, int);
	int	(*_listen)(int, int);
	int	(*_accept)(int, struct sockaddr*, int*, int timeout, int*);
	int	(*_sockread)(int, void*, size_t, int timeout, ssize_t *);
	int	(*_recvfrom)(int,void*,size_t,int,struct sockaddr*, int*,
		    int timeout, ssize_t *);
	int	(*_sockwrite)(int, const void*, size_t, ssize_t *);
	int 	(*_sendto)(int,const void*,size_t,int,const struct sockaddr*,
		    int, ssize_t *);
	int	(*_setsockopt)(int, int, int, const void*, int);
	int	(*_getsockopt)(int, int, int, void*, int*);
	int	(*_getsockname)(int, struct sockaddr*, int*);
	int	(*_getpeername)(int, struct sockaddr*, int*);
	int	(*_sockclose)(int);
	int	(*_gethostbyname)(const char*, struct hostent**);
	int	(*_gethostbyaddr)(const char*, int, int, struct hostent**);

	int	(*_select)(int, fd_set*, fd_set*, fd_set*, struct timeval*, 
		int*);
	int	(*_forkexec)(char**, char**, int[4], int*, const char*);
	int	(*_waitpid)(int, int*, int, int*);
	int	(*_kill)(int, int);

} SystemCallInterface;

extern SystemCallInterface Kaffe_SystemCallInterface;

/*
 * Define some convenience macros
 */

#define	KOPEN(A,B,C,D)	(*Kaffe_SystemCallInterface._open)(A,B,C,D)
#define	KREAD(A,B,C,D)	(*Kaffe_SystemCallInterface._read)(A,B,C,D)
#define	KWRITE(A,B,C,D)	(*Kaffe_SystemCallInterface._write)(A,B,C,D)
#define	KLSEEK(A,B,C,D)	(*Kaffe_SystemCallInterface._lseek)(A,B,C,D)
#define	KCLOSE(A)	(*Kaffe_SystemCallInterface._close)(A)
#define	KFSTAT(A,B)	(*Kaffe_SystemCallInterface._fstat)(A,B)
#define	KSTAT(A,B)	(*Kaffe_SystemCallInterface._stat)(A,B)

#define	KMKDIR(A,B)	(*Kaffe_SystemCallInterface._mkdir)(A,B)
#define	KRMDIR(A)	(*Kaffe_SystemCallInterface._rmdir)(A)
#define	KRENAME(A,B)	(*Kaffe_SystemCallInterface._rename)(A,B)
#define	KREMOVE(A)	(*Kaffe_SystemCallInterface._remove)(A)

#define	KSOCKET(A,B,C,D) \
			(*Kaffe_SystemCallInterface._socket)(A,B,C,D)
#define	KCONNECT(A,B,C,D) \
			(*Kaffe_SystemCallInterface._connect)(A,B,C,D)
#define	KBIND(A,B,C)	(*Kaffe_SystemCallInterface._bind)(A,B,C)
#define	KLISTEN(A,B)	(*Kaffe_SystemCallInterface._listen)(A,B)
#define	KACCEPT(A,B,C,D,E) \
			(*Kaffe_SystemCallInterface._accept)(A,B,C,D,E)
#define	KSOCKREAD(A,B,C,D,E) \
			(*Kaffe_SystemCallInterface._sockread)(A,B,C,D,E)
#define	KRECVFROM(A,B,C,D,E,F,G,H) \
			(*Kaffe_SystemCallInterface._recvfrom)(A,B,C,D,E,F,G,H)
#define	KSOCKWRITE(A,B,C,D) \
			(*Kaffe_SystemCallInterface._sockwrite)(A,B,C,D)
#define	KSENDTO(A,B,C,D,E,F,G) \
			(*Kaffe_SystemCallInterface._sendto)(A,B,C,D,E,F,G)
#define	KSETSOCKOPT(A,B,C,D,E) \
			(*Kaffe_SystemCallInterface._setsockopt)(A,B,C,D,E)
#define	KGETSOCKOPT(A,B,C,D,E) \
			(*Kaffe_SystemCallInterface._getsockopt)(A,B,C,D,E)
#define	KGETSOCKNAME(A,B,C) \
			(*Kaffe_SystemCallInterface._getsockname)(A,B,C)
#define	KGETPEERNAME(A,B,C) \
			(*Kaffe_SystemCallInterface._getpeername)(A,B,C)
#define KGETHOSTBYNAME(A,B) \
			(*Kaffe_SystemCallInterface._gethostbyname)(A,B)
#define KGETHOSTBYADDR(A,B,C,D) \
			(*Kaffe_SystemCallInterface._gethostbyaddr)(A,B,C,D)
#define KSOCKCLOSE(A)	(*Kaffe_SystemCallInterface._sockclose)(A)

#define	KSELECT(A,B,C,D,E,F) \
			(*Kaffe_SystemCallInterface._select)(A,B,C,D,E,F)

#define	KWAITPID(A,B,C,D) \
			(*Kaffe_SystemCallInterface._waitpid)(A,B,C,D)
#define	KFORKEXEC(A,B,C,D,E) \
			(*Kaffe_SystemCallInterface._forkexec)(A,B,C,D,E)
#define	KKILL(A,B)	(*Kaffe_SystemCallInterface._kill)(A,B)

#endif
