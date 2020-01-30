#include "GameObject.h"

std::vector<GameObject*> GameObject::m_gameObjectList, GameObject::m_updateList, GameObject::m_drawList;

GameObject::GameObject() :
	m_parent(nullptr),
	m_location(.0f, .0f, .0f),
	m_scale(1.0f, 1.0f, 1.0f),
	m_rotation(.0f, .0f, .0f),
	m_locationP(.0f, .0f, .0f),
	m_scaleP(1.0f, 1.0f, 1.0f),
	m_rotationP(.0f, .0f, .0f),
	m_status(NULL),// 不依靠初始化列表，需要用函数初始化状态
	m_isMoved(false),
	m_drawIndex(-1),
	m_updateIndex(-1),
	m_localMatrix(XMMatrixIdentity())
{
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

GameObject::~GameObject()
{
	m_gameObjectList[m_gameObjectIndex] = nullptr;
	m_gameObjectIndex = -1;
}

void GameObject::updateAll(float dt)
{
	for (auto p : m_updateList)
	{
		p->update(dt);
	}
}

void GameObject::drawAll(ID3D11DeviceContext* pDeviceContext)
{
	for (auto p : m_drawList)
	{
		p->draw(pDeviceContext);
	}
}

const XMFLOAT3& GameObject::getLocation() const
{
	return m_location;
}

void GameObject::setLocation(float x, float y, float z)
{
	m_location = { x, y, z };
	XMFLOAT3 ls = { m_locationP.x + m_location.x,m_locationP.y + m_location.y, m_locationP.z + m_location.z };
	for (auto child : m_childen)
	{
		setLocationP(ls);
	}
	m_isMoved = true;
}

void GameObject::setLocation(const XMFLOAT3& location)
{
	m_location = location;
	XMFLOAT3 ls = { m_locationP.x + m_location.x,m_locationP.y + m_location.y, m_locationP.z + m_location.z };
	for (auto child : m_childen)
	{
		setLocationP(ls);
	}
	m_isMoved = true;
}

const XMFLOAT3& GameObject::getScale() const
{
	return m_scale;
}

void GameObject::setScale(float x, float y, float z)
{
	m_scale = { x, y, z };
	XMFLOAT3 ls = { m_scaleP.x * m_scale.x,m_scaleP.y * m_scale.y,m_scaleP.z * m_scale.z };
	for (auto child : m_childen)
	{
		setScaleP(ls);
	}
	m_isMoved = true;
}

void GameObject::setScale(const XMFLOAT3& scale)
{
	m_scale = scale;
	XMFLOAT3 ls = { m_scaleP.x * m_scale.x,m_scaleP.y * m_scale.y,m_scaleP.z * m_scale.z };
	for (auto child : m_childen)
	{
		setScaleP(ls);
	}
	m_isMoved = true;
}

const XMFLOAT3& GameObject::getRotation() const
{
	return m_rotation;
}

void GameObject::setRotation(float x, float y, float z)
{
	m_rotation = { x, y, z };
	XMFLOAT3 ls = { m_rotationP.x + m_rotation.x,m_rotationP.y + m_rotation.y,m_rotationP.z + m_rotation.z };
	for (auto child : m_childen)
	{
		setRotationP(ls);
	}
	m_isMoved = true;
}

void GameObject::setRotation(const XMFLOAT3& rotation)
{
	m_rotation = rotation;
	m_isMoved = true;
}

const bool GameObject::getVisable() const
{
	return m_status & Status::visable;
}

void GameObject::setVisable(bool lb)
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

const bool GameObject::getActiveUpdate() const
{
	return m_status & Status::updateActive;
}

void GameObject::setActiveUpdate(bool lb)
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

void GameObject::setTexture(ComPtr<ID3D11ShaderResourceView> texture)
{
	m_texture = texture;
}

auto GameObject::getTexture() const
{
	return m_texture;
}

void GameObject::setMeshbuffer(const MeshBuffer& meshBuffer)
{
	m_vertexBuffer = meshBuffer.vertexBuffer;
	m_indexBuffer = meshBuffer.indexBuffer;
	m_indexCount = meshBuffer.count;
	m_vertexStride = meshBuffer.vertexStride;
}

auto GameObject::getMeshBuffer() const
{
	return MeshBuffer{ m_vertexBuffer, m_indexBuffer, m_indexCount };
}

void GameObject::update(float dt)
{
	updateLocalMatrix();
}

void GameObject::draw(ID3D11DeviceContext* pDeviceContext)
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
	cbDrawing.world = XMMatrixTranspose(m_localMatrix);
	cbDrawing.worldInvTranspose = XMMatrixInverse(nullptr, m_localMatrix);	// 两次转置抵消

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



void GameObject::setDebugObjectName(const std::string& name)
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

void GameObject::updateLocalMatrix()
{
	if (!m_isMoved)
	{
		return;
	}
	XMFLOAT3 scale = { m_scale.x * m_scaleP.x, m_scale.y * m_scaleP.y, m_scale.z * m_scaleP.z };
	XMFLOAT3 rotation = { m_rotation.x + m_rotationP.x, m_rotation.y + m_rotationP.y, m_rotation.z + m_rotationP.z };
	XMFLOAT3 location = { m_location.x + m_locationP.x, m_location.y + m_locationP.y, m_location.z + m_locationP.z };
	XMFLOAT4X4 scaleF4 = {
		scale.x, 0, 0, 0,
		0, scale.y, 0, 0,
		0, 0, scale.z, 0,
		0,0,0,1
	};
	XMMATRIX scaleMatrix = XMLoadFloat4x4(&scaleF4);
	XMMATRIX rotationMatrix = XMMatrixRotationX(rotation.x) * XMMatrixRotationY(rotation.y) * XMMatrixRotationZ(rotation.z);
	XMFLOAT4X4 locationF4 = {
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		location.x, location.y, location.z, 1
	};
	XMMATRIX locationMatrix = XMLoadFloat4x4(&locationF4);
	m_localMatrix = scaleMatrix * rotationMatrix * locationMatrix;
}

void GameObject::setLocationP(const XMFLOAT3& location)
{
	m_locationP = location;
	XMFLOAT3 ls = { location.x + m_location.x,location.y + m_location.y,location.z + m_location.z };
	for (auto child : m_childen)
	{
		setLocationP(ls);
	}
	m_isMoved = true;
}

void GameObject::setScaleP(const XMFLOAT3& scale)
{
	m_scaleP = scale;
	XMFLOAT3 ls = { scale.x * m_scale.x,scale.y * m_scale.y,scale.z * m_scale.z };
	for (auto child : m_childen)
	{
		setScaleP(ls);
	}
	m_isMoved = true;
}

void GameObject::setRotationP(const XMFLOAT3& rotation)
{
	m_rotationP = rotation;
	XMFLOAT3 ls = { rotation.x + m_rotation.x,rotation.y + m_rotation.y,rotation.z + m_rotation.z };
	for (auto child : m_childen)
	{
		setRotationP(ls);
	}
	m_isMoved = true;
}
