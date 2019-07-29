#include <stdlib.h>
#include <stdio.h>
#include <winsock2.h>
#include <windows.h>
#include <windowsx.h>

#include "netlib.h"


int main(void)
{
    int rc;
    int wsaErr = 0;
    SOCKET fdSock;
    fdSock = net_get_tcp_stream_socket(&wsaErr);
    printf("The socket: %d\n", fdSock );
    printf("wsaErr: %d\n", wsaErr);
    rc = net_socket_set_linger(1, 0, &fdSock, &wsaErr);
    printf("Result of net_socket_set_linger: %d\n", rc);

    return 0;
}

