#include "Sence_MainMenu.h"

Sence_MainMenu::Sence_MainMenu() :
	m_pCamera(std::make_shared<Camera>()),
	m_buttonIndex(-1)
{	
}

Sence_MainMenu::~Sence_MainMenu()
{
}

bool Sence_MainMenu::initResource(ID3D11Device* pDevice)
{
	ResourceDepot::loadDDSTesture(pDevice, L"Texture\\brick.dds", L"brick");

	m_pCamera->lookAt(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f));
	
	// 标题
	m_title.setText(L"Deja vu");
	m_title.setRect(300.0f, 100.0f, 600.0f, 300.0f);
	m_title.setTextColor(D2D1::ColorF::PeachPuff);
	m_title.setTextFormat(L"Title", L"Georgia", nullptr, DWRITE_FONT_WEIGHT_NORMAL,
		DWRITE_FONT_STYLE_ITALIC, DWRITE_FONT_STRETCH_NORMAL, 40, L"zh-cn");
	
	// 定义选择测试关卡的按钮的行为和样式
	m_buttonChangeSence.getLabel().setText(L"Driver Test");
	m_buttonChangeSence.getLabel().setTextFormat(L"MainMenuButton", L"Georgia", nullptr, DWRITE_FONT_WEIGHT_NORMAL,
		DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 30, L"zh-cn");
	m_buttonChangeSence.setRect(300.f, 400.0f, 490.0f, 450.0f);
	m_buttonChangeSence.setNormalFunction([&]() {
		m_buttonChangeSence.getLabel().setTextColor(D2D1::ColorF::White);
		m_buttonChangeSence.getImage().setVisable(false);
		});
	m_buttonChangeSence.setSelectFunction([&]() {
		m_buttonChangeSence.getLabel().setTextColor(D2D1::ColorF::Orange);
		m_buttonChangeSence.getImage().setVisable(true);
		});
	m_buttonChangeSence.setPressFunction([&]() {
		switchSence(std::make_shared<Sence_DriverTest>());
		});
	m_buttonChangeSence.normal();
	m_buttonChangeSence.getImage().setSRV(ResourceDepot::getShaderResource(L"brick"));
	m_buttonList.push_back(&m_buttonChangeSence);

	// 定义退出按钮
	m_buttonExit.getLabel().setText(L"Exit");
	m_buttonExit.getLabel().setTextFormat(L"MainMenuButton");
	m_buttonExit.setRect(300.0f, 500.0f, 490.0f, 550.0f);
	m_buttonExit.setNormalFunction([&]() {
		m_buttonExit.getLabel().setTextColor(D2D1::ColorF::White);
		m_buttonExit.getImage().setVisable(false);
		});
	m_buttonExit.setSelectFunction([&]() {
		m_buttonExit.getLabel().setTextColor(D2D1::ColorF::Red);
		m_buttonExit.getImage().setVisable(true);
		});
	m_buttonExit.setPressFunction([&]() {
		switchSence(nullptr);
		});
	m_buttonExit.getImage().setSRV(ResourceDepot::getShaderResource(L"brick"));
	m_buttonExit.normal();
	m_buttonList.push_back(&m_buttonExit);

	m_buttonIndex = 0;
	m_buttonList[m_buttonIndex]->select();
	return true;
}

bool Sence_MainMenu::initEffect(ID3D11Device* pDevice)
{
	return false;
}

void Sence_MainMenu::update(float dt)
{
}

void Sence_MainMenu::draw()
{
	GameObject2D::drawAll();
}

void Sence_MainMenu::beforeResize()
{
	
}

void Sence_MainMenu::afterResize()
{
}

void Sence_MainMenu::input(Keyboard& keyboard, Mouse& mouse, float dt)
{
	if (keyboard.isKeyPress(Keyboard::Keys::Escape))// EXIT
	{
		switchSence(nullptr);
	}
	if (keyboard.isKeyPress(Keyboard::Keys::Enter))
	{
		m_buttonList[m_buttonIndex]->press();
	}
	if (keyboard.isKeyPress(Keyboard::Keys::W))
	{
		if (m_buttonIndex > 0)
		{
			m_buttonList[m_buttonIndex]->normal();
			m_buttonList[--m_buttonIndex]->select();
		}
	}
	if (keyboard.isKeyPress(Keyboard::Keys::S))
	{
		if (m_buttonIndex + 1 < m_buttonList.size())
		{
			if (m_buttonIndex != -1)
			{
				m_buttonList[m_buttonIndex]->normal();
			}
			m_buttonList[++m_buttonIndex]->select();
		}
	}

	if (keyboard.isKeyPress(Keyboard::Keys::G))
	{
		if (mouse.isLock())
		{
			mouse.lock();
		}
		else
		{
			mouse.unlock();
		}
	}

	// 处理鼠标选择按键的行为
	auto size = m_buttonList.size();
	for (size_t i = 0; i < size; ++i)
	{
		if (m_buttonList[i]->isPointInRect(mouse.getMousePos()))
		{
			if (i != m_buttonIndex)
			{
				if (m_buttonIndex != -1)
				{
					m_buttonList[m_buttonIndex]->normal();
				}
				m_buttonList[i]->select();
				m_buttonIndex = i;
			}
			break;
		}
		else
		{
			/*if (i == m_buttonIndex)
			{
				m_buttonList[m_buttonIndex]->normal();
				m_buttonIndex = -1;
			}*/
		}
	}
	// 处理鼠标左键按下的行为
	if (mouse.isButtonPress(Mouse::Button::LeftButton) && m_buttonIndex != -1)
	{
		if (m_buttonList[m_buttonIndex]->isPointInRect(mouse.getMousePos()))
			m_buttonList[m_buttonIndex]->press();
	}
}

std::shared_ptr<Camera> Sence_MainMenu::getCamera()
{
	return m_pCamera;
}
