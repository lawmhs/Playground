#define DEBUG_C            /* define symbol to handshake with our H file */
#include "debug.h"

long int G_debug_level = 0;

// need some magic here for Linux (Mac is okay) with gcc
// to statically initialize this value
//FILE *G_debug_file = stderr;
FILE *G_debug_file;
static void G_debug_file_construct (void) __attribute__((constructor));
static void G_debug_file_construct (void) { G_debug_file = stdout; }

void set_debug_level(long int p_dblevel)
{
	G_debug_level = p_dblevel;
	return;
}

void set_debug_output(FILE *p_dbfile)
{
    if(p_dbfile == NULL)
		G_debug_file = stderr;
	else
        G_debug_file = p_dbfile;
    return;
}

#if defined(NODEBUG) && defined(__GNUC__)
/* Nothing. debug has been "defined away" in debug.h already. */
#else
void debug(long int level, const char* format, ...) {

#ifdef NODEBUG
	/* Empty body, so a good compiler will optimise calls to pmesg away */
#else
        va_list args;

        if (!(level & G_debug_level))
                return;

        va_start(args, format);
        vfprintf(G_debug_file, format, args);
        va_end(args);
#endif /* NODEBUG */
}
#endif /* NODEBUG && __GNUC__ */

#if defined(NODEBUG) && defined(__GNUC__)
/* Nothing. debug has been "defined away" in debug.h already. */
#else
void debug_r(long int level, const char* format, ...) {
#ifdef NODEBUG
	/* Empty body, so a good compiler will optimise calls to pmesg away */
#else
        va_list args;

        if (!(level & G_debug_level))
                return;

        va_start(args, format);
        vfprintf(G_debug_file,format, args);
        va_end(args);
#endif /* NODEBUG */
}
#endif /* NODEBUG && __GNUC__ */

/*********************************
*  Halts mpi if there's a problem
#define MPI_program
**********************************/
#ifndef NOMPI
#include <mpi.h>
#endif
void mpibail_def(int who, const char* file, int where, const char *format, ...)
{
        va_list args;
        fprintf(stderr,"%d: (%s:%u) ",who,file,where);
        va_start(args,format);
        vfprintf(stderr,format,args);
        va_end(args);
#ifndef NOMPI
	MPI_Abort(MPI_COMM_WORLD,-1);
        //MPI_Finalize();
#else
        exit(-1);   //otherwise just returns 
#endif
}

/* almost identical to mpibail_def but instead it takes a string NOT a rank */
void mpibailstr_def(char *who, const char* file, int where, const char *format, ...)
{
        va_list args;
        fprintf(stderr,"%s: (%s:%u) ",who,file,where);
        va_start(args,format);
        vfprintf(stderr,format,args);
        va_end(args);
#ifndef NOMPI
	MPI_Abort(MPI_COMM_WORLD,-1);
        //MPI_Finalize();
#else
        exit(-1);   //otherwise just returns 
#endif
}

/*********************************************************************************
*  Halts mpi, these are for MPI errors when the MPI_ERROR_RETURN has been specified
*  This will abort the program and print the location of the failed call, always
*  with respect to the communicator passed in (typically MPI_COMM_WORLD) everything
*  else maybe very unpredictable
***********************************************************************************/
#ifndef NOMPI 
#define MAXBUF 1024
void mpierror_def(MPI_Comm comm, int err, const char* file, int where)
{	
   char error_string[MAXBUF];
   int length_of_error_string;
   MPI_Error_string(err, error_string, &length_of_error_string);
   fprintf(stderr, "ERROR ******* [%s,%d] : %s\n", file, where, error_string);   
   MPI_Abort(MPI_COMM_WORLD, err); 
}
#endif

