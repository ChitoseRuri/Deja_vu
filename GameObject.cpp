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
	// TODO
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
