/*

Copyright (c) 2016 <Alexandr Dubovikov> 

All rights reserved.

Redistribution and use in source and binary forms are permitted
provided that the above copyright notice and this paragraph are
duplicated in all such forms and that any documentation,
advertising materials, and other materials related to such
distribution and use acknowledge that the software was developed
by the <SIPCAPTURE>. The name of the SIPCAPTURE may not be used to 
endorse or promote products derived from this software without specific 
prior written permission.

THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.

*/

#include <netinet/in.h>
#include <heplog.h>

typedef enum {
  SEND_UDP_REQUEST = 0,
  SEND_TCP_REQUEST = 1,
  QUIT_REQUEST
} hep_request_type_t;

typedef struct hep_connection {
  bool type;
  struct sockaddr_in send_addr;
  void *context;
} hep_connection_t;

typedef struct hep_request {
  hep_request_type_t request_type;
  hep_connection_t *conn;
  unsigned char *message;
  int len;
} hep_request_t;

