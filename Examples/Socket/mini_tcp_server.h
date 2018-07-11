﻿#ifndef mini_tcp_server_h
#define mini_tcp_server_h

#include <stdint.h>

typedef uint64_t peer_t;

typedef void(*on_connection_cb_t)(void*, peer_t);
typedef void(*on_read_cb_t)(void*, peer_t, void*, size_t);
typedef void(*on_close_cb_t)(void*, peer_t);


class IMiniTcpServer
{
public:
    virtual ~IMiniTcpServer() {}

    // 开始监听，port不需要htons来转换字节序，内部会帮你完成
    virtual int Listen(char const *ip, int port) = 0;

    // 停止监听
    virtual int Close() = 0;

    // 设置收到客户端连接回调
    virtual void SetOnConnectionCallback(on_connection_cb_t const cb, void *ud) = 0;

    // 设置收到客户端数据回调
    virtual void SetOnReadCallback(on_read_cb_t const &cb, void *ud) = 0;

    // 设置客户端连接断开回调
    virtual void SetOnCloseCallback(on_close_cb_t const &cb, void *ud) = 0;

    // 向客户端发送数据
    virtual int Send(peer_t peer, void const *data, size_t sz) = 0;

    // 断开与客户端的连接
    virtual int Disconnect(peer_t peer) = 0;

    // 轮询处理消息
    virtual int Poll() = 0;
};


IMiniTcpServer* CreateServer();

#endif // mini_tcp_server_h
