#include "Sence_MainMenu.h"

Sence_MainMenu::Sence_MainMenu() :
	m_pCamera(std::make_shared<Camera>())
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
	m_buttonChangeSence.getLabel().setTextColor(D2D1::ColorF::White);
	m_buttonChangeSence.getLabel().setTextFormat(L"MainMenuButton", L"黑体", nullptr, DWRITE_FONT_WEIGHT_NORMAL,
		DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 30, L"zh-cn");
	m_buttonChangeSence.setRect(300.f, 400.0f, 490.0f, 450.0f);
	m_buttonChangeSence.setNormalFunction([&]() {
		m_buttonChangeSence.getLabel().setTextColor(D2D1::ColorF::White);
		});
	m_buttonChangeSence.setSelectFunction([&]() {
		m_buttonChangeSence.getLabel().setTextColor(D2D1::ColorF::Orange);
		});
	m_buttonChangeSence.setPressFunction([&]() {
		m_senceChangeFunction(std::make_shared<Sence_DriverTest>());
		});

	m_buttonChangeSence.getImage().setSRV(ResourceDepot::getShaderResource(L"brick"));

	//m_buttonChangeSence.select();
	return false;
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

std::shared_ptr<Camera> Sence_MainMenu::getCamera()
{
	return m_pCamera;
}
