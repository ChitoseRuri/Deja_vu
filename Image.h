#pragma once
#include "GameObject2D.h"
#include "XMF_MATH.h"
#include "Geometry.h"

class Image :
	public GameObject2D
{
private:
	static ComPtr<ID3D11Buffer> m_pVertexBuffer;
	static ComPtr<ID3D11Buffer> m_pIndexBuffer;

protected:
	ComPtr<ID3D11ShaderResourceView> m_pTexture;
	CBWorld m_cbWorld;
	float m_rotation;

public:
	Image();
	~Image();

	static void init(ID3D11Device* pDevice);

	void setSRV(ComPtr<ID3D11ShaderResourceView> texture);
	auto getTexture() const;

	void setRotation(float rotation);
	float getRoation() const;

	void setRect(const D2D1_RECT_F& rect) override;
	void setRect(float left, float top, float right, float bottom) override;

	void update(float dt) override;
	void draw() override;

private:
	void updateLocalMatrix();
};

