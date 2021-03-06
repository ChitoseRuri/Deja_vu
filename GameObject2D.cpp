#include "GameObject2D.h"

std::vector<GameObject2D*> GameObject2D::m_gameObject2DList;
std::vector<GameObject2D*> GameObject2D::m_updateList;
std::multimap<GameObject2D::DEPTH, GameObject2D*> GameObject2D::m_drawMap;

float GameObject2D::m_viewWidth = 0.0f;
float GameObject2D::m_viewHeight = 0.0f;

GameObject2D::GameObject2D():
	m_rect{ 0.0f, 0.0f, 0.0f, 0.0f },
	m_depth(0.0f),
	m_status(NULL),
	m_updateIndex(-1),
	m_gameObject2DIndex(m_gameObject2DList.size()),
	m_updateFunction(nullptr)
{
	setVisable(true);
	setActive(true);
	m_gameObject2DList.push_back(this);
}

GameObject2D::~GameObject2D()
{
	setActive(false);
	setVisable(false);

	m_gameObject2DList.back()->m_gameObject2DIndex = m_gameObject2DIndex;
	m_gameObject2DList[m_gameObject2DIndex] = m_gameObject2DList.back();
	m_gameObject2DList.pop_back();
}

void GameObject2D::init(ComPtr<ID2D1RenderTarget> pd2dRenderTarget, ComPtr<IDWriteFactory> pdwriteFactory)
{
	m_pd2dRenderTarget = pd2dRenderTarget;
	m_pdwriteFactory = pdwriteFactory;
}

void GameObject2D::afterResize(ComPtr<ID2D1RenderTarget> pd2dRenderTarget, ComPtr<IDWriteFactory> pdwriteFactory, float viewWidth, float viewHeight)
{
	m_pd2dRenderTarget = pd2dRenderTarget;
	m_pdwriteFactory = pdwriteFactory;
	m_viewWidth = viewWidth;
	m_viewHeight = viewHeight;
}

void GameObject2D::setRect(const D2D1_RECT_F& rect)
{
	m_rect = rect;
}

void GameObject2D::setRect(float left, float top, float right, float bottom)
{
	m_rect = { left, top, right, bottom };
}

const D2D1_RECT_F& GameObject2D::getRect() const
{
	return m_rect;
}

void GameObject2D::setDepth(float depth)
{
	if (m_status & Status::visable)
	{
		setVisable(false);
		m_depth = depth;
		setVisable(true);
	}
	else
	{
		m_depth = depth;
	}
}

auto GameObject2D::getDepth() const
{
	return m_depth;
}

void GameObject2D::setVisable(bool lb)
{
	if (lb)
	{
		if (m_status & Status::visable)
		{
			return;
		}
		else
		{
			m_drawMap.insert(std::make_pair(DEPTH{ m_depth }, this));
			m_status |= Status::visable;
		}
	}
	else
	{
		if (m_status & Status::visable)
		{
			auto itr = m_drawMap.find(DEPTH{ m_depth });
			while (itr->second != this)
			{
				++itr;
			}
			m_drawMap.erase(itr);
			m_status &= ~Status::visable;
		}
		else
		{
			return;
		}
	}
}

bool GameObject2D::getVisable() const
{
	return m_status & Status::visable;
}

void GameObject2D::setActive(bool lb)
{
	if (lb)
	{
		if (m_status & Status::updateActive)
		{
			return;
		}
		else
		{
			m_updateIndex = m_updateList.size();
			m_updateList.push_back(this);
			m_status |= Status::updateActive;
		}
	}
	else
	{
		if (m_status & Status::updateActive)
		{
			m_updateList.back()->m_updateIndex = m_updateIndex;
			m_updateList[m_updateIndex] = m_updateList.back();
			m_updateList.pop_back();
			m_updateIndex = -1;
			m_status &= ~Status::updateActive;
		}
		else
		{
			return;
		}
	}
}

bool GameObject2D::getActive() const
{
	return m_status & Status::updateActive;
}

void GameObject2D::setUpdateFunction(std::function<void(float dt)> pf)
{
	m_updateFunction = pf;
}

auto GameObject2D::getUpdateFunction() const
{
	return m_updateFunction;
}

void GameObject2D::update(float dt)
{
	if(m_updateFunction)
		m_updateFunction(dt);
}

void GameObject2D::updateAll(float dt)
{
	for (auto p : m_updateList)
	{
		p->update(dt);
	}
}

void GameObject2D::drawAll()
{
	auto itr = m_drawMap.begin();
	while (itr != m_drawMap.end())
	{
		itr->second->draw();
		++itr;
	}
}

bool GameObject2D::isPointInRect(const POINT& pos) const
{
	return pos.x >= m_rect.left && pos.x <= m_rect.right && pos.y >= m_rect.top && pos.y <= m_rect.bottom;
}

bool GameObject2D::DEPTH::operator<(const DEPTH& rhs) const
{
	return depth > rhs.depth;
}
