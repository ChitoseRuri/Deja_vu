#include "GameObject.h"

ComPtr<ID2D1RenderTarget> GameObject::m_pd2dRenderTarget;		// D2D渲染目标
ComPtr<IDWriteFactory> GameObject::m_pdwriteFactory;			// DWrite工厂
ComPtr<ID3D11DeviceContext> GameObject::m_pd3dImmediateContext;

GameObject::GameObject()
{
}

GameObject::~GameObject()
{
}

void GameObject::init(ComPtr<ID2D1RenderTarget> pd2dRenderTarget, 
	ComPtr<IDWriteFactory> pdwriteFactory, 
	ComPtr<ID3D11DeviceContext> pd3dImmediateContext)
{
	m_pd2dRenderTarget = pd2dRenderTarget;
	m_pdwriteFactory = pdwriteFactory;
	m_pd3dImmediateContext = pd3dImmediateContext;
}
