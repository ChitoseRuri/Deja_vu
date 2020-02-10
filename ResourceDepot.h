#pragma once
#include "GameObject3D.h"
#include "DDSTextureLoader.h"
#include "Geometry.h"
#include "d3dUtil.h"
#include "DXTrace.h"
#include <map>

class ResourceDepot
{
private:
	static std::vector<ComPtr<ID3D11ShaderResourceView>> m_shaderResource;
	static std::map<std::wstring, size_t> m_shaderResourceMap;

	static std::vector<ComPtr<ID3D11Buffer>> m_pVertexBuffer, m_pIndexBuffer;
	static std::vector<size_t> m_vertexIndexCount, m_vertexStride;
	static std::map<std::wstring, size_t> m_meshMap;

protected:
public:
	static ComPtr<ID3D11ShaderResourceView> getShaderResource(size_t index);
	static ComPtr<ID3D11ShaderResourceView> getShaderResource(const std::wstring& name);
	static size_t addShaderResource(ComPtr<ID3D11ShaderResourceView> pSRV, std::wstring name);

	static MeshBuffer getMeshBuffer(size_t index);
	static MeshBuffer getMeshBuffer(const std::wstring& name);

	static size_t loadDDSTesture(ID3D11Device* pDevice, const wchar_t* fileName, std::wstring name = L"");//DDS贴图读取
	static size_t loadImage(ID3D11Device* pDevice, const wchar_t* fileName, std::wstring name = L"");	// 读取图片
	
	template<class VertexType, class IndexType>
	static size_t loadGeometry(ID3D11Device* pDevice, const Geometry::MeshData<VertexType, IndexType>& meshData, std::wstring name = L"");// 针对Geometry类的读取

};

template<class VertexType, class IndexType>
static inline size_t ResourceDepot::loadGeometry(ID3D11Device* pDevice, const Geometry::MeshData<VertexType, IndexType>& meshData, std::wstring name)
{
	if (!name.empty())
	{
		auto itr = m_meshMap.find(name);
		if (itr != m_meshMap.end())
		{
			return itr->second;
		}
	}

	ComPtr<ID3D11Buffer> vertexBuffer, indexBuffer;

	// 设置顶点缓冲区描述
	size_t vertexStride = sizeof(VertexType);
	D3D11_BUFFER_DESC vbd;
	ZeroMemory(&vbd, sizeof(vbd));
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = (UINT)meshData.vertexVec.size() * vertexStride;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	// 新建顶点缓冲区
	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = meshData.vertexVec.data();
	HR(pDevice->CreateBuffer(&vbd, &InitData, vertexBuffer.GetAddressOf()));


	// 设置索引缓冲区描述
	size_t indexCount = (UINT)meshData.indexVec.size();
	D3D11_BUFFER_DESC ibd;
	ZeroMemory(&ibd, sizeof(ibd));
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = indexCount * sizeof(IndexType);
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	// 新建索引缓冲区
	InitData.pSysMem = meshData.indexVec.data();
	HR(pDevice->CreateBuffer(&ibd, &InitData, indexBuffer.GetAddressOf()));

	size_t index = m_pVertexBuffer.size();
	m_pVertexBuffer.push_back(vertexBuffer);
	m_pIndexBuffer.push_back(indexBuffer);
	m_vertexIndexCount.push_back(indexCount);
	m_vertexStride.push_back(vertexStride);
	if (!name.empty())
	{
		m_meshMap.insert(std::make_pair(std::move(name), index));
	}
	return index;
}
