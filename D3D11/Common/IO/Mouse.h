#pragma once

#include <queue>

class Mouse
{
    friend class Window;

public:
    class Event
    {
    public:
        enum class Type
        {
            LPress,
            LRelease,
            RPress,
            RRelease,
            WheelUp,
            WheelDown,
            Move,
            Enter,
            Leave,
            Invalid
        };
    private:
        Type type;
        bool bLeftIsPressed;
        bool bRightIsPressed;
        int x;
        int y;
    public:
        Event() noexcept :
            type(Type::Invalid),
            bLeftIsPressed(false),
            bRightIsPressed(false),
            x(0),
            y(0)
        {}
        Event(Type type, const Mouse& parent) noexcept :
            type(type),
            bLeftIsPressed(parent.bLeftIsPressed),
            bRightIsPressed(parent.bRightIsPressed),
            x(parent.x),
            y(parent.y)
        {}
        bool IsValid() const noexcept
        {
            return type != Type::Invalid;
        }
        Type GetType() const noexcept
        {
            return type;
        }
        std::pair<int,int> GetPos() const noexcept
        {
            return{ x,y };
        }
        int GetPosX() const noexcept
        {
            return x;
        }
        int GetPosY() const noexcept
        {
            return y;
        }
        bool LeftIsPressed() const noexcept
        {
            return bLeftIsPressed;
        }
        bool RightIsPressed() const noexcept
        {
            return bRightIsPressed;
        }
    };

public:
    Mouse() = default;
    Mouse(const Mouse&) = delete;
    Mouse& operator=(const Mouse&) = delete;
    std::pair<int, int> GetPos() const noexcept;
    int GetPosX() const noexcept;
    int GetPosY() const noexcept;
    bool IsInWindow() const noexcept;
    bool LeftIsPressed() const noexcept;
    bool RightIsPressed() const noexcept;
    Mouse::Event Read() noexcept;
    bool IsEmpty() const noexcept { return buffer.empty(); }
    void Flush() noexcept;

private:
    void OnMouseMove(int newX, int newY) noexcept;
    void OnMouseLeave() noexcept;
    void OnMouseEnter() noexcept;
    void OnLeftPressed(int newX, int newY) noexcept;
    void OnLeftReleased(int newX, int newY) noexcept;
    void OnRightPressed(int newX, int newY) noexcept;
    void OnRightReleased(int newX, int newY) noexcept;
    void OnWheelUp(int newX, int newY) noexcept;
    void OnWheelDown(int newX, int newY) noexcept;
    void TrimBuffer() noexcept;
    void OnWheelDelta(int newX, int newY, int delta) noexcept;

private:
    static constexpr unsigned int bufferSize = 16u;
    int x = 0;
    int y = 0;
    bool bLeftIsPressed = false;
    bool bRightIsPressed = false;
    bool bIsInWindow = false;
    int wheelDeltaCarry = 0;
    std::queue<Event> buffer;
};