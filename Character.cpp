#include "Character.h"

using namespace XMF_MATH;

Character::Character()
{
}

Character::~Character()
{
}

const Camera& Character::getCamera() const
{
	return m_camera;
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

void Character::update(float dt)
{
	updateSpeed(dt);
}

void Character::updateSpeed(float dt)
{
	// 计算加速后的速度
	m_speedVector += (m_acceleration * dt);
	// 计算阻力后的速度
	if (abs(m_speedVector.x) < abs(m_resistance.x))
	{
		m_speedVector.x = 0.0f;
	}
	else
	{
		m_speedVector.x += m_resistance.x;
	}
	if (abs(m_speedVector.y) < abs(m_resistance.y))
	{
		m_speedVector.y = 0.0f;
	}
	else
	{
		m_speedVector.y += m_resistance.y;
	}
	if (abs(m_speedVector.z) < abs(m_resistance.z))
	{
		m_speedVector.z = 0.0f;
	}
	else
	{
		m_speedVector.z += m_resistance.z;
	}

	setLocation(m_location + (m_speedVector * dt));
}
