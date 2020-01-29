#include "GameObject.h"

std::vector<GameObjectN*> GameObjectN::m_gameObjectList, GameObjectN::m_updateList, GameObjectN::m_drawList;

GameObjectN::GameObjectN() :
	m_parent(nullptr),
	m_location(.0f, .0f, .0f),
	m_scale(1.0f, 1.0f, 1.0f),
	m_rotation(.0f, .0f, .0f),
	m_status(Status::visable | Status::updateActive),
	m_drawIndex(-1),
	m_updateIndex(-1)
{
	XMStoreFloat4x4(&m_localMatrix, XMMatrixIdentity());

	size_t index = -1;
	size_t count = m_gameObjectList.size();
	for (size_t i = 0; i < count; ++i)
	{
		if (m_gameObjectList[i] == nullptr)
		{
			m_gameObjectList[i] = this;
			index = i;
			break;
		}
	}
	if (index == -1)
	{
		m_gameObjectList.push_back(this);
		m_gameObjectIndex = count;
	}

	setActiveUpdate(true);
	setVisable(true);
}

GameObjectN::~GameObjectN()
{
	m_gameObjectList[m_gameObjectIndex] = nullptr;
	m_gameObjectIndex = -1;
}

void GameObjectN::updateAll(float dt)
{
	for (auto p : m_updateList)
	{
		p->update(dt);
	}
}

void GameObjectN::drawAll(ID3D11DeviceContext* pDeviceContext)
{
	// TODO
}

const XMFLOAT3& GameObjectN::getLocation() const
{
	return m_location;
}

void GameObjectN::setLocation(float x, float y, float z)
{
	m_location = { x, y, z };
}

void GameObjectN::setLocation(const XMFLOAT3& location)
{
	m_location = location;
}

const XMFLOAT3& GameObjectN::getScale() const
{
	return m_scale;
}

void GameObjectN::setScale(float x, float y, float z)
{
	m_scale = { x, y, z };
}

void GameObjectN::setScale(const XMFLOAT3& scale)
{
	m_scale = scale;
}

const XMFLOAT3& GameObjectN::getRotation() const
{
	return m_rotation;
}

void GameObjectN::setRotation(float x, float y, float z)
{
	m_rotation = { x, y, z };
}

void GameObjectN::setRotation(const XMFLOAT3& rotation)
{
	m_rotation = rotation;
}

const bool GameObjectN::getVisable() const
{
	return m_status & Status::visable;
}

void GameObjectN::setVisable(bool lb)
{
	if (lb && m_drawIndex == -1)
	{
		m_status |= Status::visable;
		size_t count = m_drawList.size();
		for (size_t i = 0; i < count; ++i)
		{
			if (m_drawList[i] == nullptr)
			{
				m_drawList[i] = this;
				m_drawIndex = i;
				break;
			}
		}
		if (m_drawIndex == -1)
		{
			m_drawIndex = count;
			m_drawList.push_back(this);
		}
	}
	else if(!lb && m_drawIndex != -1)
	{
		m_status &= ~Status::visable;
		m_drawList[m_drawIndex] = nullptr;
		m_drawIndex = -1;
	}
}

const bool GameObjectN::getActiveUpdate() const
{
	return m_status & Status::updateActive;
}

void GameObjectN::setActiveUpdate(bool lb)
{
	if (lb && m_updateIndex == -1)
	{
		m_status |= Status::updateActive;
		size_t count = m_updateList.size();
		for (size_t i = 0; i < count; ++i)
		{
			if (m_updateList[i] == nullptr)
			{
				m_updateList[i] = this;
				m_updateIndex = i;
				break;
			}
		}
		if (m_updateIndex == -1)
		{
			m_updateIndex = count;
			m_updateList.push_back(this);
		}
	}
	else if(!lb && m_updateIndex != -1)
	{
		m_status &= ~Status::updateActive;
		m_updateList[m_updateIndex] = nullptr;
		m_updateIndex = -1;
	}
}

void GameObjectN::setTexture(ComPtr<ID3D11ShaderResourceView> texture)
{
	m_texture = texture;
}

auto GameObjectN::getTexture() const
{
	return m_texture;
}

void GameObjectN::setMeshbuffer(const MeshBuffer& meshBuffer)
{
	m_vertexBuffer = meshBuffer.vertexBuffer;
	m_indexBuffer = meshBuffer.indexBuffer;
	m_indexCount = meshBuffer.count;
	m_vertexStride = meshBuffer.vertexStride;
}

auto GameObjectN::getMeshBuffer() const
{
	return MeshBuffer{ m_vertexBuffer, m_indexBuffer, m_indexCount };
}

void GameObjectN::update(float dt)
{
}

void GameObjectN::draw(ID3D11DeviceContext* pDeviceContext)
{
	// 设置顶点/索引缓冲区
	UINT strides = m_vertexStride;
	UINT offsets = 0;
	pDeviceContext->IASetVertexBuffers(0, 1, m_vertexBuffer.GetAddressOf(), &strides, &offsets);
	pDeviceContext->IASetIndexBuffer(m_indexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);

	// 获取之前已经绑定到渲染管线上的常量缓冲区并进行修改
	ComPtr<ID3D11Buffer> cBuffer = nullptr;
	pDeviceContext->VSGetConstantBuffers(0, 1, cBuffer.GetAddressOf());
	CBWorld cbDrawing;

	// 内部进行转置，这样外部就不需要提前转置了
	XMMATRIX W = XMLoadFloat4x4(&m_localMatrix);
	cbDrawing.world = XMMatrixTranspose(W);
	cbDrawing.worldInvTranspose = XMMatrixInverse(nullptr, W);	// 两次转置抵消

	// 更新常量缓冲区
	D3D11_MAPPED_SUBRESOURCE mappedData;
	HR(pDeviceContext->Map(cBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData));
	memcpy_s(mappedData.pData, sizeof(CBWorld), &cbDrawing, sizeof(CBWorld));
	pDeviceContext->Unmap(cBuffer.Get(), 0);

	// 设置纹理
	pDeviceContext->PSSetShaderResources(0, 1, m_texture.GetAddressOf());
	// 可以开始绘制
	pDeviceContext->DrawIndexed(m_indexCount, 0, 0);
}

void GameObjectN::setDebugObjectName(const std::string& name)
{
#if (defined(DEBUG) || defined(_DEBUG)) && (GRAPHICS_DEBUGGER_OBJECT_NAME)
	std::string vbName = name + ".VertexBuffer";
	std::string ibName = name + ".IndexBuffer";
	m_vertexBuffer->SetPrivateData(WKPDID_D3DDebugObjectName, static_cast<UINT>(vbName.length()), vbName.c_str());
	m_indexBuffer->SetPrivateData(WKPDID_D3DDebugObjectName, static_cast<UINT>(ibName.length()), ibName.c_str());
#else
	UNREFERENCED_PARAMETER(name);
#endif
}
