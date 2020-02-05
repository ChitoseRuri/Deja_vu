#include "ResourceDepot.h"

ResourceDepot::ResourceDepot()
{
}

ResourceDepot::~ResourceDepot()
{
}

ComPtr<ID3D11ShaderResourceView> ResourceDepot::getShaderResource(size_t index) const
{
	return m_shaderResource[index];
}

ComPtr<ID3D11ShaderResourceView> ResourceDepot::getShaderResource(const std::wstring& name) const
{
	return m_shaderResource[m_shaderResourceMap.find(name)->second];
}

MeshBuffer ResourceDepot::getMeshBuffer(size_t index) const
{
	return MeshBuffer{ m_pVertexBuffer[index], m_pIndexBuffer[index], m_vertexIndexCount[index], m_vertexStride[index] };
}

MeshBuffer ResourceDepot::getMeshBuffer(const std::wstring& name) const
{
	size_t index = m_meshMap.find(name)->second;
	return MeshBuffer{ m_pVertexBuffer[index], m_pIndexBuffer[index], m_vertexIndexCount[index], m_vertexStride[index] };
}

size_t ResourceDepot::loadDDSTesture(ID3D11Device* pDevice, const wchar_t* fileName, std::wstring name)
{
	ComPtr<ID3D11ShaderResourceView> texture;
	HR(CreateDDSTextureFromFile(pDevice, fileName, nullptr, texture.ReleaseAndGetAddressOf()));
	size_t index = m_shaderResource.size();
	m_shaderResource.push_back(texture);
	if (!name.empty())
	{
		m_shaderResourceMap.insert(std::make_pair(std::move(name), index));
	}
	return index;
}

size_t ResourceDepot::loadImage(ID3D11Device* pDevice, const wchar_t* fileName, std::wstring name)
{
	ComPtr <ID3D11ShaderResourceView> texture;
	CreateWICTextureFromFile(pDevice, fileName, nullptr, texture.GetAddressOf());
	size_t index = m_shaderResource.size();
	m_shaderResource.push_back(texture);
	if (!name.empty())
	{
		m_shaderResourceMap.insert(std::make_pair(std::move(name), index));
	}
	return index;
}
