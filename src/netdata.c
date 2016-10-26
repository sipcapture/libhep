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

# include <stdlib.h>
# include <stdio.h>
# include <sys/socket.h>
# include <sys/types.h>
# include <unistd.h> // read()/write()
# include <stdint.h>
# include <netdb.h> // getaddrinfo()
# include <string.h>
# include <errno.h>
# include <sys/ioctl.h>
# include <net/if.h>






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
            LOG_ERROR( "capture: getaddrinfo: %s", gai_strerror(s));
            return 2;
    }

    if((profile_transport[idx].socket = socket(ai->ai_family, ai->ai_socktype, ai->ai_protocol)) < 0) {
             LOG_ERROR("Sender socket creation failed: %s", strerror(errno));
             return 1;
    }

    if ((ret = connect(profile_transport[idx].socket, ai->ai_addr, (socklen_t)(ai->ai_addrlen))) == -1) {

         //select(profile_transport[idx].socket + 1 , NULL, &myset, NULL, &tv);
         if (errno != EINPROGRESS) {
             LOG_ERROR("Sender socket creation failed: %s", strerror(errno));
             return 1;
          }
    }

    return 0;
}


/* libnetsocket */


int create_inet_dgram_socket(char proto_osi3, int flags)
{
    int sfd;

    if (proto_osi3 != LIBSOCKET_IPv4 && proto_osi3 != LIBSOCKET_IPv6)
    {
#ifdef VERBOSE
	LOG_ERROR("create_inet_dgram_socket: osi3 argument invalid for DGRAM sockets\n");
#endif
	return -1;
    }

    switch ( proto_osi3 )
    {
	case LIBSOCKET_IPv4 :
	    sfd = socket(AF_INET,SOCK_DGRAM|flags,0);
	    break;
	case LIBSOCKET_IPv6 :
	    sfd = socket(AF_INET6,SOCK_DGRAM|flags,0);
	    break;
	default:
	    return -1;
    }

    if ( -1 == check_error(sfd) )
	return -1;

    return sfd;
}


/**
 * @brief Connect a UDP socket.
 *
 * If a datagram socket is connected, all data written to it (using `write(2)`) is sent to the peer
 * connected to and all data `read(2)` from it is data sent by the peer. Usually used by clients only.
 *
 * @param sfd The socket file descriptor
 * @param host The host to connect to
 * @param service The port/service specifier
 *
 * @retval 0 Success
 * @retval -1 Error.
 */
int connect_inet_dgram_socket(int sfd, const char* host, const char* service)
{
    struct addrinfo *result, *result_check, hint;
    struct sockaddr_storage oldsockaddr;
    struct sockaddr deconnect;
    socklen_t oldsockaddrlen = sizeof(struct sockaddr_storage);
    int return_value;
# ifdef VERBOSE
    const char* errstring;
# endif

    if ( sfd < 0 )
	return -1;

    if ( host == NULL )
    {
	// This does not work on FreeBSD systems. We pretend to disconnect the socket although we don't do so. This is not very severe for the application
# ifndef __FreeBSD__
	memset(&deconnect,0,sizeof(struct sockaddr));

	deconnect.sa_family = AF_UNSPEC;

	if ( check_error(connect(sfd,&deconnect,sizeof(struct sockaddr))) )
	    return -1;
# endif
	return 0;
    }

    if ( -1 == check_error(getsockname(sfd,(struct sockaddr*)&oldsockaddr,&oldsockaddrlen)) )
	return -1;

    if ( oldsockaddrlen > sizeof(struct sockaddr_storage) ) // If getsockname truncated the struct
	return -1;

    memset(&hint,0,sizeof(struct addrinfo));

    hint.ai_family = ((struct sockaddr_in*)&oldsockaddr)->sin_family; // AF_INET or AF_INET6 - offset is same at sockaddr_in and sockaddr_in6
    hint.ai_socktype = SOCK_DGRAM;

    if ( 0 != (return_value = getaddrinfo(host,service,&hint,&result)))
    {
# ifdef VERBOSE
	errstring = gai_strerror(return_value);
	debug_write(errstring);
# endif
	return -1;
    }

    // As described in "The Linux Programming Interface", Michael Kerrisk 2010, chapter 59.11 (p. 1220ff)

    for ( result_check = result; result_check != NULL; result_check = result_check->ai_next ) // go through the linked list of struct addrinfo elements
    {
	if ( -1 != (return_value = connect(sfd,result_check->ai_addr,result_check->ai_addrlen))) // connected without error
	{
	    break;
	} else
	{
	    check_error(return_value);
	}
    }

    // We do now have a working (updated) socket connection to our target

    if ( result_check == NULL ) // or not?
    {
# ifdef VERBOSE
	debug_write("connect_inet_dgram_socket: Could not connect to any address!\n");
# endif
	return -1;
    }

    freeaddrinfo(result);

    return 0;
}

/**
 * @brief Close a socket.
 *
 * This function closes a socket. You may also use `close(2)`.
 *
 * @param sfd The file descriptor
 *
 * @retval 0 Closed socket successfully
 * @retval -1 Socket was already closed (other errors are very unlikely to occur)
 */
int destroy_inet_socket(int sfd)
{
    if ( sfd < 0 )
	return -1;

    if ( -1 == check_error(close(sfd)))
	return -1;

    return 0;
}



#endif /* NETDATA_C_*/
