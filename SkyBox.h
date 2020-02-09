#pragma once
#include "d3dUtil.h"
#include "d3dApp.h"
#include "DDSTextureLoader.h"
#include "WICTextureLoader.h"
#include "ResourceDepot.h"
#include "Geometry.h"

class SkyBox
{
private:
protected:
	template <class T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;

	ComPtr<ID3D11ShaderResourceView> m_pTexture;
	ComPtr<ID3D11Buffer> m_pVertexBuffer;
	ComPtr<ID3D11Buffer> m_pIndexBuffer;
	size_t m_indexCount;
	unsigned m_vertexStride;

public:
	SkyBox();
	~SkyBox();

	void init(ID3D11Device* pDevice, std::wstring fileName);
	void init(ID3D11Device* pDevice, std::vector<std::wstring> fileNames) = delete;
	void draw(ID3D11DeviceContext* pContext, const CBWorld& eyeCB);

protected:

private:
	void loadImageTexture(ID3D11Device* pDevice, std::wstring fileName);
};