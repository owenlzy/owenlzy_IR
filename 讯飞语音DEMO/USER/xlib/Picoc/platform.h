/* all platform-specific includes and defines go in this file */
#ifndef PLATFORM_H
#define PLATFORM_H

/* configurable options */
/* select your host type (or do it in the Makefile):
 * #define  UNIX_HOST
 * #define  FLYINGFOX_HOST
 * #define  SURVEYOR_HOST
 * #define  SRV1_UNIX_HOST
 * #define  UMON_HOST
 */

#define LARGE_INT_POWER_OF_TEN 1000000000   /* the largest power of ten which fits in an int on this architecture */
#if defined(__hppa__) || defined(__sparc__)
#define ALIGN_TYPE double                   /* the default data type to use for alignment */
#else
#define ALIGN_TYPE void *                   /* the default data type to use for alignment */
#endif

#define GLOBAL_TABLE_SIZE 97                /* global variable table */
#define STRING_TABLE_SIZE 97                /* shared string table size */
#define STRING_LITERAL_TABLE_SIZE 97        /* string literal table size */
#define PARAMETER_MAX 16                    /* maximum number of parameters to a function */
#define LINEBUFFER_MAX 256                  /* maximum number of characters on a line */
#define LOCAL_TABLE_SIZE 11                 /* size of local variable table (can expand) */
#define STRUCT_TABLE_SIZE 11                /* size of struct/union member table (can expand) */

#define INTERACTIVE_PROMPT_START "starting picoc " PICOC_VERSION "\r\n"
#define INTERACTIVE_PROMPT_STATEMENT "picoc> "
#define INTERACTIVE_PROMPT_LINE "     > "

/* host platform includes */
#define HEAP_SIZE (16*1024)               /* space for the heap and the stack */
// #define NO_HASH_INCLUDE
#define BUILTIN_MINI_STDLIB
#undef 	BIG_ENDIAN
#define LITTLE_ENDIAN 

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
// #  include <sys/types.h>
#include <stdarg.h>
#include <setjmp.h>
#include <math.h>
#include <assert.h>			/* define NDEBUG*/
// #  define assert(x)

extern jmp_buf PicocExitBuf;
int PicocPlatformSetExitPoint(void);

#endif /* PLATFORM_H */
