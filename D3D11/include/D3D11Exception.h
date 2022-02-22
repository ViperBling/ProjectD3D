#pragma once

#include <exception>
#include <string>

class D3D11Exception : public std::exception
{
public:
    D3D11Exception(int line, const char* file) noexcept;
    // 打印错误消息
    const char* what() const noexcept override;
    // 类型
    virtual const char* GetType() const noexcept;
    int GetLine() const noexcept;
    const std::string& GetFile() const noexcept;
    // 将错误信息转化成字符串
    std::string GetOriginString() const noexcept;

private:
    int line;                   // 错误行号
    std::string file;           // 出错文件

protected:
    // mutable突破const限制，防止Buffer被lock
    mutable std::string whatBuffer;
};