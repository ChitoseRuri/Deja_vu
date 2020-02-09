#pragma once
#include "GameObject.h"
#include "DXTrace.h"
#include <functional>
#include <map>

template <class T>
using ComPtr = Microsoft::WRL::ComPtr<T>;

using namespace DirectX;

enum class Status2D:char
{
	null = 0,
	selected = 1,
	pressed = 1 << 1,
	released = 1 << 2
};

class GameObject2D:
	public GameObject
{
private:
	struct DEPTH
	{
		float depth;

		bool operator<(const DEPTH& rhs) const;
	};

	static std::vector<GameObject2D*> m_gameObject2DList;
	static std::vector<GameObject2D*> m_updateList;
	static std::multimap<DEPTH, GameObject2D*> m_drawMap;
	size_t m_gameObject2DIndex, m_updateIndex;

protected:
	static float m_viewWidth, m_viewHeight;


	D2D1_RECT_F m_rect;
	float m_depth;
	UINT32 m_status;
	Status2D m_status2D;
	std::function<void()> m_selectFunction, m_pressFunction, m_releaseFunction;

public:
	GameObject2D();
	virtual ~GameObject2D();

	static void init(ComPtr<ID2D1RenderTarget> pd2dRenderTarget, ComPtr<IDWriteFactory> pdwriteFactory);
	static void afterResize(ComPtr<ID2D1RenderTarget> pd2dRenderTarget, ComPtr<IDWriteFactory> pdwriteFactory, float viewWidth, float viewHeight);

	virtual void setRect(const D2D1_RECT_F& rect);
	virtual void setRect(float left, float top, float right, float bottom);
	const D2D1_RECT_F& getRect() const;

	void setDepth(float depth);
	auto getDepth() const;

	void setVisable(bool lb);
	bool getVisable() const;

	void setActive(bool lb);
	bool getActive() const;

	void setStatus2D(Status2D status);
	Status2D getStatus2D() const;

	void setSelectFunction(std::function<void()> pf);
	auto getSelectFunction() const;

	void setPressFunction(std::function<void()> pf);
	auto getPressFunction() const;

	void setReleaseFunction(std::function<void()> pf);
	auto getReleaseFunction() const;

	virtual void update(float dt) = NULL;
	virtual void draw() = NULL;

	static void updateAll(float dt);
	static void drawAll();
};

