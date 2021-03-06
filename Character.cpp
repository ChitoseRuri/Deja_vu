#include "Character.h"

using namespace XMF_MATH;

Character::Character() :
	m_acceleration({0.0f, 0.0f, 0.0f}),
	m_resistance({ 0.0f, 0.0f, 0.0f }),
	m_speedVector({ 0.0f, 0.0f, 0.0f })
{
}

Character::~Character()
{
}

void Character::init()
{
	m_pCamera = std::shared_ptr<Camera>(new Camera);
	addChild(&*m_pCamera);
	m_pCamera->lookAt(XMFLOAT3(0.0f, 0.0f, -4.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f));
}

std::shared_ptr<Camera> Character::getCamera() const
{
	return m_pCamera;
}

void Character::setSpeedVector(const XMFLOAT3& speed)
{
	m_speedVector = speed;
}

void Character::setSpeedVector(float x, float y, float z)
{
	m_speedVector = { x, y, z };
}

const XMFLOAT3& Character::getSpeedVector() const
{
	return m_speedVector;
}

void Character::setAcceleration(const XMFLOAT3& acceleration)
{
	m_acceleration = acceleration;
}

void Character::setAcceleration(float x, float y, float z)
{ 
	m_acceleration = { x, y, z };
}

const XMFLOAT3& Character::getAcceleration() const
{
	return m_acceleration;
}

void Character::setResistance(const XMFLOAT3& acceleration)
{
	m_resistance = acceleration;
}

void Character::setResistance(float x, float y, float z)
{
	m_resistance = { x,y,z };
}

const XMFLOAT3& Character::getResistance() const
{
	return m_resistance;
}

void Character::update(float dt)// 弃用
{
	setPosition(m_localPositon + (m_speedVector * dt));
	updateLocalMatrix();
	for (auto child : m_childen)
	{
		child->update(dt);
	}
}