#include "impls\RTColor.h"
#include "impls\RTMath.h"


namespace RTRender
{

	CColor::CColor():
		r(0.0f), g(0.0f), b(0.0f)
	{
	}

	CColor::CColor(const CColor& color) :
		r(color.r), g(color.g), b(color.b)
	{
	}

	CColor::CColor(F32 r, F32 g, F32 b) :
		r(r), g(g), b(b)
	{
	}

	CColor::~CColor()
	{
	}

	CColor CColor::operator+ (const CColor& color) const
	{
		return CColor(r + color.r, g + color.g, b + color.b);
	}

	void CColor::operator+= (const CColor& color)
	{
		r += color.r;
		g += color.g;
		b += color.b;
	}

	CColor CColor::operator* (const F32& value) const
	{
		return CColor(r * value, g * value, b * value);
	}

	CColor CColor::operator* (const CColor& color) const
	{
		return CColor(r * color.r, g * color.g, b * color.b);
	}

	CColor CColor::ToRange()
	{
		r = CMath::Minf(r, 1.0f);
		g = CMath::Minf(g, 1.0f);
		b = CMath::Minf(b, 1.0f);

		return *this;
	}

	TR8G8B8Color CColor::ToR8G8B8Color() const
	{
		TR8G8B8Color convertedColor;

		convertedColor.mR = static_cast<unsigned char>(r * 255.0); 
		convertedColor.mG = static_cast<unsigned char>(g * 255.0);
		convertedColor.mB = static_cast<unsigned char>(b * 255.0);

		return convertedColor;
	}
}