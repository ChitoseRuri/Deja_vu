#pragma once
#include "GameObject2D.h"
#include <string>
#include <map>
#include <assert.h>

class Label :
	public GameObject2D
{
private:
protected:
	static std::map<std::wstring, ComPtr<IDWriteTextFormat>> m_textFormatMap;
	static std::map<int, ComPtr<ID2D1SolidColorBrush>> m_solidColorBrushMap;

	ComPtr<IDWriteTextFormat> m_pTextFormat;
	ComPtr<ID2D1SolidColorBrush> m_pSolidColorBrush;
	std::wstring m_text;

public:
	Label();
	~Label();

	void setTextColor(int color);
	
	void setTextFormat(const std::wstring& name);
	void setTextFormat(const std::wstring& name,
		_In_z_ WCHAR const* fontFamilyName,
		_In_opt_ IDWriteFontCollection* fontCollection,
		DWRITE_FONT_WEIGHT fontWeight,
		DWRITE_FONT_STYLE fontStyle,
		DWRITE_FONT_STRETCH fontStretch,
		FLOAT fontSize,
		_In_z_ WCHAR const* localeName);

	void setText(const std::wstring& text);
	const auto& getText() const;

	void draw() override;
	void update(float dt) override;
};

