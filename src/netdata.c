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



int init_udp_socket(hep_connection_t *conn, char *host, int port) {
{
      

  
      return 1;
}



int uv_ip4_addr(const char* ip, int port, struct sockaddr_in* addr) {

      memset(addr, 0, sizeof(*addr));
      addr->sin_family = AF_INET;
      addr->sin_port = htons(port);
      return uv_inet_pton(AF_INET, ip, &(addr->sin_addr.s_addr));
}


struct sockaddr_in6 create_socket6(int port, const char * address) {

    struct addrinfo hints, *res, *resalloc;
    struct sockaddr_in6 input_socket6;
    int errcode;

    memset (&hints, 0, sizeof (hints));
    memset (&input_socket6, 0, sizeof(struct sockaddr_in6));

    hints.ai_family = AF_INET6;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_DEFAULT;

    errcode = getaddrinfo (address, NULL, &hints, &res);

    if (errcode != 0)
    {
		perror ("[ERROR] getaddrinfo ");
		return input_socket6;
    }

    resalloc = res;

    while (res)
    {
        if(res->ai_family == AF_INET6) {
                        memcpy (&input_socket6, res->ai_addr, res->ai_addrlen);
                        input_socket6.sin6_port = htons (port);
                        input_socket6.sin6_family = AF_INET6;
               break;
        }

        res = res->ai_next;
    }

    freeaddrinfo(resalloc);

    return input_socket6;
}

int init_hepsocket_blocking (unsigned int idx) {

    int s, ret = 0;
    struct timeval tv;
    struct addrinfo *ai;
    struct addrinfo hints[1] = {{ 0 }};

    stats.reconnect_total++;

    hints->ai_flags = AI_NUMERICSERV;
    hints->ai_family = AF_UNSPEC;

    if(!strncmp(profile_transport[idx].capt_proto, "udp", 3)) {
               hints->ai_socktype = SOCK_DGRAM;
               hints->ai_protocol = IPPROTO_UDP;
    }
    else if(!strncmp(profile_transport[idx].capt_proto, "tcp", 3) || !strncmp(profile_transport[idx].capt_proto, "ssl", 3)) {
               hints->ai_socktype = SOCK_STREAM;
               hints->ai_protocol = IPPROTO_TCP;
    }

    if(profile_transport[idx].socket) close(profile_transport[idx].socket);

    if ((s = getaddrinfo(profile_transport[idx].capt_host, profile_transport[idx].capt_port, hints, &ai)) != 0) {
            LERR( "capture: getaddrinfo: %s", gai_strerror(s));
            return 2;
    }

    if((profile_transport[idx].socket = socket(ai->ai_family, ai->ai_socktype, ai->ai_protocol)) < 0) {
             LERR("Sender socket creation failed: %s", strerror(errno));
             return 1;
    }

    if ((ret = connect(profile_transport[idx].socket, ai->ai_addr, (socklen_t)(ai->ai_addrlen))) == -1) {

         //select(profile_transport[idx].socket + 1 , NULL, &myset, NULL, &tv);
         if (errno != EINPROGRESS) {
             LERR("Sender socket creation failed: %s", strerror(errno));
             return 1;
          }
    }

    return 0;
}
