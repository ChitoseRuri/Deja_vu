#pragma once
#include "d3dApp.h"
#include "d3dUtil.h"

template <class T>
using ComPtr = Microsoft::WRL::ComPtr<T>;

struct  CBWorld
{
	DirectX::XMMATRIX world;
	DirectX::XMMATRIX worldInvTranspose;
};

class GameObject
{
protected:
	enum Status
	{
		visable = 1,
		updateActive = 1 << 1,
		updatePassive = 1 << 2,
		updateAP = updateActive | updatePassive,
		statical = 1 << 3
	};

	static ComPtr<ID2D1RenderTarget> m_pd2dRenderTarget;		// D2D渲染目标
	static ComPtr<IDWriteFactory> m_pdwriteFactory;				// DWrite工厂
	static ComPtr<ID3D11DeviceContext> m_pd3dImmediateContext;	// D3D

public:
	GameObject();
	virtual ~GameObject();

	static void init(
		ComPtr<ID2D1RenderTarget> pd2dRenderTarget, 
		ComPtr<IDWriteFactory> pdwriteFactory,
		ComPtr<ID3D11DeviceContext> pd3dImmediateContext
	);
};

