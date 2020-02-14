#include "Camera_FirstPerson.h"
#include "XMF_MATH.h"

Camera_FirstPerson::Camera_FirstPerson() :
	m_targetL({0.0f, 0.0f, 1.0f})
{
}

Camera_FirstPerson::~Camera_FirstPerson()
{
}

void Camera_FirstPerson::setPosition(float x, float y, float z)
{
	setPosition({ x, y, z });
}

void Camera_FirstPerson::setPosition(const XMFLOAT3& location)
{
}

void Camera_FirstPerson::setScale(float x, float y, float z)
{
	setScale({ x,y,z });
}

void Camera_FirstPerson::setScale(const XMFLOAT3& scale)
{
}

void Camera_FirstPerson::setRotation(float x, float y, float z)
{
	setRotation({ x,y,z });
}

void Camera_FirstPerson::setRotation(const XMFLOAT3& rotation)
{
}
