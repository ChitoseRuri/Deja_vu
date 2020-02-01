#include "Sence.h"

Sence::Sence()
{
}

Sence::~Sence()
{
}

void Sence::init(std::shared_ptr<DirectX::Mouse> mouse, std::shared_ptr<DirectX::Keyboard> keyBoard)
{
	m_pMouse = mouse;
	m_pKeyboard = keyBoard;

	if (!initResource())
	{

	}

	if (!initEffect())
	{

	}
}
