#include "Label.h"

std::map<std::wstring, ComPtr<IDWriteTextFormat>> Label::m_textFormatMap;
std::map<int, ComPtr<ID2D1SolidColorBrush>> Label::m_solidColorBrushMap;

Label::Label() :
	m_text(L"Label")
{
	m_rect = {0.0f, 100.0f, 600.0f, 200.0f};
	setTextColor(D2D1::ColorF::Black);
	if (m_textFormatMap.empty())
	{
		setTextFormat(L"default", L"宋体", nullptr, DWRITE_FONT_WEIGHT_NORMAL,
			DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 15, L"zh-cn");
	}
	else
	{
		setTextFormat(L"default");
	}
	
}

Label::~Label()
{
}

void Label::setTextColor(int color)
{
	auto itr = m_solidColorBrushMap.find(color);
	if (itr == m_solidColorBrushMap.end())
	{
		HR(m_pd2dRenderTarget->CreateSolidColorBrush(
			D2D1::ColorF(color),
			m_pSolidColorBrush.GetAddressOf()));

		m_solidColorBrushMap.insert(std::make_pair(color, m_pSolidColorBrush));
	}
	else
	{
		m_pSolidColorBrush = itr->second;
	}
}

void Label::setTextFormat(const std::wstring& name)
{
	auto itr = m_textFormatMap.find(name);
	assert(itr != m_textFormatMap.end());
	m_pTextFormat = itr->second;
}

void Label::setTextFormat(
	const std::wstring& name, 
	WCHAR const* fontFamilyName, 
	IDWriteFontCollection* fontCollection, 
	DWRITE_FONT_WEIGHT fontWeight, 
	DWRITE_FONT_STYLE fontStyle, 
	DWRITE_FONT_STRETCH fontStretch, 
	FLOAT fontSize, 
	WCHAR const* localeName)
{
	assert(m_textFormatMap.find(name) == m_textFormatMap.end());// 这个格式必然未被定义

	HR(m_pdwriteFactory->CreateTextFormat(
		fontFamilyName, 
		fontCollection, 
		fontWeight,
		fontStyle,
		fontStretch,
		fontSize,
		localeName,
		m_pTextFormat.GetAddressOf()));

	m_textFormatMap.insert(std::make_pair(name, m_pTextFormat));
}

void Label::setText(const std::wstring& text)
{
	m_text = text;
}

const auto& Label::getText() const
{
	return m_text;
}

void Label::draw()
{
	m_pd2dRenderTarget->BeginDraw();
	m_pd2dRenderTarget->DrawTextW(m_text.c_str(), m_text.length(), m_pTextFormat.Get(), m_rect, m_pSolidColorBrush.Get());
	HR(m_pd2dRenderTarget->EndDraw());
}