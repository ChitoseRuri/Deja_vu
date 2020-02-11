#include "Keyboard.h"

#define KEYDOWN true
#define KEYUP false
constexpr int KEYS_LENGTH = 256;// bool vector的长度，即Keys枚举的长度

Keyboard::Keyboard():
	m_indexNow(0),
	m_indexLast(1)
{
	m_status[0].resize(KEYS_LENGTH, KEYUP);
	m_status[1].resize(KEYS_LENGTH, KEYUP);
}

Keyboard::~Keyboard()
{
}


void Keyboard::keyDown(Keys key)
{
	m_status[m_indexNow][static_cast<unsigned>(key)] = KEYDOWN;
}

bool Keyboard::isKeyDown(Keys key) const
{
	return m_status[m_indexNow][static_cast<unsigned>(key)] == KEYDOWN;
}

void Keyboard::keyUp(Keys key) 
{
	m_status[m_indexNow][static_cast<unsigned>(key)] = KEYUP;
}

bool Keyboard::isKeyUp(Keys key) const
{
	return m_status[m_indexNow][static_cast<unsigned>(key)] == KEYUP;
}

bool Keyboard::isKeyPress(Keys key) const
{
	return m_status[m_indexLast][static_cast<unsigned>(key)] == KEYUP && m_status[m_indexNow][static_cast<unsigned>(key)] == KEYDOWN;
}

bool Keyboard::isKeyRelease(Keys key) const
{
	return m_status[m_indexLast][static_cast<unsigned>(key)] == KEYDOWN && m_status[m_indexNow][static_cast<unsigned>(key)] == KEYUP;
}

void Keyboard::update()
{
	m_indexNow ^= 1;
	m_indexLast ^= 1;

	m_status[m_indexNow] = m_status[m_indexLast];// 覆盖并延续掉keydown的状态
}
