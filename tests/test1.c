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

    printf("Get a socket...\n");
    fdSock = net_get_tcp_stream_socket(&wsaErr);
    printf("Result of socket(): %d\n", fdSock );
    printf("wsaErr: %d\n", wsaErr);

    printf("Calling net_socket_set_linger...\n");
    rc = net_socket_set_linger(1, 0, &fdSock, &wsaErr);
    printf("Result of net_socket_set_linger: %d\n", rc);
    printf("wsaErr: %d\n", wsaErr);

    printf("Calling net_socket_set_blocking_state...\n");
    rc = net_socket_set_blocking_state(&fdSock, 1, &wsaErr);
    printf("Result of net_set_socket_blocking_state: %d\n", rc);
    printf("wsaErr: %d\n", wsaErr);

    return 0;
}

