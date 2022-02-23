#include <sstream>
#include "D3D11Exception.h"

D3D11Exception::D3D11Exception(int line, const char *file) noexcept :
    line(line),
    file(file)
{}

const char* D3D11Exception::what() const noexcept
{
    // oss的作用域只在当前函数，所以要用whatBuffer来缓存消息
    std::ostringstream oss;
    oss << GetType() << std::endl
        << GetOriginString();

    whatBuffer = oss.str();
    return whatBuffer.c_str();
}

const char *D3D11Exception::GetType() const noexcept
{
    return "ProjectD3D11 Exception";
}

int D3D11Exception::GetLine() const noexcept
{
    return line;
}

const std::string &D3D11Exception::GetFile() const noexcept
{
    return file;
}

std::string D3D11Exception::GetOriginString() const noexcept
{
    std::ostringstream oss;
    oss << "[File] " << file << std::endl
        << "[Line] " << line;
    return oss.str();
}
