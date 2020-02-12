#include "Mouse.h"
#include <windowsx.h>

constexpr int MOUSE_LENGTH = static_cast<int>(Mouse::Button::ButonEnd);
constexpr bool BUTTON_UP = false;
constexpr bool BUTTON_DOWN = true;

Mouse::Mouse():
	m_indexNow(0),
	m_indexLast(1)
{
	m_status[0].resize(MOUSE_LENGTH, BUTTON_UP);
	m_status[1].resize(MOUSE_LENGTH, BUTTON_UP);
}

Mouse::~Mouse()
{
}

void Mouse::afterResize()
{
	RECT rect;
	GetWindowRect(m_hwnd, &rect);
	ClipCursor(&rect);
}

void Mouse::afterResize(HWND hwnd)
{
	m_hwnd = hwnd;
	RECT rect;
	GetWindowRect(hwnd, &rect);
	ClipCursor(&rect);
}

void Mouse::setMousePos(WPARAM lParam)
{
	m_pos = { GET_X_LPARAM(lParam) , GET_Y_LPARAM(lParam) };
}

DirectX::XMINT2 Mouse::getMousePos() const
{
	return m_pos;
}

void Mouse::buttonUp(Button button)
{
	m_status[m_indexNow][static_cast<unsigned>(button)] = BUTTON_UP;
}

void Mouse::buttonDown(Button button)
{
	m_status[m_indexNow][static_cast<unsigned>(button)] = BUTTON_DOWN;
}

bool Mouse::isButonUp(Button button) const
{
	return m_status[m_indexNow][static_cast<unsigned>(button)] == BUTTON_UP;
}

bool Mouse::isButtonDown(Button button) const
{
	return m_status[m_indexNow][static_cast<unsigned>(button)] == BUTTON_DOWN;
}

bool Mouse::isButtonRelease(Button button) const
{
	return m_status[m_indexNow][static_cast<unsigned>(button)] == BUTTON_UP && m_status[m_indexLast][static_cast<unsigned>(button)] == BUTTON_DOWN;
}

bool Mouse::isButtonPress(Button button) const
{
	return m_status[m_indexNow][static_cast<unsigned>(button)] == BUTTON_DOWN && m_status[m_indexLast][static_cast<unsigned>(button)] == BUTTON_UP;
}

void Mouse::update()
{
	m_indexLast ^= 1;
	m_indexNow ^= 1;
	
	m_status[m_indexNow] = m_status[m_indexLast];
}