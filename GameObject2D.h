#pragma once
#include <wrl/client.h>
#include <d2d1.h>
#include <dwrite.h>
#include "d3dUtil.h"
#include "DXTrace.h"

template <class T>
using ComPtr = Microsoft::WRL::ComPtr<T>;

using namespace DirectX;

class GameObject2D
{
private:

protected:
	static ComPtr<ID2D1RenderTarget> m_pd2dRenderTarget;		// D2D渲染目标
	static ComPtr<IDWriteFactory> m_pdwriteFactory;				// DWrite工厂

	D2D1_RECT_F m_rect;

public:
	GameObject2D();
	virtual ~GameObject2D();

	static void init(ComPtr<ID2D1RenderTarget> pd2dRenderTarget, ComPtr<IDWriteFactory> pdwriteFactory);
	static void resize(ComPtr<ID2D1RenderTarget> pd2dRenderTarget, ComPtr<IDWriteFactory> pdwriteFactory);

	void setRect(const D2D1_RECT_F& rect);
	void setRect(float left, float top, float right, float bottom);
	const D2D1_RECT_F& getRect() const;


	virtual void update(float dt) = NULL;
	virtual void draw() = NULL;
};

