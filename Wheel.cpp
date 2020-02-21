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
	setMeshbuffer(ResourceDepot::getMeshBuffer(ResourceDepot::loadGeometry(pDevice, createWheel(), L"cylinder")));
	setRotation(0.0f, 0.0f, 90.0f);
	setVisable(true);
}

void Wheel::rotWheel(float angle)
{
	m_localRotation.y = angle;
	m_trans |= Trans::rotate_t;
}

void Wheel::rollWheel(float distance)
{
	m_localRotation.x += toAngle(XMScalarModAngle(distance));
	m_trans |= Trans::rotate_t;
}
