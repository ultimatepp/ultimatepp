/** @file io.c
 * MAT File I/O Utility Functions
 */
/*
 * Copyright (c) 2005-2019, Christopher C. Hulbert
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include "matio_private.h"
/*
#if !defined(HAVE_VA_COPY) && defined(HAVE___VA_COPY)
#    define va_copy(d,s) __va_copy(d,s)
#elif !defined(HAVE_VA_COPY)
#    define va_copy(d,s) memcpy(&(d),&(s),sizeof(va_list))
#endif
*/
static void (*logfunc)(int log_level, char *message ) = NULL;
static const char *progname = NULL;

/** @brief Allocates and prints to a new string
 *
 * @ingroup mat_util
 * @param format format string
 * @param ap variable argument list
 * @return Newly allocated string with format printed to it
 */
char *
strdup_vprintf(const char* format, va_list ap)
{
    va_list ap2;
    int size;
    char* buffer;

    va_copy(ap2, ap);
    size = mat_vsnprintf(NULL, 0, format, ap2)+1;
    va_end(ap2);

    buffer = (char*)malloc(size+1);
    if ( !buffer )
        return NULL;

    mat_vsnprintf(buffer, size, format, ap);
    return buffer;
}

/** @brief Allocates and prints to a new string using printf format
 *
 * @ingroup mat_util
 * @param format format string
 * @return Pointer to resulting string, or NULL if there was an error
 */
char *
strdup_printf(const char* format, ...)
{
    char* buffer;
    va_list ap;
    va_start(ap, format);
    buffer = strdup_vprintf(format, ap);
    va_end(ap);
    return buffer;
}

/** @brief Default logging function
 *
 * Prints the message to stderr/stdout and calls abort() if the
 * log_level equals MATIO_LOG_LEVEL_ERROR.
 * @ingroup mat_util
 * @param log_level logging level
 * @param message logging message
 */
static void
mat_logfunc( int log_level, char *message )
{
    if ( progname ) {
        if ( log_level & MATIO_LOG_LEVEL_CRITICAL) {
            fprintf(stderr,"-E- %s: %s\n", progname, message);
            fflush(stderr);
        } else if ( log_level & MATIO_LOG_LEVEL_ERROR ) {
            fprintf(stderr,"-E- %s: %s\n", progname, message);
            fflush(stderr);
            abort();
        } else if ( log_level & MATIO_LOG_LEVEL_WARNING ) {
            fprintf(stderr,"-W- %s: %s\n", progname, message);
            fflush(stderr);
        } else if ( log_level & MATIO_LOG_LEVEL_DEBUG ) {
            fprintf(stderr,"-D- %s: %s\n", progname, message);
            fflush(stderr);
        } else if ( log_level & MATIO_LOG_LEVEL_MESSAGE ) {
            fprintf(stdout,"%s\n", message);
            fflush(stdout);
        }
    } else {
        if ( log_level & MATIO_LOG_LEVEL_CRITICAL) {
            fprintf(stderr,"-E- : %s\n", message);
            fflush(stderr);
        } else if ( log_level & MATIO_LOG_LEVEL_ERROR ) {
            fprintf(stderr,"-E- : %s\n", message);
            fflush(stderr);
            abort();
        } else if ( log_level & MATIO_LOG_LEVEL_WARNING ) {
            fprintf(stderr,"-W- : %s\n", message);
            fflush(stderr);
        } else if ( log_level & MATIO_LOG_LEVEL_DEBUG ) {
            fprintf(stderr,"-D- : %s\n", message);
            fflush(stderr);
        } else if ( log_level & MATIO_LOG_LEVEL_MESSAGE ) {
            fprintf(stdout,"%s\n", message);
            fflush(stdout);
        }
    }
}

/** @brief Logging function handler
 *
 * Calls either the default logging function @ref mat_logfunc
 * set by @ref Mat_LogInit or a custom logging function set by
 * @ref Mat_LogInitFunc.
 * @ingroup mat_util
 * @param loglevel log level
 * @param format format string
 * @param ap variable argument list
 */
static void
mat_log(int loglevel, const char *format, va_list ap)
{
    char* buffer;

    if ( !logfunc ) return;
    buffer = strdup_vprintf(format, ap);
    (*logfunc)(loglevel,buffer);
    free(buffer);
    return;
}

#if defined(MAT73) && MAT73
#define MSG_SIZE 1024

/** @brief HDF5 Error logging function
 *
 * @ingroup mat_util
 * @param n indexed position of the error in the stack
 * @param err_desc pointer to a data structure describing the error
 * @param client_data pointer to client data
 */
static herr_t
mat_h5_log_func(unsigned n, const H5E_error_t *err_desc, void *client_data)
{
    char maj[MSG_SIZE];
    char min[MSG_SIZE];
    char cls[MSG_SIZE];

    if ( H5Eget_class_name(err_desc->cls_id, cls, MSG_SIZE) < 0 )
        return -1;

    if ( H5Eget_msg(err_desc->maj_num, NULL, maj, MSG_SIZE) < 0 )
        return -1;

    if ( H5Eget_msg(err_desc->min_num, NULL, min, MSG_SIZE) < 0 )
        return -1;

    Mat_Critical("%s error #%03u in %s()\n"
        "      file : %s:%u\n"
        "      major: %s\n"
        "      minor: %s",
        cls, n, err_desc->func_name, err_desc->file_name, err_desc->line,
        maj, min);

   return 0;
}

/** @brief HDF5 Error logging function callback
 *
 * @ingroup mat_util
 * @param estack error stack identifier
 * @param client_data pointer to client data
 */
static herr_t
mat_h5_log_cb(hid_t estack, void *client_data)
{
    hid_t estack_id = H5Eget_current_stack();
    H5Ewalk(estack_id, H5E_WALK_DOWNWARD, mat_h5_log_func, client_data);
    return H5Eclose_stack(estack_id);
}
#endif

/** @var debug
 *  @brief holds the debug level set in @ref Mat_SetDebug
 *  This variable is used to determine if information should be printed to
 *  the screen
 *  @ingroup mat_util
 */
static int debug = 0;

/** @var verbose
 *  @brief holds the verbose level set in @ref Mat_SetVerbose
 *  This variable is used to determine if information should be printed to
 *  the screen
 *  @ingroup mat_util
 */
static int verbose = 0;

/** @var silent
 *  @brief holds the silent level set in @ref Mat_SetVerbose
 *  If set, all output which is not an error is not displayed regardless
 *  of verbose level
 *  @ingroup mat_util
 */
static int silent = 0;

/** @brief Sets verbose parameters
 *
 *  Sets the verbose level and silent level.  These values are used by
 *  programs to determine what information should be printed to the screen
 *  @ingroup mat_util
 *  @param verb sets logging verbosity level
 *  @param s sets logging silent level
 */
int
Mat_SetVerbose( int verb, int s )
{
    verbose = verb;
    silent  = s;

    return 0;
}

/** @brief Set debug parameter
 *
 *  Sets the debug level.  This value is used by
 *  program to determine what information should be printed to the screen
 *  @ingroup mat_util
 *  @param d sets logging debug level
 */
int
Mat_SetDebug( int d )
{
    debug = d;
    return 0;
}

/** @brief Log a message unless silent
 *
 * Logs the message unless the silent option is set (See @ref Mat_SetVerbose).
 * To log a message based on the verbose level, use @ref Mat_VerbMessage
 * @ingroup mat_util
 * @param format message format
 */
int Mat_Message( const char *format, ... )
{
    va_list ap;

    if ( silent ) return 0;
    if ( !logfunc ) return 0;

    va_start(ap, format );
    mat_log(MATIO_LOG_LEVEL_MESSAGE, format, ap );
    va_end(ap);
    return 0;
}

/** @brief Log a message based on debug level
 *
 *  If @e level is less than or equal to the set debug level, the message
 *  is printed.  If the level is higher than the set debug level nothing
 *  is displayed.
 *  @ingroup mat_util
 *  @param level debug level
 *  @param format message format
 */
int Mat_DebugMessage( int level, const char *format, ... )
{
    va_list ap;

    if ( silent ) return 0;
    if ( level > debug ) return 0;

    va_start(ap, format );
    mat_log(MATIO_LOG_LEVEL_DEBUG, format, ap );
    va_end(ap);
    return 0;
}

/** @brief Log a message based on verbose level
 *
 *  If @e level is less than or equal to the set verbose level, the message
 *  is printed.  If the level is higher than the set verbose level nothing
 *  is displayed.
 *  @ingroup mat_util
 *  @param level verbose level
 *  @param format message format
 */
int Mat_VerbMessage( int level, const char *format, ... )
{
    va_list ap;

    if ( silent ) return 0;
    if ( level > verbose ) return 0;

    va_start(ap, format );
    mat_log(MATIO_LOG_LEVEL_MESSAGE, format, ap );
    va_end(ap);
    return 0;
}

/** @brief Logs a Critical message and returns to the user
 *
 * Logs a Critical message and returns to the user.  If the program should
 * stop running, use @ref Mat_Error
 * @ingroup mat_util
 * @param format format string identical to printf format
 * @param ... arguments to the format string
 */
void Mat_Critical( const char *format, ... )
{
    va_list ap;

    va_start(ap, format );
    mat_log(MATIO_LOG_LEVEL_CRITICAL, format, ap );
    va_end(ap);
}

/** @brief Logs a Critical message and aborts the program
 *
 * Logs an Error message and aborts
 * @ingroup mat_util
 * @param format format string identical to printf format
 * @param ... arguments to the format string
 */
void Mat_Error( const char *format, ... )
{
    va_list ap;

    va_start(ap, format );
    mat_log( MATIO_LOG_LEVEL_ERROR, format, ap ); /* Shall never return to the calling function */
    va_end(ap);
    abort(); /* Always abort */
}

/** @brief Prints a helpstring to stdout and exits with status EXIT_SUCCESS (typically 0)
 *
 * Prints the array of strings to stdout and exits with status EXIT_SUCCESS.  The array
 * of strings should have NULL as its last element
 * @code
 * char *helpstr[] = {"My Help string line1","My help string line 2",NULL};
 * Mat_Help(helpstr);
 * @endcode
 * @ingroup mat_util
 * @param helpstr array of strings with NULL as its last element
 */
void Mat_Help( const char *helpstr[] )
{
    int i;
    for (i = 0; helpstr[i] != NULL; i++)
        printf("%s\n",helpstr[i]);
    exit(EXIT_SUCCESS);
}

/** @brief Closes the logging system
 *
 * @ingroup mat_util
 * @retval 1
 */
int
Mat_LogClose( void )
{
    logfunc = NULL;
#if defined(MAT73) && MAT73
    H5Eset_auto(H5E_DEFAULT, NULL, NULL);
#endif
    return 1;
}

/** @brief Initializes the logging system
 *
 * @ingroup mat_util
 * @param prog_name Name of the program initializing the logging functions
 * @return 0 on success
 */
int
Mat_LogInit( const char *prog_name )
{
    logfunc = &mat_logfunc;
#if defined(MAT73) && MAT73
    H5Eset_auto(H5E_DEFAULT, mat_h5_log_cb, NULL);
#endif
    verbose = 0;
    silent  = 0;

    return 0;
}

/** @brief Initializes the logging system
 *
 * @ingroup mat_util
 * @param prog_name Name of the program initializing the logging functions
 * @param log_func pointer to the function to do the logging
 * @return 0 on success
 */
int
Mat_LogInitFunc(const char *prog_name,
    void (*log_func)(int log_level,char *message))
{
    logfunc = log_func;
    progname = prog_name;
#if defined(MAT73) && MAT73
    H5Eset_auto(H5E_DEFAULT, mat_h5_log_cb, NULL);
#endif
    verbose = 0;
    silent  = 0;
    return 0;
}

/** @brief Prints a warning message to stdout
 *
 * Logs a warning message then returns
 * @ingroup mat_util
 * @param format format string identical to printf format
 * @param ... arguments to the format string
 */
void
Mat_Warning( const char *format, ... )
{
    va_list ap;

    va_start(ap, format );
    mat_log(MATIO_LOG_LEVEL_WARNING, format, ap );
    va_end(ap);
}

/** @brief Calculate the size of MAT data types
 *
 * @ingroup mat_util
 * @param data_type Data type enumeration
 * @return size of the data type in bytes
 */
size_t
Mat_SizeOf(enum matio_types data_type)
{
    switch (data_type) {
        case MAT_T_DOUBLE:
            return sizeof(double);
        case MAT_T_SINGLE:
            return sizeof(float);
#ifdef HAVE_MAT_INT64_T
        case MAT_T_INT64:
            return sizeof(mat_int64_t);
#endif
#ifdef HAVE_MAT_UINT64_T
        case MAT_T_UINT64:
            return sizeof(mat_uint64_t);
#endif
        case MAT_T_INT32:
            return sizeof(mat_int32_t);
        case MAT_T_UINT32:
            return sizeof(mat_uint32_t);
        case MAT_T_INT16:
            return sizeof(mat_int16_t);
        case MAT_T_UINT16:
            return sizeof(mat_uint16_t);
        case MAT_T_INT8:
            return sizeof(mat_int8_t);
        case MAT_T_UINT8:
            return sizeof(mat_uint8_t);
        case MAT_T_UTF8:
            return 1;
        case MAT_T_UTF16:
            return 2;
        case MAT_T_UTF32:
            return 4;
        default:
            return 0;
    }
}
