#include "SkyBox.h"

SkyBox::SkyBox()
{
}

SkyBox::~SkyBox()
{
}

void SkyBox::init(ID3D11Device* pDevice, std::wstring fileName)
{
	if (fileName.substr(fileName.size() - 3) == L"DDS")
	{
		assert(true);
	}
	else
	{
		loadImageTexture(pDevice, std::move(fileName));
	}
}

void SkyBox::draw(ID3D11DeviceContext* pContext, const CBWorld& eyeCB)
{
	// 设置顶点/索引缓冲区
	UINT strides = m_vertexStride;
	UINT offsets = 0;
	pContext->IASetVertexBuffers(0, 1, m_pVertexBuffer.GetAddressOf(), &strides, &offsets);
	pContext->IASetIndexBuffer(m_pIndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);

	// 获取之前已经绑定到渲染管线上的常量缓冲区并进行修改
	ComPtr<ID3D11Buffer> cBuffer = nullptr;
	pContext->VSGetConstantBuffers(0, 1, cBuffer.GetAddressOf());

	// 更新常量缓冲区
	D3D11_MAPPED_SUBRESOURCE mappedData;
	HR(pContext->Map(cBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData));
	memcpy_s(mappedData.pData, sizeof(CBWorld), &eyeCB, sizeof(CBWorld));
	pContext->Unmap(cBuffer.Get(), 0);

	// 设置纹理
	pContext->PSSetShaderResources(0, 1, m_pTexture.GetAddressOf());
	// 可以开始绘制
	pContext->DrawIndexed(m_indexCount, 0, 0);
}

void SkyBox::loadImageTexture(ID3D11Device* pDevice,  std::wstring fileName)
{
	constexpr float hp = 1.0f / 3.0f;
	constexpr float wp = 1.0f / 4.0f;

	auto meshData = Geometry::CreateBox(100.0f, 100.0f, 100.0f);
	auto& vertexDataArr = meshData.vertexVec;

	// 右面(+X面)

	vertexDataArr[0].tex = { wp * 3.0f, hp * 2.0f };
	vertexDataArr[1].tex = { wp * 3.0f, hp };
	vertexDataArr[2].tex = { wp * 2.0f, hp };
	vertexDataArr[3].tex = { wp * 2.0f, hp * 2.0f };
	// 左面(-X面)

	vertexDataArr[4].tex = { wp, hp * 2.0f };
	vertexDataArr[5].tex = { wp, hp };
	vertexDataArr[6].tex = { 0.0f, hp };
	vertexDataArr[7].tex = { 0.0f, hp * 2.0f };
	// 顶面(+Y面)

	vertexDataArr[8].tex = { wp, 0.0f };
	vertexDataArr[9].tex = { wp, hp };
	vertexDataArr[10].tex = { wp * 2.0f, hp };
	vertexDataArr[11].tex = { wp * 2.0f, 0.0f };
	// 底面(-Y面)

	vertexDataArr[12].tex = { wp * 2.0f, 1.0f };
	vertexDataArr[13].tex = { wp * 2.0f, hp * 2.0f };
	vertexDataArr[14].tex = { wp, hp * 2.0f };
	vertexDataArr[15].tex = { wp, 1.0f };
	// 背面(+Z面)

	vertexDataArr[16].tex = { wp * 2.0f, hp * 2.0f };
	vertexDataArr[17].tex = { wp * 2.0f, hp };
	vertexDataArr[18].tex = { wp, hp };
	vertexDataArr[19].tex = { wp, hp * 2.0f };
	// 正面(-Z面)

	vertexDataArr[20].tex = { wp * 3.0f, hp * 2.0f };
	vertexDataArr[21].tex = { wp * 3.0f, hp };
	vertexDataArr[22].tex = { 1.0f, hp };
	vertexDataArr[23].tex = { 1.0f, hp * 2.0f };

	for (UINT i = 0; i < 4; ++i)
	{
		// 右面(+X面)
		vertexDataArr[i].normal = XMFLOAT3(1.0f, 0.0f, 0.0f);
		// 左面(-X面)
		vertexDataArr[i + 4].normal = XMFLOAT3(-1.0f, 0.0f, 0.0f);
		// 顶面(+Y面)
		vertexDataArr[i + 8].normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
		// 底面(-Y面)
		vertexDataArr[i + 12].normal = XMFLOAT3(0.0f, -1.0f, 0.0f);
		// 背面(+Z面)
		vertexDataArr[i + 16].normal = XMFLOAT3(0.0f, 0.0f, 1.0f);
		// 正面(-Z面)
		vertexDataArr[i + 20].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
	}

	for (UINT i = 0; i < 24; ++i)
	{
		meshData.vertexVec.push_back(vertexDataArr[i]);
	}

	// 反向索引
	meshData.indexVec = {
		0, 1, 2, 2, 3, 0,		// 右面(+X面)
		4, 5, 6, 6, 7, 4,		// 左面(-X面)
		8, 9, 10, 10, 11, 8,	// 顶面(+Y面)
		12, 13, 14, 14, 15, 12,	// 底面(-Y面)
		16, 17, 18, 18, 19, 16, // 背面(+Z面)
		20, 21, 22, 22, 23, 20	// 正面(-Z面)
	};

	size_t index = ResourceDepot::loadGeometry(pDevice, meshData, L"skybox");
	auto mesh = ResourceDepot::getMeshBuffer(index);
	m_pVertexBuffer = mesh.vertexBuffer;
	m_pIndexBuffer = mesh.indexBuffer;
	m_indexCount = mesh.count;
	m_vertexStride = mesh.vertexStride;

	index = ResourceDepot::loadImage(pDevice, L"Texture\\daylight.jpg", L"sky");

	m_pTexture = ResourceDepot::getShaderResource(index);
}
