#include "GameObject.h"

std::vector<GameObject*> GameObject::m_gameObjectList, GameObject::m_updateList, GameObject::m_drawList;

constexpr float toRadian(float angle)
{
	return angle * 0.017453f;
}

constexpr float toAngle(float radian)
{
	return radian * 57.29578f;
}

// 向量相加
XMFLOAT3 operator+(const XMFLOAT3& lhs, const XMFLOAT3& rhs)
{
	return { lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z };
}

// 向量点乘
XMFLOAT3 operator*(const XMFLOAT3& lhs, const XMFLOAT3& rhs)
{
	return { lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z };
}

GameObject::GameObject() :
	m_parent(nullptr),
	m_location(.0f, .0f, .0f),
	m_scale(1.0f, 1.0f, 1.0f),
	m_rotation(.0f, .0f, .0f),
	m_locationP(.0f, .0f, .0f),
	m_scaleP(1.0f, 1.0f, 1.0f),
	m_rotationP(.0f, .0f, .0f),
	m_status(NULL),// 不依靠初始化列表，需要用函数初始化状态
	m_trans(NULL),
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
		if (p)
		{
			p->draw(pDeviceContext);
		}
	}
}

const XMFLOAT3& GameObject::getLocation() const
{
	return m_location;
}

void GameObject::setLocation(float x, float y, float z)
{
	m_location = { x, y, z };
	m_trans |= Trans::locate_t;
}

void GameObject::setLocation(const XMFLOAT3& location)
{
	m_location = location;
	m_trans |= Trans::locate_t;
}

const XMFLOAT3& GameObject::getScale() const
{
	return m_scale;
}

void GameObject::setScale(float x, float y, float z)
{
	m_scale = { x, y, z };
	m_trans |= Trans::scale_t;
}

void GameObject::setScale(const XMFLOAT3& scale)
{
	m_scale = scale;
	m_trans |= Trans::scale_t;
}

const XMFLOAT3& GameObject::getRotation() const
{
	return m_rotation;
}

void GameObject::setRotation(float x, float y, float z)
{
	m_rotation = { x, y, z };
	m_trans |= Trans::rotate_t;
}

void GameObject::setRotation(const XMFLOAT3& rotation)
{
	m_rotation = rotation;
	m_trans |= Trans::rotate_t;
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
	for (auto child : m_childen)
	{
		child->update(dt);
	}
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

void GameObject::setParent(GameObject* parent)
{
	if (parent == m_parent)
	{
		return;
	}

	if (parent == nullptr)//亲节点为空，自己主动更新
	{
		m_parent = parent;
		setActiveUpdate(true);
		m_scaleP = { 1.0f,1.0f,1.0f };
		m_rotation = { 0.0f,0.0f,0.0f };
		m_locationP = { 0.0f, 0.0f, 0.0f };
		m_trans = UINT32_MAX;
	}
	else
	{
		parent->addChildPassive(this);
		m_parent = parent;
		setActiveUpdate(false);// 关掉主动更新，改为亲节点更新
	}
}

GameObject* GameObject::getParent() const
{
	return m_parent;
}

void GameObject::addChild(GameObject* child)
{
	addChildPassive(child);
	child->setActiveUpdate(false);
	child->setParentPassive(this);
}

void GameObject::delChild(GameObject* child)
{
	size_t size = m_childen.size();
	for (size_t index = 0; index < size; ++index)
	{
		if (m_childen[index] == child)
		{
			m_childen.erase(m_childen.begin() + index);
			child->setParent(nullptr);
			child->setActiveUpdate(true);
			return;
		}
	}
	assert(true);
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
	if (!m_trans)
	{
		return;
	}
	XMFLOAT3 scale = m_scale * m_scaleP;
	XMFLOAT3 rotation = m_rotation + m_rotationP;
	XMFLOAT3 location = m_location + m_locationP;
	XMFLOAT4X4 scaleF4 = {
		scale.x, 0, 0, 0,
		0, scale.y, 0, 0,
		0, 0, scale.z, 0,
		0,0,0,1
	};
	XMMATRIX scaleMatrix = XMLoadFloat4x4(&scaleF4);
	XMMATRIX rotationMatrix = XMMatrixRotationX(toRadian(rotation.x)) * XMMatrixRotationY(toRadian(rotation.y)) * XMMatrixRotationZ(toRadian(rotation.z));
	XMFLOAT4X4 locationF4 = {
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		location.x, location.y, location.z, 1
	};
	XMMATRIX locationMatrix = XMLoadFloat4x4(&locationF4);
	m_localMatrix = scaleMatrix * rotationMatrix * locationMatrix;
	// 根据更新总量更新子节点
	if (m_trans | Trans::scale_t)
	{
		for (auto child : m_childen)
		{
			child->setScaleP(scale);
		}
	}
	if (m_trans | Trans::rotate_t)
	{
		for (auto child : m_childen)
		{
			child->setRotationP(rotation);
		}
	}
	if (m_trans | Trans::locate_t)
	{
		for (auto child : m_childen)
		{
			child->setLocationP(location);
		}
	}
	m_trans = NULL;// 重置修改判断符
}

void GameObject::setLocationP(const XMFLOAT3& location)
{
	m_locationP = location;
	m_trans |= Trans::locate_t;
}

void GameObject::setScaleP(const XMFLOAT3& scale)
{
	m_scaleP = scale;
	m_trans |= Trans::scale_t;
}

void GameObject::setRotationP(const XMFLOAT3& rotation)
{
	m_rotationP = rotation;
	m_trans |= rotate_t;
}

void GameObject::setParentPassive(GameObject* parent)
{
	m_parent = parent;
}

void GameObject::addChildPassive(GameObject* child)
{
	if (child->getParent() == this)
	{
		return;
	}

	m_childen.push_back(child);
	child->setScaleP(m_scale * m_scaleP);
	child->setRotationP(m_rotation * m_rotationP);
	child->setLocationP(m_location + m_locationP);
}
