#include "CameraN.h"

CameraN::CameraN():
	m_Right(0.0f, 0.0f, 0.0f),
	m_Up(0.0f, 0.0f, 0.0f), m_Look(0.0f, 0.0f, 0.0f),
	m_NearZ(), m_FarZ(), m_FovY(), m_Aspect(),
	m_NearWindowHeight(), m_FarWindowHeight(),
	m_View(), m_Proj(), m_ViewPort()
{
	setActiveUpdate(true);
	setVisable(false);
}

CameraN::~CameraN()
{
}

XMVECTOR CameraN::getLocationXM() const
{
	return XMLoadFloat3(&m_location);
}

XMVECTOR CameraN::GetRightXM() const
{
	return XMLoadFloat3(&m_Right);
}

XMFLOAT3 CameraN::GetRight() const
{
	return m_Right;
}

XMVECTOR CameraN::GetUpXM() const
{
	return XMLoadFloat3(&m_Up);
}

XMFLOAT3 CameraN::GetUp() const
{
	return m_Up;
}

XMVECTOR CameraN::GetLookXM() const
{
	return XMLoadFloat3(&m_Look);
}

XMFLOAT3 CameraN::GetLook() const
{
	return m_Look;
}

XMMATRIX CameraN::GetViewXM() const
{
	return XMLoadFloat4x4(&m_View);
}

XMMATRIX CameraN::GetProjXM() const
{
	return XMLoadFloat4x4(&m_Proj);
}

XMMATRIX CameraN::GetViewProjXM() const
{
	return XMLoadFloat4x4(&m_View) * XMLoadFloat4x4(&m_Proj);
}

D3D11_VIEWPORT CameraN::GetViewPort() const
{
	return m_ViewPort;
}

void CameraN::SetFrustum(float fovY, float aspect, float nearZ, float farZ)
{
	m_FovY = fovY;
	m_Aspect = aspect;
	m_NearZ = nearZ;
	m_FarZ = farZ;

	m_NearWindowHeight = 2.0f * m_NearZ * tanf(0.5f * m_FovY);
	m_FarWindowHeight = 2.0f * m_FarZ * tanf(0.5f * m_FovY);

	XMStoreFloat4x4(&m_Proj, XMMatrixPerspectiveFovLH(m_FovY, m_Aspect, m_NearZ, m_FarZ));
}

void CameraN::SetViewPort(const D3D11_VIEWPORT& viewPort)
{
	m_ViewPort = viewPort;
}

void CameraN::SetViewPort(float topLeftX, float topLeftY, float width, float height, float minDepth, float maxDepth)
{
	m_ViewPort.TopLeftX = topLeftX;
	m_ViewPort.TopLeftY = topLeftY;
	m_ViewPort.Width = width;
	m_ViewPort.Height = height;
	m_ViewPort.MinDepth = minDepth;
	m_ViewPort.MaxDepth = maxDepth;
}

void XM_CALLCONV CameraN::LookAt(DirectX::FXMVECTOR pos, DirectX::FXMVECTOR target, DirectX::FXMVECTOR up)
{
	LookTo(pos, target - pos, up);
}

void CameraN::LookAt(const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& target, const DirectX::XMFLOAT3& up)
{
	LookAt(XMLoadFloat3(&pos), XMLoadFloat3(&target), XMLoadFloat3(&up));
}

void XM_CALLCONV CameraN::LookTo(DirectX::FXMVECTOR pos, DirectX::FXMVECTOR to, DirectX::FXMVECTOR up)
{
	XMVECTOR L = XMVector3Normalize(to);
	XMVECTOR R = XMVector3Normalize(XMVector3Cross(up, L));
	XMVECTOR U = XMVector3Cross(L, R);

	XMStoreFloat3(&m_location, pos);
	XMStoreFloat3(&m_Look, L);
	XMStoreFloat3(&m_Right, R);
	XMStoreFloat3(&m_Up, U);
}

void CameraN::LookTo(const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& to, const DirectX::XMFLOAT3& up)
{
	LookTo(XMLoadFloat3(&pos), XMLoadFloat3(&to), XMLoadFloat3(&up));
}

void CameraN::update(float dt)
{
	XMVECTOR R = XMLoadFloat3(&m_Right);
	XMVECTOR U = XMLoadFloat3(&m_Up);
	XMVECTOR L = XMLoadFloat3(&m_Look);
	XMVECTOR P = XMLoadFloat3(&m_location);

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
}
