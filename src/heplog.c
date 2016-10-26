/*
*
* Copyright (c) 2016 <Alexandr Dubovikov> 
* 
* All rights reserved.
* 
* Redistribution and use in source and binary forms are permitted
* provided that the above copyright notice and this paragraph are
* duplicated in all such forms and that any documentation,
* advertising materials, and other materials related to such
* distribution and use acknowledge that the software was developed
* by the <SIPCAPTURE>. The name of the SIPCAPTURE may not be used to 
* endorse or promote products derived from this software without specific 
* prior written permission.
* 
* THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
* IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
*
*/

#ifndef LOGGING_C_
#define LOGGING_C_

#include <heplog.h>
#include <stdio.h>
#include <stdarg.h>

static int use_syslog = 0;
static int log_level = LOG_WARNING;

void init_hep_log(char *_prgname, int _use_syslog) {
        use_syslog = _use_syslog;
        if (use_syslog) {
                openlog(_prgname, LOG_PID, LOG_DAEMON);
        }
}

void set_hep_log_level(int level) {
        log_level = level;
}


void destroy_hep_log(void) {
        if (use_syslog) closelog();
}


void hep_log_stdout(const char * format, va_list ap)
{
        vfprintf(stdout, format, ap);
        fprintf(stdout, "\r\n");
        fflush(stdout);
}

void data_hep_log(int priority, const char *fmt, ...) {

	va_list args;
        if (priority<=log_level) {
                va_start(args, fmt);
                if (use_syslog) vsyslog(priority, fmt, args);
                else hep_log_stdout(fmt, args);
                va_end(args);

        }
}

#endif /* LOGGING_C_ */
