#pragma once

#include <asio.hpp>
#include <string>

class TcpServer
{
public:
    explicit TcpServer(asio::io_context &io);
    ~TcpServer();

    void start(unsigned short port);
    void run();

private:
    asio::io_context &m_io;//是asio的io_context，用于异步操作的事件循环
    asio::ip::tcp::acceptor m_acceptor;//是asio的acceptor，用于监听客户端连接
    bool m_running = false;//记录是否服务是否在运行
};
