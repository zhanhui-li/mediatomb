/*MT*
    
    MediaTomb - http://www.mediatomb.org/
    
    logger.cc - this file is part of MediaTomb.
    
    Copyright (C) 2005 Gena Batyan <bgeradz@mediatomb.org>,
                       Sergey Bostandzhyan <jin@mediatomb.org>
    Copyright (C) 2006 Gena Batyan <bgeradz@mediatomb.org>,
                       Sergey Bostandzhyan <jin@mediatomb.org>,
                       Leonhard Wimmer <leo@mediatomb.org>
    
    MediaTomb is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.
    
    MediaTomb is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    
    You should have received a copy of the GNU General Public License
    along with MediaTomb; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
    
    $Id$
*/

/// \file logger.cc

#ifdef HAVE_CONFIG_H
    #include "autoconfig.h"
#endif

#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>

#ifdef HAVE_EXECINFO_H
    #include <execinfo.h>
#endif

#include "logger.h"

FILE *LOG_FILE = stderr;

#ifdef LOG_FLUSH
#define FLUSHIT fflush(LOG_FILE);
#else
#define FLUSHIT
#endif

void log_open(char *filename)
{
    LOG_FILE = fopen(filename, "a");
    if (! LOG_FILE)
    {
        fprintf(stderr, "Could not open log file %s : %s\n",
                filename, strerror(errno));
        exit(1);
    }
}
void log_close()
{
    if (LOG_FILE)
        fclose(LOG_FILE);
}

static void log_stamp(const char *type)
{
    time_t unx;
    struct tm t;
    time(&unx);
    localtime_r(&unx, &t);
    fprintf(LOG_FILE, "%.4d-%.2d-%.2d %.2d:%.2d:%.2d %*s: ",
           t.tm_year + 1900,
           t.tm_mon,
           t.tm_mday,
           t.tm_hour,
           t.tm_min,
           t.tm_sec,
           7, // max length we have is "WARNING"
           type);
}
        
void _log_info(const char *format, ...)
{
    va_list ap;
    va_start(ap, format);
    log_stamp("INFO");
    vfprintf(LOG_FILE, format, ap);
    FLUSHIT
    va_end(ap);
}
void _log_warning(const char *format, ...)
{
    va_list ap;
    va_start(ap, format);
    log_stamp("WARNING");
    vfprintf(LOG_FILE, format, ap);
    FLUSHIT
    va_end(ap);
}
void _log_error(const char *format, ...)
{
    va_list ap;
    va_start(ap, format);
    log_stamp("ERROR");
    vfprintf(LOG_FILE, format, ap);
    FLUSHIT
    va_end(ap);
}
void _log_debug(const char *format, const char *file, int line, const char *function, ...)
{
    va_list ap;
    va_start(ap, function);
    log_stamp("DEBUG");
    fprintf(LOG_FILE, "[%s:%d] %s(): ", file, line, function);
    vfprintf(LOG_FILE, format, ap);
    FLUSHIT
    va_end(ap);
}

void _print_backtrace()
{
#if defined HAVE_BACKTRACE && defined HAVE_BACKTRACE_SYMBOLS
    void* b[100];
    int size = backtrace(b, 100);
    char **s = backtrace_symbols(b, size);
    for(int i = 0; i < size; i++)
        fprintf(LOG_FILE, "_STRACE_ %i %s\n", i, s[i]);
    free(s);
#endif
}

