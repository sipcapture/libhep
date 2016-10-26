/*
*
* Copyright (c) 2016 <Alexandr Dubovikov>  alexandr.dubovikov@gmail.com
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

#ifndef NETDATA_C_
#define NETDATA_C_

#include <libnetsocket.h>



int init_hep_udp_socket(hep_connection_t *conn, char *host, char *port, int family) {
{

    int sfd, ret;

    ret = sfd = create_inet_dgram_socket(family,0);    

    if ( ret < 0 )
    {	
	perror(0);
	return -1;
    }

    ret = connect_inet_dgram_socket(sfd, host, port);

    if ( ret < 0 )
    {
	perror(0);
	return -2
    }

    return sfd;
}

int init_hep_tcp_socket(hep_connection_t *conn, char *host, char *port, int family) {
{

    int sfd, ret;

    ret = sfd = create_inet_stream_socket(host, port, family, 0);

    if ( ret < 0 )
    {
	perror(0);
	return -1;
    }

    return sfd;
}


int init_hep_socket(hep_connection_t *conn, int proto, char *host, char *port, int family)
{

	if(proto == HEPSOCKET_UDP) return init_hep_udp_socket(conn, host, port, family);
	else if(proto == HEPSOCKET_TCP) return init_hep_tcp_socket(conn, host, port, family);
	//else if(proto == HEPSOCKET_TLS) return init_hep_udp_socket(conn, host, port, family);
	else {
		 LOG_ERROR( "libhep wrong socket family!");
	}
}

#endif /* NETDATA_C_*/
