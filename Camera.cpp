#include "Camera.h"

using namespace XMF_MATH;

Camera::Camera():
	m_targetR(1.0f, 0.0f, 0.0f), 
	m_targetU(0.0f, 1.0f, 0.0f), m_targetL(0.0f, 0.0f, 1.0f),
	m_nearZ(), m_farZ(), m_fovY(), m_aspect(),
	m_nearWindowHeight(), m_farWindowHeight(),
	m_view(), m_proj(), m_viewPort()
{
	setVisable(false);
}

Camera::~Camera()
{
}

XMVECTOR Camera::getLocationXM() const
{
	return XMLoadFloat3(&m_localPositon);
}

XMVECTOR Camera::getRightXM() const
{
	return XMLoadFloat3(&m_targetR);
}

XMFLOAT3 Camera::getRight() const
{
	return m_targetR;
}

XMVECTOR Camera::getUpXM() const
{
	return XMLoadFloat3(&m_targetU);
}

XMFLOAT3 Camera::getUp() const
{
	return m_targetU;
}

XMVECTOR Camera::getLookXM() const
{
	return XMVector3Transform(XMLoadFloat3(&m_targetL) - XMLoadFloat3(&m_localPositon),m_worldMatrix);
}

XMMATRIX Camera::getViewXM() const
{
	return XMLoadFloat4x4(&m_view);
}

XMMATRIX Camera::getProjXM() const
{
	return XMLoadFloat4x4(&m_proj);
}

XMMATRIX Camera::getViewProjXM() const
{
	return XMLoadFloat4x4(&m_view) * XMLoadFloat4x4(&m_proj);
}

D3D11_VIEWPORT Camera::getViewPort() const
{
	return m_viewPort;
}

void Camera::setFrustum(float fovY, float aspect, float nearZ, float farZ)
{
	m_fovY = fovY;
	m_aspect = aspect;
	m_nearZ = nearZ;
	m_farZ = farZ;

	m_nearWindowHeight = 2.0f * m_nearZ * tanf(0.5f * m_fovY);
	m_farWindowHeight = 2.0f * m_farZ * tanf(0.5f * m_fovY);

	XMStoreFloat4x4(&m_proj, XMMatrixPerspectiveFovLH(m_fovY, m_aspect, m_nearZ, m_farZ));
}

void Camera::setViewPort(const D3D11_VIEWPORT& viewPort)
{
	m_viewPort = viewPort;
}

void Camera::setViewPort(float topLeftX, float topLeftY, float width, float height, float minDepth, float maxDepth)
{
	m_viewPort.TopLeftX = topLeftX;
	m_viewPort.TopLeftY = topLeftY;
	m_viewPort.Width = width;
	m_viewPort.Height = height;
	m_viewPort.MinDepth = minDepth;
	m_viewPort.MaxDepth = maxDepth;
}

void XM_CALLCONV Camera::lookAt(DirectX::FXMVECTOR pos, DirectX::FXMVECTOR target, DirectX::FXMVECTOR up)
{
	auto L = target - pos;
	auto R = XMVector3Normalize(XMVector3Cross(up, L));

	XMStoreFloat3(&m_localPositon, pos);
	XMStoreFloat3(&m_targetL, L);
	XMStoreFloat3(&m_targetU, up);
	XMStoreFloat3(&m_targetR, R);

	m_trans |= Trans::locate_t;
}

void Camera::lookAt(const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& target, const DirectX::XMFLOAT3& up)
{
	lookAt(XMLoadFloat3(&pos), XMLoadFloat3(&target), XMLoadFloat3(&up));
}

void XM_CALLCONV Camera::lookTo(DirectX::FXMVECTOR pos, DirectX::FXMVECTOR to, DirectX::FXMVECTOR up)
{
	XMStoreFloat3(&m_localPositon, pos);
	XMStoreFloat3(&m_targetL, to);
	XMStoreFloat3(&m_targetU, up);
	XMStoreFloat3(&m_targetR, XMVector3Cross(up, to));

	m_trans = Trans::locate_t;
}

void Camera::lookTo(const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& to, const DirectX::XMFLOAT3& up)
{
	lookTo(XMLoadFloat3(&pos), XMLoadFloat3(&to), XMLoadFloat3(&up));
}

void Camera::update(float dt)
{
	updateLocalMatrix();
	updateView();
	for (auto child : m_childen)
	{
		child->update(dt);
	}
}

void Camera::updateLocalMatrix()
{
 	GameObject3D::updateLocalMatrix();
}

void Camera::updateWorldMatrix()
{
	GameObject3D::updateWorldMatrix();
}

void Camera::updateView()
{
	// 控制条件： R与视线相交，但U、L未必垂直
	XMVECTOR TR = XMVector3Transform(XMLoadFloat3(&m_targetR),m_worldMatrix);
	XMVECTOR TU = XMVector3Transform(XMLoadFloat3(&m_targetU),m_worldMatrix);
	XMVECTOR TL = XMVector3Transform(XMLoadFloat3(&m_targetL),m_worldMatrix);
	XMVECTOR P = XMVector3Transform(XMVectorZero(), m_worldMatrix);

	XMVECTOR L = XMVector3Normalize(TL - P);
	XMVECTOR R = XMVector3Normalize(TR - P);
	XMVECTOR U = XMVector3Normalize(TU - P);

	XMFLOAT3 right, up, look;

	XMStoreFloat3(&right, R);
	XMStoreFloat3(&up, U);
	XMStoreFloat3(&look, L);

	// 填充观察矩阵
	m_view = {
		right.x, up.x, look.x, 0.0f,
		right.y, up.y, look.y, 0.0f,
		right.z, up.z, look.z, 0.0f,
		-XMVectorGetX(XMVector3Dot(P, R)), -XMVectorGetX(XMVector3Dot(P, U)), -XMVectorGetX(XMVector3Dot(P, L)), 1.0f
	};
}

const CBWorld& Camera::getWorldCB() const
{
	return m_cbWorld;
}

void Camera::input(const XMFLOAT3& ir)
{
}
