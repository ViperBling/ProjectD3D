#include "Keyboard.h"

bool Keyboard::KeyIsPressed(unsigned char keycode) const noexcept
{
    return keystates[keycode];
}

Keyboard::Event Keyboard::ReadKey() noexcept
{
    if (!keybuffer.empty())
    {
        Keyboard::Event event = keybuffer.front();
        keybuffer.pop();
        return event;
    } else
    {
        return {};
    }
}

bool Keyboard::KeyIsEmpty() const noexcept
{
    return keybuffer.empty();
}

void Keyboard::FlushKey() noexcept
{
    keybuffer = std::queue<Event>();
}

char Keyboard::ReadChar() noexcept
{
    if (!charbuffer.empty())
    {
        unsigned char charcode = charbuffer.front();
        charbuffer.pop();
        return charcode;
    } else
    {
        return 0;
    }
}

bool Keyboard::CharIsEmpty() const noexcept
{
    return charbuffer.empty();
}

void Keyboard::FlushChar() noexcept
{
    charbuffer = std::queue<char>();
}

void Keyboard::Flush() noexcept
{
    FlushKey();
    FlushChar();
}

void Keyboard::EnableAutorepeat() noexcept
{
    bAutorepeatEnabled = true;
}

void Keyboard::DisableAutorepeat() noexcept
{
    bAutorepeatEnabled = false;
}

bool Keyboard::AutorepeatIsEnabled() const noexcept
{
    return bAutorepeatEnabled;
}

void Keyboard::OnKeyPressed(unsigned char keycode) noexcept
{
    keystates[keycode] = true;
    keybuffer.push(Keyboard::Event(Keyboard::Event::Type::Press, keycode));
    TrimBuffer(keybuffer);
}

void Keyboard::OnKeyReleased(unsigned char keycode) noexcept
{
    keystates[keycode] = false;
    keybuffer.push(Keyboard::Event(Keyboard::Event::Type::Release,keycode));
    TrimBuffer(keybuffer);
}

void Keyboard::OnChar(char character) noexcept
{
    charbuffer.push(character);
    TrimBuffer(charbuffer);
}

void Keyboard::ClearState() noexcept
{
    keystates.reset();
}

template<class T>
void Keyboard::TrimBuffer(std::queue<T> &buffer) noexcept
{
    while (buffer.size() > bufferSize)
    {
        buffer.pop();
    }
}