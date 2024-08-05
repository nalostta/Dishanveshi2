#ifndef __dbg_h__
#define __dbg_h__

#include <stdio.h>
#include <errno.h>
#include <string.h>

#ifdef NDEBUG
#define debug(M, ...)
#else
#define debug(M, ...) fprintf(stderr, "\nDEBUG %s:%d: " M "\n", __FILE__, __LINE__, ##__VA_ARGS__)
#endif

#define clean_errno() (errno == 0 ? "None" : strerror(errno))

#define log_err(M, ...) fprintf(stderr, "[ERROR] (%s:%d: errno: %s) " M "\n", __FILE__, __LINE__, clean_errno(), ##__VA_ARGS__)

#define log_warn(M, ...) fprintf(stderr, "[WARN] (%s:%d: errno: %s) " M "\n", __FILE__, __LINE__, clean_errno(), ##__VA_ARGS__)

#define log_info(M, ...) fprintf(stderr, "[INFO] (%s:%d) " M "\n", __FILE__, __LINE__, ##__VA_ARGS__)

#define check(A, M, ...) if(!(A)) { log_err(M, ##__VA_ARGS__); errno=0; goto error; }

#define sentinel(M, ...)  { log_err(M, ##__VA_ARGS__); errno=0; goto error; }

#define check_mem(A) check((A), "Out of memory.")

#define check_debug(A, M, ...) if(!(A)) { debug(M, ##__VA_ARGS__); errno=0; goto error; }

#endif

/*
->handling of errors is a challenge in every programming language.Each language adopts it's own strategy to tackle this.
->Some implement complex control structures like exceptions to pass error conditions around
->C tackles this by returning error codes and setting a global errno values which can be checked
*/

/*
Q.what is 'stderr'?
>Stderr, also known as standard error, is the default file descriptor where a process can write error messages. 
	In computer programming, standard streams are preconnected input and output communication channels between a
	computer program and its environment when it begins execution.The three input/output (I/O) connections are called
	standard input (stdin), standard output (stdout) and standard error (stderr).
	
Q.what is 'errno'?
>errno.h is a header file in the standard library of the C programming language. 	
	It defines macros for reporting and retrieving error conditions through error 
	codes stored in a static memory location called errno.
	
Q.what is strerror()?
>The strerror() function returns the appropriate error description string, or an unknown error message if the error code is unknown. 
	The value of errno is not changed for a successful call, and is set to a nonzero value upon error.
*/