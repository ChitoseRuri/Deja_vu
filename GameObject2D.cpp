#include "GameObject2D.h"

ComPtr<ID2D1RenderTarget> GameObject2D::m_pd2dRenderTarget;		// D2D渲染目标
ComPtr<IDWriteFactory> GameObject2D::m_pdwriteFactory;			// DWrite工厂

GameObject2D::GameObject2D():
	m_rect{ 0.0f, 0.0f, 0.f, 0.0f }
{
}

GameObject2D::~GameObject2D()
{
}

void GameObject2D::init(ComPtr<ID2D1RenderTarget> pd2dRenderTarget, ComPtr<IDWriteFactory> pdwriteFactory)
{
	m_pd2dRenderTarget = pd2dRenderTarget;
	m_pdwriteFactory = pdwriteFactory;
}

void GameObject2D::resize(ComPtr<ID2D1RenderTarget> pd2dRenderTarget, ComPtr<IDWriteFactory> pdwriteFactory)
{
	m_pd2dRenderTarget = pd2dRenderTarget;
	m_pdwriteFactory = pdwriteFactory;
}

void GameObject2D::setRect(const D2D1_RECT_F& rect)
{
	m_rect = rect;
}

void GameObject2D::setRect(float left, float top, float right, float bottom)
{
	m_rect = { left, top, right, bottom };
}

const D2D1_RECT_F& GameObject2D::getRect() const
{
	return m_rect;
}