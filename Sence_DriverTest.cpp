#include "Sence_DriverTest.h"

Sence_DriverTest::Sence_DriverTest()
{
}

Sence_DriverTest::~Sence_DriverTest()
{
}

bool Sence_DriverTest::initResource(ID3D11Device* pDevice)
{
	// init UI
	m_labelSpeed.setRect(0.0f, 0.0f, 200.0f, 20.0f);
	m_labelSpeed.setTextColor(D2D1::ColorF::PaleVioletRed);
	m_labelSpeed.setTextFormat(L"HUD", L"Georgia", nullptr, DWRITE_FONT_WEIGHT_NORMAL,
		DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 20, L"zh-cn");
	m_labelSpeed.setActive(true);
	m_labelSpeed.setUpdateFunction([&](float dt) {
		m_labelSpeed.setText(L"Speed:" + std::to_wstring(m_car.getSpeed()));
		});
	m_labelSpeed.setText(L"Speed:" + std::to_wstring(0.0f));

	// init 3d object
	m_car.initCar(pDevice);
	m_land.setTexture(ResourceDepot::getShaderResource(ResourceDepot::loadImage(pDevice, L"Texture\\plane.jpg", L"plane")));
	auto mesh = Geometry::CreatePlane();
	for (auto& hs : mesh.vertexVec)
	{
		hs.tex.x *= 100.0f;
		hs.tex.y *= 100.0f;
	}
	m_land.setMeshbuffer(ResourceDepot::getMeshBuffer(ResourceDepot::loadGeometry(pDevice, mesh, L"plane")));
	m_land.setPosition(0.0f, -0.5f, 0.0f);
	m_land.setScale(100.0f, 0.0f, 100.0f);
	m_car.viewRotate({ 0.0f,0.0f,0.0f });
	m_box.setMeshbuffer(ResourceDepot::getMeshBuffer(ResourceDepot::loadGeometry(pDevice, Geometry::CreateBox(), L"box")));
	m_box.setTexture(ResourceDepot::getShaderResource(ResourceDepot::loadDDSTesture(pDevice, L"Texture\\WoodCrate.dds", L"box")));
	m_box.setPosition(5.0f, 0.5f, 5.0f);
	//m_car.setRotation(0.0f, 45.0f, 0.0f);
	//m_car.setPosition(1.0f, 0.0f, 1.0f);
	return true;
}

bool Sence_DriverTest::initEffect(ID3D11Device* pDevice)
{
	return true;
}

void Sence_DriverTest::update(float dt)
{
}

void Sence_DriverTest::draw()
{
}

void Sence_DriverTest::beforeResize()
{
}

void Sence_DriverTest::afterResize()
{
}

void Sence_DriverTest::input(Keyboard& keyboard, Mouse& mouse, float dt)
{
	if (keyboard.isKeyPress(Keyboard::Keys::Escape))
	{
		Sence::switchSence(nullptr);
	}
	if (keyboard.isKeyPress(Keyboard::Keys::G))
	{
		if (mouse.isLock())
		{
			mouse.unlock();
		}
		else
		{
			mouse.lock();
		}
	}
	if (keyboard.isKeyDown(Keyboard::Keys::W))
	{
		m_car.straight(dt);
	}
	if (keyboard.isKeyDown(Keyboard::Keys::S))
	{
		m_car.backup(dt);
	}
	if (keyboard.isKeyDown(Keyboard::Keys::A))
	{
		m_car.left(dt);
	}
	if (keyboard.isKeyDown(Keyboard::Keys::D))
	{
		m_car.right(dt);
	}
	if (keyboard.isKeyDown(Keyboard::Keys::Space))
	{
		m_car.brakeOn(dt);
	}
	else
	{
		m_car.brakeOff(dt);
	}
	if (keyboard.isKeyPress(Keyboard::Keys::T))
	{
		m_car.changeCamera();
	}
	if (keyboard.isKeyDown(Keyboard::Keys::I))
	{
		m_car.setCameraTRadius(m_car.getCameraTRadius() - 10.0f * dt);
	}
	if (keyboard.isKeyDown(Keyboard::Keys::O))
	{
		m_car.setCameraTRadius(m_car.getCameraTRadius() + 10.0f * dt);
	}
	// 鼠标输入
	auto rm = mouse.getRelativeMovement();
	if (rm.x || rm.y)
	{
		m_car.viewRotate({ dt * rm.y, dt * rm.x, 0.0f });
	}
}

std::shared_ptr<Camera> Sence_DriverTest::getCamera()
{
	return m_car.getCamera();
}
