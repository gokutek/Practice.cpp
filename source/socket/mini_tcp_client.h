#ifndef mini_tcp_client_h
#define mini_tcp_client_h

#include <WinSock2.h>

class MiniTcpClient
{
    typedef void(*on_connect_cb_t)(void*);
    typedef void(*on_read_cb_t)(void*, void*, size_t);
    typedef void(*on_close_cb_t)(void*);

public:
    MiniTcpClient();

    ~MiniTcpClient();

    // 连接服务器
    int Connect(char const *ip, int port);

    // 与服务器断开连接
    int Close();

    // 向服务器发送数据
    int Send(void const *data, int sz);

    // 设置与服务端连接成功/失败回调
    void SetOnConnectCallback(on_connect_cb_t cb, void *ud);

    // 设置收到服务端数据回调
    void SetOnReadCallback(on_read_cb_t cb, void *ud);

    // 设置与服务端连接断开回调
    void SetOnCloseCallback(on_close_cb_t cb, void *ud);

    // 轮询处理消息
    int Poll();

private:
    SOCKET          sock_;
    on_connect_cb_t on_connect_;
    void            *on_connect_ud_;
    on_read_cb_t    on_read_;
    void            *on_read_ud_;
    on_close_cb_t   on_close_;
    void            *on_close_ud_;
    bool            connected_;
};

#endif // mini_tcp_client_h
