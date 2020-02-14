#include "Sence_DriverTest.h"

Sence_DriverTest::Sence_DriverTest()
{
}

Sence_DriverTest::~Sence_DriverTest()
{
}

bool Sence_DriverTest::initResource(ID3D11Device* pDevice)
{
	m_car.init(pDevice);
	m_car.setRotation(0.0f, 90.0f, 0.0f);
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

std::shared_ptr<Camera> Sence_DriverTest::getCamera()
{
	return m_car.getCamera();
}
