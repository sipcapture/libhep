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

#ifndef LOGGING_H_
#define LOGGING_H_

#include <syslog.h>

void init_hep_log(char *_prgname, int _use_syslog);

void set_hep_log_level(int level);

void destroy_hep_log(void);

void data_hep_log(int priority, const char * fmt, ...);

#define PA_GCC_PRINTF_ATTR(a,b) __attribute__ ((format (printf, a, b)));

#define LOG_EMERG(fmt, args...) data_hep_log(LOG_EMERG, "[DEBUG] %s:%d " fmt, __FILE__, __LINE__, ## args)
#define LOG_ALERT(fmt, args...) data_hep_log(LOG_ALERT, "[ALERT] %s:%d " fmt, __FILE__, __LINE__, ## args)
#define LOG_CRIT(fmt, args...) data_hep_log(LOG_CRIT, "[CRIT] %s:%d " fmt, __FILE__, __LINE__, ## args)
#define LOG_ERROR(fmt, args...) data_hep_log(LOG_ERR, "[ERR] %s:%d " fmt, __FILE__, __LINE__, ## args)
#define LOG_WARNING(fmt, args...) data_hep_log(LOG_WARNING, "[WARNING] %s:%d " fmt, __FILE__, __LINE__, ## args)
#define LOG_NOTICE(fmt, args...) data_hep_log(LOG_NOTICE, "[NOTICE] " fmt, ## args)
#define LOG_INFO(fmt, args...) data_hep_log(LOG_INFO, "[INFO] %s:%d " fmt, __FILE__, __LINE__, ## args)
#define LOG_DEBUG(fmt, args...) data_hep_log(LOG_DEBUG, "[DEBUG] %s:%d " fmt, __FILE__, __LINE__, ## args)
#define LOG_MESSAGE(fmt, args...) data_hep_log(LOG_ERR, "[MESSAGE] " fmt, ## args)

#endif /* LOGGING_H_ */
