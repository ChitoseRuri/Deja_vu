#include "ResourceDepot.h"

std::vector<ComPtr<ID3D11ShaderResourceView>> ResourceDepot::m_shaderResource;
std::map<std::wstring, size_t> ResourceDepot::m_shaderResourceMap;

std::vector<ComPtr<ID3D11Buffer>> ResourceDepot::m_pVertexBuffer;
std::vector<ComPtr<ID3D11Buffer>> ResourceDepot::m_pIndexBuffer;
std::vector<size_t> ResourceDepot::m_vertexIndexCount;
std::vector<size_t> ResourceDepot::m_vertexStride;
std::map<std::wstring, size_t> ResourceDepot::m_meshMap;

ComPtr<ID3D11ShaderResourceView> ResourceDepot::getShaderResource(size_t index)
{
	return m_shaderResource[index];
}

ComPtr<ID3D11ShaderResourceView> ResourceDepot::getShaderResource(const std::wstring& name)
{
	return m_shaderResource[m_shaderResourceMap.find(name)->second];
}

size_t ResourceDepot::addShaderResource(ComPtr<ID3D11ShaderResourceView> pSRV, std::wstring name)
{
	size_t index = m_shaderResource.size();
	m_shaderResource.push_back(pSRV);
	m_shaderResourceMap.insert(std::make_pair(std::move(name), index));
	return index;
}

MeshBuffer ResourceDepot::getMeshBuffer(size_t index)
{
	if (index < m_pIndexBuffer.size())
		return MeshBuffer{ m_pVertexBuffer[index], m_pIndexBuffer[index], m_vertexIndexCount[index], m_vertexStride[index] };
	else
		return { nullptr, nullptr, NULL, NULL };
}

MeshBuffer ResourceDepot::getMeshBuffer(const std::wstring& name)
{
	auto itr = m_meshMap.find(name);
	if (itr != m_meshMap.end())
	{
		size_t index = m_meshMap.find(name)->second;
		return MeshBuffer{ m_pVertexBuffer[index], m_pIndexBuffer[index], m_vertexIndexCount[index], m_vertexStride[index] };
	}
	else
	{
		return { nullptr, nullptr, NULL, NULL };
	}
}

size_t ResourceDepot::loadDDSTesture(ID3D11Device* pDevice, const wchar_t* fileName, std::wstring name)
{
	if (!name.empty())
	{
		auto itr = m_shaderResourceMap.find(name);
		if (itr != m_shaderResourceMap.end()) // 这个名字的srv已经存在，直接返回
		{
			return itr->second;
		}
	}

	ComPtr<ID3D11ShaderResourceView> pSRV;
	HR(CreateDDSTextureFromFile(pDevice, fileName, nullptr, pSRV.ReleaseAndGetAddressOf()));
	size_t index = m_shaderResource.size();
	m_shaderResource.push_back(pSRV);
	if (!name.empty())
	{
		m_shaderResourceMap.insert(std::make_pair(std::move(name), index));
	}
	return index;
}

size_t ResourceDepot::loadImage(ID3D11Device* pDevice, const wchar_t* fileName, std::wstring name)
{
	if (!name.empty())
	{
		auto itr = m_shaderResourceMap.find(name);
		if (itr != m_shaderResourceMap.end()) // 这个名字的srv已经存在，直接返回
		{
			return itr->second;
		}
	}

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
