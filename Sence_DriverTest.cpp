#include "Sence_DriverTest.h"

Sence_DriverTest::Sence_DriverTest()
{
}

Sence_DriverTest::~Sence_DriverTest()
{
}

bool Sence_DriverTest::initResource(ID3D11Device* pDevice)
{
	return false;
}

bool Sence_DriverTest::initEffect(ID3D11Device* pDevice)
{
	return false;
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
	return std::shared_ptr<Camera>();
}
