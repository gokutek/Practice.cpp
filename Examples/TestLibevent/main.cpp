#include <assert.h>
#include <iostream>
#include <WinSock2.h>
#include "event2/listener.h"
#include "event2/bufferevent.h"
#include "event2/buffer.h"


static void read_cb(bufferevent *bev, void *ctx)
{
    struct evbuffer *input = bufferevent_get_input(bev);
    struct evbuffer *output = bufferevent_get_output(bev);

    evbuffer_add_buffer(output, input);
}


static void write_cb(bufferevent *bev, void *ctx)
{
}


static void event_cb(bufferevent *bev, short what, void *ctx)
{
}


// 收到客户端的连接请求
static void listener_cb(evconnlistener *listener, evutil_socket_t fd, sockaddr *sa, int socklen, void *user_data)
{
    event_base *base = (event_base*)user_data;
    assert(base);

    bufferevent *bev = bufferevent_socket_new(base, fd, BEV_OPT_CLOSE_ON_FREE);
    assert(bev);

    bufferevent_setcb(bev, read_cb, write_cb, event_cb, NULL);

    bufferevent_enable(bev, EV_READ | EV_WRITE);
}


// ECHO server DEMO
static int StartEchoServer(int port)
{
    event_base *base = event_base_new();
    assert(base);

    struct sockaddr_in sin;
    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_port = htons(60005);

    evconnlistener *listener = evconnlistener_new_bind(base, listener_cb, base, LEV_OPT_REUSEABLE | LEV_OPT_CLOSE_ON_FREE, -1, (sockaddr*)&sin, sizeof(sin));
    assert(listener);

    event_base_dispatch(base);

    evconnlistener_free(listener);

    event_base_free(base);

    return 0;
}


static int StartClient()
{
    return 0;
}


int main()
{
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    StartEchoServer(60005);

    WSACleanup();
    return 0;
}
