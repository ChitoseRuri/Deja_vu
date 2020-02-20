#include "Camera_ThirdPerson.h"
#include "XMF_MATH.h"

using namespace XMF_MATH;

Camera_ThirdPerson::Camera_ThirdPerson() :
	m_phi(0.0f),
	m_theta(0.0f)
{
	setRadius(5.0f);
}

Camera_ThirdPerson::~Camera_ThirdPerson()
{

}

void Camera_ThirdPerson::setRadius(float radius)
{
	m_radius = radius;
	fixView();
}

float Camera_ThirdPerson::getRadius() const
{
	return m_radius;
}

void Camera_ThirdPerson::input(const XMFLOAT3& ir)
{
	m_phi = XMScalarModAngle(m_phi + ir.x);
	m_theta = XMScalarModAngle(m_theta + ir.y);
	m_radius += ir.z;

	fixView();
}

void Camera_ThirdPerson::fixView()
{
	// 将上下视野角度Phi限制在[pi/6, pi/2]，
	// 即余弦值[0, cos(pi/6)]之间
	if (m_phi < XM_PI / 6)
		m_phi = XM_PI / 6;
	else if (m_phi > XM_PIDIV2)
		m_phi = XM_PIDIV2;

	if (m_radius < 2.0f)
		m_radius = 2.0f;
	else if (m_radius > 10.0f)
		m_radius = 10.0f;

	// 球面坐标系
	float x = m_targetL.x + m_radius * sinf(m_phi) * cosf(m_theta);
	float z = m_targetL.z + m_radius * sinf(m_phi) * sinf(m_theta);
	float y = m_targetL.y + m_radius * cosf(m_phi);

	setPosition(x, y, z);

	XMVECTOR P = XMLoadFloat3(&m_localPositon);
	XMVECTOR L = XMVector3Normalize(XMLoadFloat3(&m_targetL) - P);
	XMVECTOR R = XMVector3Normalize(XMVector3Cross(XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f), L));
	XMVECTOR U = XMVector3Cross(L, R);

	// 更新向量
	XMStoreFloat3(&m_targetR, R);
	XMStoreFloat3(&m_targetU, U);
	XMStoreFloat3(&m_targetL, L);
}
