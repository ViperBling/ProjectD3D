#include "Utility/D3D11Win.h"
#include "Mouse.h"

std::pair<int, int> Mouse::GetPos() const noexcept
{
    return {x, y};
}

int Mouse::GetPosX() const noexcept
{
    return x;
}

int Mouse::GetPosY() const noexcept
{
    return y;
}

bool Mouse::IsInWindow() const noexcept
{
    return bIsInWindow;
}

bool Mouse::LeftIsPressed() const noexcept
{
    return bLeftIsPressed;
}

bool Mouse::RightIsPressed() const noexcept
{
    return bRightIsPressed;
}

std::optional<Mouse::Event> Mouse::Read() noexcept
{
    if (!buffer.empty())
    {
        Mouse::Event event = buffer.front();
        buffer.pop();
        return event;
    }
    return {};
}

void Mouse::Flush() noexcept
{
    buffer = std::queue<Event>();
}

void Mouse::OnMouseMove(int newX, int newY) noexcept
{
    x = newX;
    y = newY;

    buffer.push(Mouse::Event(Mouse::Event::Type::Move, *this));
    TrimBuffer();
}

void Mouse::OnMouseLeave() noexcept
{
    bIsInWindow = false;
    buffer.push(Mouse::Event(Mouse::Event::Type::Leave, *this));
    TrimBuffer();
}

void Mouse::OnMouseEnter() noexcept
{
    bIsInWindow = true;
    buffer.push( Mouse::Event( Mouse::Event::Type::Enter,*this ) );
    TrimBuffer();
}

void Mouse::OnLeftPressed(int newX, int newY) noexcept
{
    bLeftIsPressed = true;

    buffer.push( Mouse::Event( Mouse::Event::Type::LPress,*this ) );
    TrimBuffer();
}

void Mouse::OnLeftReleased(int newX, int newY) noexcept
{
    bLeftIsPressed = false;

    buffer.push( Mouse::Event( Mouse::Event::Type::LRelease,*this ) );
    TrimBuffer();
}

void Mouse::OnRightPressed(int newX, int newY) noexcept
{
    bRightIsPressed = true;

    buffer.push( Mouse::Event( Mouse::Event::Type::RPress,*this ) );
    TrimBuffer();
}

void Mouse::OnRightReleased(int newX, int newY) noexcept
{
    bRightIsPressed = false;

    buffer.push( Mouse::Event( Mouse::Event::Type::RRelease,*this ) );
    TrimBuffer();
}

void Mouse::OnWheelUp(int newX, int newY) noexcept
{
    buffer.push( Mouse::Event( Mouse::Event::Type::WheelUp,*this ) );
    TrimBuffer();
}

void Mouse::OnWheelDown(int newX, int newY) noexcept
{
    buffer.push( Mouse::Event( Mouse::Event::Type::WheelDown,*this ) );
    TrimBuffer();
}

void Mouse::TrimBuffer() noexcept
{
    while (buffer.size() > bufferSize)
    {
        buffer.pop();
    }
}

void Mouse::OnWheelDelta(int newX, int newY, int delta) noexcept
{
    wheelDeltaCarry += delta;
    // generate events for every 120 
    while( wheelDeltaCarry >= WHEEL_DELTA )
    {
        wheelDeltaCarry -= WHEEL_DELTA;
        OnWheelUp(newX, newY);
    }
    while( wheelDeltaCarry <= -WHEEL_DELTA )
    {
        wheelDeltaCarry += WHEEL_DELTA;
        OnWheelDown(newX, newY);
    }
}