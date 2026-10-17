#include "transport/tcp/TcpClient.h"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <vector>

using asio::ip::tcp;

TcpClient::TcpClient(asio::io_context &io)
    : m_io(io), m_socket(io) {}

TcpClient::~TcpClient()
{
    close();
}

void TcpClient::connect(const std::string &ip, unsigned short port)
{
    tcp::resolver resolver(m_io);
    asio::connect(m_socket, resolver.resolve(ip, std::to_string(port)));
    m_connected = true;
    std::cout << "[Client] 已连接 " << ip << ":" << port << std::endl;
}

bool TcpClient::sendFile(const std::string &filePath)
{
    if (!m_connected) {
        std::cerr << "[Client] 未连接服务器" << std::endl;
        return false;
    }

    std::filesystem::path p(filePath);
    if (!std::filesystem::exists(p)) {
        std::cerr << "[Client] 文件不存在: " << filePath << std::endl;
        return false;
    }

    std::ifstream ifs(filePath, std::ios::binary | std::ios::ate);
    if (!ifs) {
        std::cerr << "[Client] 无法打开文件: " << filePath << std::endl;
        return false;
    }

    std::streamsize fileSize = ifs.tellg();
    ifs.seekg(0, std::ios::beg);

    std::string fileName = p.filename().string();
    uint32_t netNameLen = asio::detail::socket_ops::host_to_network_long(
        static_cast<uint32_t>(fileName.size()));
    uint32_t netFileSize = asio::detail::socket_ops::host_to_network_long(
        static_cast<uint32_t>(fileSize));

    asio::write(m_socket, asio::buffer(&netNameLen, 4));
    asio::write(m_socket, asio::buffer(fileName.data(), fileName.size()));
    asio::write(m_socket, asio::buffer(&netFileSize, 4));

    std::vector<char> buf(8192);
    std::cout << "[Client] 正在发送: " << fileName
              << " (" << fileSize << " bytes)" << std::endl;
    while (ifs) {
        ifs.read(buf.data(), static_cast<std::streamsize>(buf.size()));
        std::streamsize n = ifs.gcount();
        if (n > 0)
            asio::write(m_socket, asio::buffer(buf.data(), static_cast<size_t>(n)));
    }

    std::cout << "[Client] 发送完成" << std::endl;
    return true;
}

void TcpClient::close()
{
    if (m_connected) {
        asio::error_code ec;
        m_socket.shutdown(tcp::socket::shutdown_both, ec);
        m_socket.close(ec);
        m_connected = false;
    }
}