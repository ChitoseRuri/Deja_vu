#include "Mouse.h"
#include <windowsx.h>

constexpr int MOUSE_LENGTH = static_cast<int>(Mouse::Button::ButonEnd);
constexpr bool BUTTON_UP = false;
constexpr bool BUTTON_DOWN = true;
constexpr int MENU_HEIGHT = 32;// 窗口模式菜单栏的高度

Mouse::Mouse():
	m_indexNow(0),
	m_indexLast(1),
	m_posLockS({-1, -1})
{
	m_status[0].resize(MOUSE_LENGTH, BUTTON_UP);
	m_status[1].resize(MOUSE_LENGTH, BUTTON_UP);
}

Mouse::~Mouse()
{
	ClipCursor(nullptr);// xp和2000下无效
}

void Mouse::afterResize()
{
	RECT rect;
	GetWindowRect(m_hwnd, &rect);
	rect.top += MENU_HEIGHT;
	ClipCursor(&rect);
}

void Mouse::afterResize(HWND hwnd)
{
	m_hwnd = hwnd;
	RECT rect;
	GetWindowRect(hwnd, &rect);
	rect.top += MENU_HEIGHT;
	ClipCursor(&rect);
}

void Mouse::setMousePos(WPARAM lParam)
{
	m_posNow = { GET_X_LPARAM(lParam) , GET_Y_LPARAM(lParam) };
}

POINT Mouse::getMousePos() const
{
	return m_posNow;
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
	if (m_posLockS.x != -1)
	{
		SetCursorPos(m_posLockS.x, m_posLockS.y);
		m_posLast = m_posLockS;
	}
	else
	{
		m_posLast = m_posNow;
	}
}

void Mouse::lock()
{
	RECT rect;
	GetWindowRect(m_hwnd, &rect);
	m_posLockC = m_posLockS = { (rect.right - rect.left) >> 1, (rect.bottom - rect.top) >> 1 };
	ClientToScreen(m_hwnd, &m_posLockS);
}

void Mouse::lock(const POINT& pos)
{
	assert(pos.x > 0 && pos.y > 0);
	m_posLockC = m_posLockS = pos;
	ClientToScreen(m_hwnd, &m_posLockS);
}

void Mouse::unlock()
{
	m_posLockS = { -1, -1 };
}

bool Mouse::isLock() const
{
	return m_posLockS.x != -1;
}

POINT Mouse::getRelativeMovement() const
{
	if (m_posLockS.x == -1)
	{
		return { m_posNow.x - m_posLast.x, m_posNow.y - m_posLast.y};
	}
	else
	{
		return{ m_posNow.x - m_posLockC.x, m_posNow.y - m_posLockC.y };
	}
}
