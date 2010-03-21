/************************************************

  sockssocket.c -

  created at: Thu Mar 31 12:21:29 JST 1994

  Copyright (C) 1993-2007 Yukihiro Matsumoto

************************************************/

#include "rubysocket.h"

#ifdef SOCKS
static VALUE
socks_init(VALUE sock, VALUE host, VALUE serv)
{
    static int init = 0;

    if (init == 0) {
	SOCKSinit("ruby");
	init = 1;
    }

    return rsock_init_inetsock(sock, host, serv, Qnil, Qnil, INET_SOCKS);
}

#ifdef SOCKS5
static VALUE
socks_s_close(VALUE sock)
{
    rb_io_t *fptr;

    if (rb_safe_level() >= 4 && !OBJ_TAINTED(sock)) {
	rb_raise(rb_eSecurityError, "Insecure: can't close socket");
    }
    GetOpenFile(sock, fptr);
    shutdown(fptr->fd, 2);
    return rb_io_close(sock);
}
#endif
#endif

/*
 * Document-class: ::SOCKSSocket < TCPSocket
 *
 * SOCKSSocket class
 */
void
Init_sockssocket(void)
{
#ifdef SOCKS
    rb_cSOCKSSocket = rb_define_class("SOCKSSocket", rb_cTCPSocket);
    rb_define_method(rb_cSOCKSSocket, "initialize", socks_init, 2);
#ifdef SOCKS5
    rb_define_method(rb_cSOCKSSocket, "close", socks_s_close, 0);
#endif
#endif
}
