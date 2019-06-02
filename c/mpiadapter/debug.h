#ifndef DEBUG_H              /* check if this file has been previously included */
#define DEBUG_H              /* if not, define symbol to prevent future inclusion */

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

/***********************************************
These need to be cleaned-up right now they are too specific and need to be more general
*************************************************/
typedef enum
{
	DEBUG_VERBOSE	= 	(1<<0),
	DEBUG_CONTROL	=	(1<<1),
	DEBUG_START	=	(1<<2),
	DEBUG_READER	=	(1<<3),
	DEBUG_TREE	=	(1<<4),
	DEBUG_WARNING	=	(1<<5),
	DEBUG_PLS 	=	(1<<6),
	DEBUG_SERVER	=	(1<<7),
	DEBUG_SPLIT	=	(1<<8),
	DEBUG_CORR	=	(1<<9),
	DEBUG_DB	=	(1<<10),
	// for server-side client (child) process debugging
	DEBUG_CLIENT	=	(1<<11),
	DEBUG_TA	=	(1<<12),
	DEBUG_ACCUM	= 	(1<<13),
	DEBUG_IB 	= 	(1<<14),
	DEBUG_WKFLOW	=	(1<<15),
	DEBUG_WKFLOWAPI	=	(1<<16),
	// for protocol adapter debugging
	DEBUG_PADAPT    =   	(1<<17),       
	// for farm applications
	DEBUG_MANAGER  = 	(1<<18),
	DEBUG_WORKER  = 	(1<<19),
	// collector
	DEBUG_COLLECTOR = 	(1<<20),
	DEBUG_RFLOW = 		(1<<21),
	DEBUG_COMM  =		(1<<22)      // added for debugging higher-level communication routines
}
DebugFlags;

#define WHERESTR "[file %d, line %d] "
#define WHEREARG __FILE__, __LINE__

/* dumpme(x,fmt) will dump the file,location and the value of variable x  as x=v */
#define dumpme(x, fmt) printf("%s:%u: %s=" fmt, __FILE__, __LINE__, #x, x)

/* can be used with MPI or -DNOMPI  */
#define mpibail(x, fmt, ...) mpibail_def(x, __FILE__, __LINE__, fmt, ## __VA_ARGS__)
#define mpibailstr(x, fmt, ...) mpibailstr_def(x, __FILE__, __LINE__, fmt, ## __VA_ARGS__)

/* can only be used with MPI */
#define mpierror(x, y) mpierror_def(x, y, __FILE__, __LINE__)



/* DEBUG_C defined in debug.c, ensures that only its  own C file creates these variables */
#ifndef DEBUG_C            
extern int G_debug_level;
extern FILE G_debug_file;
#endif

void set_debug_level(long int p_dblevel);
void set_debug_output(FILE *p_dbfile);

void mpibail_def(int who, const char* file, int where, const char *format, ...);
void mpibailstr_def(char *who, const char* file, int where, const char *format, ...);

#if defined(NODEBUG) && defined(__GNUC__)
/* gcc's cpp has extensions; it allows for macros with a variable number of
   arguments. We use this extension here to preprocess pmesg away. */
#define debug(level, format, ...) ((void)0)
#define debug_r(level, format, ...) ((void)0)
#else
void debug(long int level, const char *format, ...);
void debug_r(long int level, const char *format, ...);
/* print a message, if it is considered significant enough.
      Adapted from [K&R2], p. 174 */
#endif


#endif  /* DEBUG_H */
