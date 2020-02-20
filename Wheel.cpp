#include "Wheel.h"
#include "XMF_MATH.h"
using namespace XMF_MATH;

Wheel::Wheel()
{
	
}

Wheel::~Wheel()
{
}

void Wheel::init(ID3D11Device* pDevice)
{
	setTexture(ResourceDepot::getShaderResource(ResourceDepot::loadImage(pDevice, L"Texture\\wheel.png", L"wheel")));
	setMeshbuffer(ResourceDepot::getMeshBuffer(ResourceDepot::loadGeometry(pDevice, Geometry::CreateCylinder(), L"cylinder")));
	setRotation(0.0f, 0.0f, 90.0f);
	setVisable(true);
}

void Wheel::rotWheel(float angle)
{
	setRotation(angle, 0.0F, 90.0f);
}

void Wheel::rollWheel(float distance)
{
	setRotation(m_localRotation - XMFLOAT3(0.0f, toAngle(XMScalarModAngle(distance)), 0.0f));
}
