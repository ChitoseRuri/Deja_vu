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

ComPtr<ID3D11ShaderResourceView> ResourceDepot::getShaderResource(const wchar_t* name) const
{
	return m_shaderResource[m_shaderResourceMap.find(name)->second];
}

MeshBuffer ResourceDepot::getMeshBuffer(size_t index) const
{
	return MeshBuffer{ m_vertexBuffer[index], m_indexBuffer[index], m_vertexIndexCount[index], m_vertexStride[index] };
}

MeshBuffer ResourceDepot::getMeshBuffer(const wchar_t* name) const
{
	size_t index = m_meshMap.find(name)->second;
	return MeshBuffer{ m_vertexBuffer[index], m_indexBuffer[index], m_vertexIndexCount[index], m_vertexStride[index] };
}

size_t ResourceDepot::loadDDSTesture(ID3D11Device* pDevice, const wchar_t* fileName, const wchar_t* name)
{
	ComPtr<ID3D11ShaderResourceView> texture;
	HR(CreateDDSTextureFromFile(pDevice, fileName, nullptr, texture.ReleaseAndGetAddressOf()));
	size_t index = m_shaderResource.size();
	m_shaderResource.push_back(texture);
	if (name != nullptr)
	{
		m_shaderResourceMap.insert(std::make_pair(name, index));
	}
	return index;
}
