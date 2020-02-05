#pragma once
#include "GameObject3D.h"

class Camera:
	public GameObject3D
{
private:

protected:
	XMFLOAT3 m_Right;
	XMFLOAT3 m_Up;
	XMFLOAT3 m_Look;
	// 视锥体属性
	float m_nearZ;
	float m_farZ;
	float m_aspect;
	float m_fovY;
	float m_NearWindowHeight;
	float m_FarWindowHeight;

	// 观察矩阵和透视投影矩阵
	XMFLOAT4X4 m_View;
	XMFLOAT4X4 m_proj;

	// 当前视口
	D3D11_VIEWPORT m_ViewPort;

	//char pad[14];//打包对齐16

public:
	Camera();
	virtual ~Camera();

	XMVECTOR getLocationXM() const;

	// 获取摄像机的坐标轴向量
	XMVECTOR getRightXM() const;
	XMFLOAT3 getRight() const;
	XMVECTOR getUpXM() const;
	XMFLOAT3 getUp() const;
	XMVECTOR getLookXM() const;
	XMFLOAT3 getLook() const;

	// 获取矩阵
	XMMATRIX getViewXM() const;
	XMMATRIX getProjXM() const;
	XMMATRIX getViewProjXM() const;

	// 获取视口
	D3D11_VIEWPORT getViewPort() const;

	// 设置视锥体
	void setFrustum(float fovY, float aspect, float nearZ, float farZ);

	// 设置视口
	void setViewPort(const D3D11_VIEWPORT& viewPort);
	void setViewPort(float topLeftX, float topLeftY, float width, float height, float minDepth = 0.0f, float maxDepth = 1.0f);

	// 设置摄像机的朝向
	void XM_CALLCONV lookAt(DirectX::FXMVECTOR pos, DirectX::FXMVECTOR target, DirectX::FXMVECTOR up);
	void lookAt(const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& target, const DirectX::XMFLOAT3& up);
	void XM_CALLCONV lookTo(DirectX::FXMVECTOR pos, DirectX::FXMVECTOR to, DirectX::FXMVECTOR up);
	void lookTo(const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& to, const DirectX::XMFLOAT3& up);

	void update(float dt) override;
};

