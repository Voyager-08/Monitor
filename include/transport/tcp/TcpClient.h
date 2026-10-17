#pragma once

#include <asio.hpp>
#include <string>

class TcpClient
{
public:
    explicit TcpClient(asio::io_context &io);
    ~TcpClient();

    void connect(const std::string &ip, unsigned short port);
    bool sendFile(const std::string &filePath);
    void close();

private:
    asio::io_context &m_io;//是asio的io_context，用于异步操作的事件循环
    asio::ip::tcp::socket m_socket;//是asio的socket，用于连接服务器
    bool m_connected = false;//记录是否已连接到服务器
};