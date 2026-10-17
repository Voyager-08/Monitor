#include "transport/tcp/TcpServer.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <vector>

using asio::ip::tcp;

TcpServer::TcpServer(asio::io_context &io)
    : m_io(io), m_acceptor(io) {}

TcpServer::~TcpServer()
{
    m_running = false;
    asio::error_code ec;
    m_acceptor.close(ec);
}

void TcpServer::start(unsigned short port)
{
    m_acceptor.open(tcp::v4());
    m_acceptor.set_option(tcp::acceptor::reuse_address(true));
    m_acceptor.bind(tcp::endpoint(tcp::v4(), port));
    m_acceptor.listen();
    m_running = true;
    std::cout << "[Server] 监听 0.0.0.0:" << port << std::endl;
}

void TcpServer::run()
{
    while (m_running) {
        tcp::socket sock(m_io);
        asio::error_code ec;
        m_acceptor.accept(sock, ec);
        if (ec) {
            std::cerr << "[Server] accept 错误: " << ec.message() << std::endl;
            continue;
        }
        std::cout << "[Server] 有客户端连接: " << sock.remote_endpoint() << std::endl;

        char sizeBuf[4];
        asio::read(sock, asio::buffer(sizeBuf, 4));
        uint32_t nameLen = asio::detail::socket_ops::network_to_host_long(
            *reinterpret_cast<uint32_t *>(sizeBuf));

        std::string fileName(nameLen, '\0');
        asio::read(sock, asio::buffer(fileName.data(), nameLen));

        asio::read(sock, asio::buffer(sizeBuf, 4));
        uint32_t fileSize = asio::detail::socket_ops::network_to_host_long(
            *reinterpret_cast<uint32_t *>(sizeBuf));

        std::cout << "[Server] 正在接收: " << fileName
                  << " (" << fileSize << " bytes)" << std::endl;

        std::ofstream ofs("recv_" + fileName, std::ios::binary);
        std::vector<char> buf(8192);
        uint32_t total = 0;
        while (total < fileSize) {
            uint32_t chunk = std::min<uint32_t>(static_cast<uint32_t>(buf.size()), fileSize - total);
            size_t n = sock.read_some(asio::buffer(buf.data(), chunk));
            ofs.write(buf.data(), static_cast<std::streamsize>(n));
            total += static_cast<uint32_t>(n);
        }

        std::cout << "[Server] 接收完成 -> recv_" << fileName << std::endl;
    }
}