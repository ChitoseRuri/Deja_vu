#include "Camera.h"

using namespace XMF_MATH;

Camera::Camera():
	m_Right(0.0f, 0.0f, 0.0f),
	m_Up(0.0f, 0.0f, 0.0f), m_Look(0.0f, 0.0f, 0.0f),
	m_nearZ(), m_farZ(), m_fovY(), m_aspect(),
	m_NearWindowHeight(), m_FarWindowHeight(),
	m_View(), m_proj(), m_ViewPort()
{
	setVisable(false);
}

Camera::~Camera()
{
}

XMVECTOR Camera::getLocationXM() const
{
	return XMLoadFloat3(&m_location);
}

XMVECTOR Camera::getRightXM() const
{
	return XMLoadFloat3(&m_Right);
}

XMFLOAT3 Camera::getRight() const
{
	return m_Right;
}

XMVECTOR Camera::getUpXM() const
{
	return XMLoadFloat3(&m_Up);
}

XMFLOAT3 Camera::getUp() const
{
	return m_Up;
}

XMVECTOR Camera::getLookXM() const
{
	return XMLoadFloat3(&m_Look);
}

XMFLOAT3 Camera::getLook() const
{
	return m_Look;
}

XMMATRIX Camera::getViewXM() const
{
	return XMLoadFloat4x4(&m_View);
}

XMMATRIX Camera::getProjXM() const
{
	return XMLoadFloat4x4(&m_proj);
}

XMMATRIX Camera::getViewProjXM() const
{
	return XMLoadFloat4x4(&m_View) * XMLoadFloat4x4(&m_proj);
}

D3D11_VIEWPORT Camera::getViewPort() const
{
	return m_ViewPort;
}

void Camera::setFrustum(float fovY, float aspect, float nearZ, float farZ)
{
	m_fovY = fovY;
	m_aspect = aspect;
	m_nearZ = nearZ;
	m_farZ = farZ;

	m_NearWindowHeight = 2.0f * m_nearZ * tanf(0.5f * m_fovY);
	m_FarWindowHeight = 2.0f * m_farZ * tanf(0.5f * m_fovY);

	XMStoreFloat4x4(&m_proj, XMMatrixPerspectiveFovLH(m_fovY, m_aspect, m_nearZ, m_farZ));
}

void Camera::setViewPort(const D3D11_VIEWPORT& viewPort)
{
	m_ViewPort = viewPort;
}

void Camera::setViewPort(float topLeftX, float topLeftY, float width, float height, float minDepth, float maxDepth)
{
	m_ViewPort.TopLeftX = topLeftX;
	m_ViewPort.TopLeftY = topLeftY;
	m_ViewPort.Width = width;
	m_ViewPort.Height = height;
	m_ViewPort.MinDepth = minDepth;
	m_ViewPort.MaxDepth = maxDepth;
}

void XM_CALLCONV Camera::lookAt(DirectX::FXMVECTOR pos, DirectX::FXMVECTOR target, DirectX::FXMVECTOR up)
{
	lookTo(pos, target - pos, up);
}

void Camera::lookAt(const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& target, const DirectX::XMFLOAT3& up)
{
	lookAt(XMLoadFloat3(&pos), XMLoadFloat3(&target), XMLoadFloat3(&up));
}

void XM_CALLCONV Camera::lookTo(DirectX::FXMVECTOR pos, DirectX::FXMVECTOR to, DirectX::FXMVECTOR up)
{
	XMVECTOR L = XMVector3Normalize(to);
	XMVECTOR R = XMVector3Normalize(XMVector3Cross(up, L));
	XMVECTOR U = XMVector3Cross(L, R);

	XMStoreFloat3(&m_location, pos);
	XMStoreFloat3(&m_Look, L);
	XMStoreFloat3(&m_Right, R);
	XMStoreFloat3(&m_Up, U);
}

void Camera::lookTo(const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& to, const DirectX::XMFLOAT3& up)
{
	lookTo(XMLoadFloat3(&pos), XMLoadFloat3(&to), XMLoadFloat3(&up));
}

void Camera::update(float dt)
{
	updateLocalMatrix();

	XMVECTOR R = XMLoadFloat3(&m_Right);
	XMVECTOR U = XMLoadFloat3(&m_Up);
	XMVECTOR L = XMLoadFloat3(&m_Look);
	XMVECTOR P = XMLoadFloat3(&(m_location + m_locationP));

	// 保持摄像机的轴互为正交，且长度都为1
	L = XMVector3Normalize(L);
	U = XMVector3Normalize(XMVector3Cross(L, R));

	// U, L已经正交化，需要计算对应叉乘得到R
	R = XMVector3Cross(U, L);

	// 填充观察矩阵
	float x = -XMVectorGetX(XMVector3Dot(P, R));
	float y = -XMVectorGetX(XMVector3Dot(P, U));
	float z = -XMVectorGetX(XMVector3Dot(P, L));

	XMStoreFloat3(&m_Right, R);
	XMStoreFloat3(&m_Up, U);
	XMStoreFloat3(&m_Look, L);

	m_View = {
		m_Right.x, m_Up.x, m_Look.x, 0.0f,
		m_Right.y, m_Up.y, m_Look.y, 0.0f,
		m_Right.z, m_Up.z, m_Look.z, 0.0f,
		x, y, z, 1.0f
	};

	for (auto child : m_childen)
	{
		child->update(dt);
	}
}

const CBWorld& Camera::getWorldCB() const
{
	return m_cbWorld;
}
